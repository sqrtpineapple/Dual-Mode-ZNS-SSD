#include "zns-ftl.h"

#define NVME_DEFAULT_ZONE_SIZE      (128 * MiB)

static inline struct zns_ssd_channel *get_ch(struct ZnsCtrl *znssd, struct pba *pba)
{
    return &(znssd->ch[pba->ch]);
}

static inline struct zns_nand_lun *get_lun(struct ZnsCtrl *znssd, struct pba *pba)
{
    struct zns_ssd_channel *ch = get_ch(znssd, pba);
    return &(ch->lun[pba->lun]);
}

static inline struct zns_nand_plane *get_pl(struct ZnsCtrl *znssd, struct pba *pba)
{
    struct zns_nand_lun *lun = get_lun(znssd, pba);
    return &(lun->pl[pba->pl]);
}

static inline void check_addr(int a, int max)
{
    ftl_assert(a >= 0 && a < max);
}

static inline uint32_t zns_ssd_zone_idx(NvmeNamespace *ns, uint64_t slba)
{
    FemuCtrl *n = ns->ctrl;

    return (n->zone_size_log2 > 0 ? slba >> n->zone_size_log2 : slba /
            n->zone_size);
}

static void zns_ssd_init_params(FemuCtrl *n)
{
    struct ZnsCtrl *znssd = n->zns_ctrl;
    struct znsparams *zpp = &znssd->zp;

    zpp->secsz = 512;
    zpp->secs_per_pg = 8;
    zpp->pgs_per_slc_blk = 256;      /* SLC block size: 1MB */
    zpp->pgs_per_qlc_blk = 1024;     /* QLC block size: 4MB */
    zpp->blks_per_pl = 64;
    zpp->pls_per_lun = 1;
    zpp->luns_per_ch = 16;
    zpp->nchs = 8;

    zpp->slc_pg_rd_lat = SLC_PAGE_READ_LAT_NS;
    zpp->slc_pg_wr_lat = SLC_PAGE_WRITE_LAT_NS;
    zpp->slc_blk_er_lat = SLC_BLOCK_ERASE_LAT_NS;

    zpp->qlc_pg_rd_lat  = QLC_PAGE_READ_LAT_NS;
    zpp->qlc_pg_wr_lat  = QLC_PAGE_WRITE_LAT_NS;
    zpp->qlc_blk_er_lat  = QLC_BLOCK_ERASE_LAT_NS;

    zpp->slc_blks_pcent = 0;

    /* calculated values */
    zpp->slc_blk_sz = zpp->secsz * zpp->secs_per_pg * zpp->pgs_per_slc_blk / 1048576 * MiB;
    zpp->qlc_blk_sz = zpp->secsz * zpp->secs_per_pg * zpp->pgs_per_qlc_blk / 1048576 * MiB;    
    zpp->slc_spblk_sz = zpp->slc_blk_sz * zpp->luns_per_ch * zpp->nchs;
    zpp->qlc_spblk_sz = zpp->qlc_blk_sz * zpp->luns_per_ch * zpp->nchs;
    
    zpp->zsblks = NVME_DEFAULT_ZONE_SIZE / zpp->slc_blk_sz;
    zpp->zqblks = NVME_DEFAULT_ZONE_SIZE / zpp->qlc_blk_sz;

    printf("slc block sz:%ld, qlc block sz:%ld, zsblks:%d, zqblks:%d \n", 
            zpp->slc_blk_sz, zpp->qlc_blk_sz, zpp->zsblks, zpp->zqblks);

    zpp->secs_per_slc_blk = zpp->secs_per_pg * zpp->pgs_per_slc_blk;
    zpp->secs_per_qlc_blk = zpp->secs_per_pg * zpp->pgs_per_qlc_blk;

    zpp->blks_per_lun = zpp->blks_per_pl * zpp->pls_per_lun;
    zpp->sblks_per_lun = (int)(zpp->slc_blks_pcent * zpp->blks_per_lun);
    zpp->qblks_per_lun = zpp->blks_per_lun - zpp->sblks_per_lun;
    zpp->blks_per_ch = zpp->blks_per_lun * zpp->luns_per_ch;
    zpp->tt_blks = zpp->blks_per_ch * zpp->nchs;
    zpp->tt_slc_blks = zpp->sblks_per_lun * zpp->luns_per_ch * zpp->nchs;
    zpp->tt_qlc_blks = zpp->qblks_per_lun * zpp->luns_per_ch * zpp->nchs;
#ifdef ZNS_DEBUG
    printf("total slc blocks:%d, total qlc blocks:%d, sblks per lun:%d, qblks_per_lun:%d \n", 
            zpp->tt_slc_blks, zpp->tt_qlc_blks, zpp->sblks_per_lun, zpp->qblks_per_lun);
#endif
    zpp->nslc_zone = zpp->tt_slc_blks / zpp->zsblks;
    zpp->nqlc_zone = zpp->tt_qlc_blks / zpp->zqblks;
    printf("nslc_zone: %d, nqlc_zone: %d\n", zpp->nslc_zone, zpp->nqlc_zone);

    uint32_t zone_add = n->num_zones - zpp->nslc_zone - zpp->nqlc_zone;
    zpp->virtual_blks_per_pl = zpp->blks_per_pl + ((double)NVME_DEFAULT_ZONE_SIZE / zpp->qlc_spblk_sz) * zone_add; 

    printf("virtual blks per pl: %d\n", zpp->virtual_blks_per_pl);
}

static void zns_ssd_init_nand_page(struct nand_page *pg, struct znsparams *zpp)
{
    pg->nsecs = zpp->secs_per_pg;
    pg->sec = g_malloc0(sizeof(nand_sec_status_t) * pg->nsecs);
    for (int i = 0; i < pg->nsecs; i++) {
        pg->sec[i] = SEC_FREE;
    }
    pg->status = PG_FREE;
}

static void zns_ssd_init_nand_slc_blk(struct nand_slc_block *blk, struct znsparams *zpp)
{
    blk->npgs = zpp->pgs_per_slc_blk;
    blk->pg = g_malloc0(sizeof(struct nand_page) * blk->npgs);
    for (int i = 0; i < blk->npgs; i++) {
        zns_ssd_init_nand_page(&blk->pg[i], zpp);
    }
    blk->erase_cnt = 0;
    blk->wp = 0;
}

static void zns_ssd_init_nand_qlc_blk(struct nand_qlc_block *blk, struct znsparams *zpp)
{
    blk->npgs = zpp->pgs_per_qlc_blk;
    blk->pg = g_malloc0(sizeof(struct nand_page) * blk->npgs);
    for (int i = 0; i < blk->npgs; i++) {
        zns_ssd_init_nand_page(&blk->pg[i], zpp);
    }
    blk->erase_cnt = 0;
    blk->wp = 0;
}

static void zns_ssd_init_nand_plane(struct zns_nand_plane *pl, struct znsparams *zpp)
{
    pl->nblks = zpp->virtual_blks_per_pl;
    pl->nsblks = (int)(zpp->slc_blks_pcent * zpp->blks_per_pl);
    pl->nqblks = pl->nblks - pl->nsblks; 
    pl->sblk = g_malloc0(sizeof(struct nand_slc_block) * pl->nsblks);
    pl->qblk = g_malloc0(sizeof(struct nand_qlc_block) * pl->nqblks);

    for (int i = 0; i < pl->nsblks; i++) {
        zns_ssd_init_nand_slc_blk(&pl->sblk[i], zpp);
    }

    for (int i = 0; i < pl->nqblks; i++) {
        zns_ssd_init_nand_qlc_blk(&pl->qblk[i], zpp);
    }
}

static void zns_ssd_init_nand_lun(struct zns_nand_lun *lun, struct znsparams *zpp)
{
    lun->npls = zpp->pls_per_lun;
    lun->pl = g_malloc0(sizeof(struct zns_nand_plane) * lun->npls);
    for(int i = 0; i < lun->npls; i++){
        zns_ssd_init_nand_plane(&lun->pl[i], zpp);
    }
    lun->next_lun_avail_time = 0;
    lun->busy = false;
}

static void zns_ssd_init_ch(struct zns_ssd_channel *ch, struct znsparams *zpp)
{
    ch->nluns = zpp->luns_per_ch;
    ch->lun = g_malloc0(sizeof(struct zns_nand_lun) * ch->nluns);
    for(int i = 0; i < ch->nluns; i++){
        zns_ssd_init_nand_lun(&ch->lun[i], zpp);
    }
    ch->next_ch_avail_time = 0;
    ch->busy = 0;
}

static void zns_ssd_init_zone(FemuCtrl *n)
{
    struct ZnsCtrl *znssd = n->zns_ctrl;
    struct znsparams *zpp = &znssd->zp;
    uint64_t start = 0, zone_size = n->zone_size;
    //uint32_t num_qlc_zone = n->num_zones - zpp->nslc_zone;
    
    znssd->zone_array = g_malloc0(sizeof(struct zone) * n->num_zones);
#ifdef ZNS_DEBUG
    printf("Zone Size: %ld \n", zone_size);
#endif
    for(int i = 0; i < n->num_zones; i++){
        znssd->zone_array[i].zt = QLC_ZONE;
        znssd->zone_array[i].nblks = znssd->zp.zqblks;
        znssd->zone_array[i].zcap = n->zone_capacity;
        znssd->zone_array[i].zslba = start;
        znssd->zone_array[i].zwp.i = 0;
        znssd->zone_array[i].zwp.stripe = 0;        
        start += zone_size;
    }

    for(int i = 0; i < zpp->nslc_zone; i++){
        znssd->zone_array[i].zt = SLC_ZONE;
        znssd->zone_array[i].nblks = znssd->zp.zsblks;
    }
}

static struct pba get_new_block(struct ZnsCtrl *znssd)
{
    struct w_ptr *wpp = &znssd->wp;
    struct znsparams *zpp = &znssd->zp;
    struct pba pba;

    pba.ch = wpp->ch;
    pba.lun = wpp->lun;
    pba.blk = wpp->blk;
    pba.pl = wpp->pl;
    
    if(pba.blk < zpp->sblks_per_lun)
    {
        pba.pba = SLC_BLOCK;
        //printf("get slc block.");
    }
    else
    {
        pba.pba = QLC_BLOCK;
    }
    
    ftl_assert(ppa.g.pl == 0);

#ifdef ZNS_DEBUG
    printf("wpp - ch:%d, lun:%d, pl:%d, blk:%d, type=%d \n",
           wpp->ch, wpp->lun, wpp->pl, wpp->blk, pba.pba);
    printf("pba - ch:%d, lun:%d, pl:%d, blk:%d, type=%d \n",
           pba.ch, pba.lun, pba.pl, pba.blk, pba.pba);
#endif
    return pba;
}

static void zns_ssd_init_write_pointer(struct ZnsCtrl *znssd)
{
    struct w_ptr *wpp = &znssd->wp; 

    wpp->ch = 0;
    wpp->lun = 0;
    wpp->blk = 0;
    wpp->pl = 0;
}

static void zns_ssd_advance_write_pointer(struct ZnsCtrl *znssd)
{
    struct znsparams *zpp = &znssd->zp;
    struct w_ptr *wpp = &znssd->wp;
    check_addr(wpp->lun, zpp->luns_per_ch);
    wpp->lun++;
    if(wpp->lun == zpp->luns_per_ch){
        wpp->lun = 0;
        check_addr(wpp->ch, zpp->nchs);
        wpp->ch++;
        if(wpp->ch == zpp->nchs){
            wpp->ch = 0;
            check_addr(wpp->blk, zpp->virtual_blks_per_pl);
            wpp->blk++;
            if(wpp->blk == zpp->virtual_blks_per_pl){
                wpp->blk = 0;
                ftl_assert(wpp->lun);
                ftl_assert(wpp->ch);
                ftl_assert(wpp->blk);
            }
        }
    }
}

static inline void zns_set_maptbl_ent(struct ZnsCtrl *znssd, int i, int j, struct pba *pba)
{
    znssd->zone_array[i].blk_maptbl[j] = *pba;
}

static inline void zns_set_rmap_ent(struct ZnsCtrl *znssd, int i, int j)
{
    znssd->zone_array[i].blk_maptbl[j].rmap = i;
}

static void zns_ssd_init_maptbl(FemuCtrl *n)
{
    struct ZnsCtrl *znssd = n->zns_ctrl;
    struct znsparams *zpp = &znssd->zp;
    struct pba pba;
    uint64_t num = 0;
    int type;

    for(uint32_t i = 0; i < n->num_zones; i++)
    {   
        type = znssd->zone_array[i].zt;
        if (type == SLC_ZONE){
            num = zpp->pgs_per_slc_blk * znssd->zone_array[i].nblks;
        }else if(type == QLC_ZONE){
            num = zpp->pgs_per_qlc_blk * znssd->zone_array[i].nblks;
        }else{
            ftl_err("zone type error!");
        }
        
        znssd->zone_array[i].blk_maptbl = g_malloc0(sizeof(struct pba) * znssd->zone_array[i].nblks);
        znssd->zone_array[i].map = g_malloc0(sizeof(uint64_t) * num);
        for(uint64_t j = 0; j < znssd->zone_array[i].nblks; j++)
        {
            pba = get_new_block(znssd);
            
            if(pba.pba != znssd->zone_array[i].zt){
                //printf("block type: %d, zone type: %ld\n", pba.pba, znssd->zone_array[i].zt);
                ftl_err("block type don't match zone type!");
            }

            zns_set_maptbl_ent(znssd, i, j, &pba);

            zns_set_rmap_ent(znssd, i, j);

            zns_ssd_advance_write_pointer(znssd);
#ifdef ZNS_DEBUG
            printf("maptbl check - ch:%d, lun:%d, pl:%d, blk:%d, type=%d \n",
                   znssd->zone_array[i].blk_maptbl[j].ch, 
                   znssd->zone_array[i].blk_maptbl[j].lun, 
                   znssd->zone_array[i].blk_maptbl[j].pl, 
                   znssd->zone_array[i].blk_maptbl[j].blk, 
                   znssd->zone_array[i].blk_maptbl[j].pba);
#endif
        }
    }
}

void zns_ssd_init(FemuCtrl *n)
{
    struct ZnsCtrl *znssd = n->zns_ctrl;
    struct znsparams *zpp = &znssd->zp;

    ftl_assert(znssd);

    zns_ssd_init_params(n);

    /* initialize write pointer */
    zns_ssd_init_write_pointer(znssd);

    /* initialize ssd internal layout architecture */
    znssd->ch = g_malloc0(sizeof(struct zns_ssd_channel) * zpp->nchs);
    for (int i = 0; i < zpp->nchs; i++) {
        zns_ssd_init_ch(&znssd->ch[i], zpp);
    }

    zns_ssd_init_zone(n);

    /* initialize maptbl */
    zns_ssd_init_maptbl(n);
}

static inline struct zone *zns_ftl_get_zone_by_slba(NvmeNamespace *ns, uint64_t slba)
{
    FemuCtrl *n = ns->ctrl;
    uint32_t zone_idx = zns_ssd_zone_idx(ns, slba);

    assert(zone_idx < n->num_zones);
    return &n->zns_ctrl->zone_array[zone_idx];
}

static uint64_t zns_ssd_advance_status(FemuCtrl *n, struct ZnsCtrl *znssd, struct pba *pba, struct
        nand_cmd *ncmd)
{
    int c = ncmd->cmd;
    uint64_t cmd_stime = (ncmd->stime == 0) ? \
        qemu_clock_get_ns(QEMU_CLOCK_REALTIME) : ncmd->stime;
    uint64_t nand_stime;
    struct znsparams *zpp = &znssd->zp;
    struct zns_nand_lun *lun = get_lun(znssd, pba);
    uint64_t lat = 0, op_lat = 0;

    switch (c) {
    case NAND_READ:
    /* read: perform NAND cmd first */
        op_lat = pba->pba == SLC_BLOCK ? zpp->slc_pg_rd_lat : zpp->qlc_pg_rd_lat;
        nand_stime = (lun->next_lun_avail_time < cmd_stime) ? cmd_stime : \
                     lun->next_lun_avail_time;
        lun->next_lun_avail_time = nand_stime + op_lat;

        lat = lun->next_lun_avail_time - cmd_stime;
    break;

    case NAND_WRITE:
        /* write: transfer data through channel first */
        op_lat = pba->pba == SLC_BLOCK ? zpp->slc_pg_wr_lat : zpp->qlc_pg_wr_lat;
        nand_stime = (lun->next_lun_avail_time < cmd_stime) ? cmd_stime : \
                    lun->next_lun_avail_time;
        lun->next_lun_avail_time = nand_stime + op_lat;

        lat = lun->next_lun_avail_time - cmd_stime;
        break;

    case NAND_ERASE:
        /* erase: only need to advance NAND status */
        op_lat = pba->pba == SLC_BLOCK ? zpp->slc_blk_er_lat : zpp->qlc_blk_er_lat;
        nand_stime = (lun->next_lun_avail_time < cmd_stime) ? cmd_stime : \
                    lun->next_lun_avail_time;
        lun->next_lun_avail_time = nand_stime + op_lat;

        lat = lun->next_lun_avail_time - cmd_stime;
        break;

    default:
        ftl_err("Unsupported NAND command: 0x%x\n", c);
    }

    return lat;
}

static void advance_zone_writer_pointer(struct znsparams *zpp, struct zone *zone)
{
    uint32_t bndry = 0;

    if(zone->zt == SLC_ZONE){
        bndry = zpp->pgs_per_slc_blk;
    }else if(zone->zt == QLC_ZONE){
        bndry = zpp->pgs_per_qlc_blk;
    }

    check_addr(zone->zwp.i, zone->nblks);
    zone->zwp.i++;
    if(zone->zwp.i == zone->nblks)
    {
        zone->zwp.i = 0;
        check_addr(zone->zwp.stripe, bndry);
        zone->zwp.stripe++;
        if(zone->zwp.stripe == bndry)
        {
            ftl_assert(zone->zwp.i);
        }
    }
}

static void initialize_zone_writer_pointer(struct znsparams *zpp, struct zone *zone)
{
    zone->zwp.i = 0;
    zone->zwp.stripe = 0;
}

static struct pba *zns_get_maptbl_ent(struct ZnsCtrl *znssd, uint32_t zone_idx, uint64_t slba, bool is_write)
{
    struct zone *zone;
    struct znsparams *zpp = &znssd->zp;
    struct zone_wp *wp;
    uint32_t idx, offset;
   
    zone = &znssd->zone_array[zone_idx];
    wp = &zone->zwp;

    offset = (slba - zone->zslba) / zpp->secs_per_pg; 

    if(is_write){
        idx = wp->i;
        zone->map[offset] = idx;
        advance_zone_writer_pointer(zpp, zone);
    }else{
        idx = zone->map[offset];
    }

    return &zone->blk_maptbl[idx];
}

static void zns_ssd_read(FemuCtrl *n, NvmeRequest *req, uint64_t slba, uint32_t nlb)
{
    NvmeNamespace *ns = (NvmeNamespace *)&n->namespaces[0];
    struct ZnsCtrl *znssd = n->zns_ctrl;
    struct znsparams *zpp = &znssd->zp;
    struct pba* pba;
    uint32_t zone_idx;
    uint64_t start_lpn = slba / zpp->secs_per_pg;
    uint64_t end_lpn = (slba + nlb - 1) / zpp->secs_per_pg;
    uint64_t sublat, maxlat = 0;

    /* normal IO read path */
    for (uint64_t lpn = start_lpn; lpn <= end_lpn; lpn++)
    {
        /* read may be crossing zones */ 
        zone_idx = zns_ssd_zone_idx(ns, slba);

        pba = zns_get_maptbl_ent(znssd, zone_idx, slba, false);
#ifdef ZNS_DEBUG
        printf("read start slba:%ld, nlb:%d \n", slba, nlb);
        printf("Read Lpn - zone:%d, ch:%d, lun:%d, pl:%d, lpn:%ld/%ld, type=%ld \n",
                zone_idx, zpba->ch, zpba->lun, zpba->pl, lpn, end_lpn, zpba->pba);
#endif
        struct nand_cmd srd;
        srd.type = USER_IO;
        srd.cmd = NAND_READ;
        srd.stime = req->stime;
        sublat = zns_ssd_advance_status(n, znssd, pba, &srd);
        maxlat = (sublat > maxlat) ? sublat : maxlat;
    }
    req->reqlat = maxlat;
    req->expire_time += maxlat;
#ifdef ZNS_DEBUG
    printf("Read Request - arrival time:%ld, latency:%ld, expire time:%ld \n", 
            req->stime, req->reqlat, req->expire_time);
#endif
}

static void zns_ssd_write(FemuCtrl *n, NvmeRequest *req, uint64_t slba, uint32_t nlb)
{
    NvmeNamespace *ns = (NvmeNamespace *)&n->namespaces[0];
    struct ZnsCtrl *znssd = n->zns_ctrl;
    struct znsparams *zpp = &znssd->zp;
    struct pba *pba;
    uint32_t zone_idx;
    uint64_t curlat = 0, maxlat = 0;
    uint64_t start_lpn = slba / zpp->secs_per_pg;
    uint64_t end_lpn = (slba + nlb - 1) / zpp->secs_per_pg;

#ifdef ZNS_DEBUG
    printf("write start slba:%ld, nlb:%d \n", slba, nlb);
    printf("start lpn:%ld, end_lpn:%ld \n", start_lpn, end_lpn);
#endif
    zone_idx = zns_ssd_zone_idx(ns, slba);

    /* normal IO write path */
    for (uint64_t lpn = start_lpn; lpn <= end_lpn; lpn++)
    {
        pba = zns_get_maptbl_ent(znssd, zone_idx, slba, true);
#ifdef ZNS_DEBUG
        printf("Write Lpn - zone:%d, ch:%d, lun:%d, pl:%d, lpn:%ld/%ld, type=%d \n",
                zone_idx, pba->ch, pba->lun, pba->pl, lpn, end_lpn, pba->pba);
        printf("start lpn:%ld, end_lpn:%ld \n", start_lpn, end_lpn);
#endif
        struct nand_cmd srd;
        srd.type = USER_IO;
        srd.cmd = NAND_WRITE;
        srd.stime = req->stime;
        curlat = zns_ssd_advance_status(n, znssd, pba, &srd);
        maxlat = (curlat > maxlat) ? curlat : maxlat;
    }

    req->reqlat = maxlat;
    req->expire_time += maxlat;
#ifdef ZNS_DEBUG
    printf("Write Request - arrival time:%ld, latency:%ld, expire time:%ld \n", 
            req->stime, req->reqlat, req->expire_time);
#endif
}

static inline struct nand_slc_block *get_slc_blk(struct ZnsCtrl *znssd, struct pba *pba)
{
    struct zns_nand_plane *pl = get_pl(znssd, pba);
    return &(pl->sblk[pba->blk]);
}

static inline struct nand_qlc_block *get_qlc_blk(struct ZnsCtrl *znssd, struct pba *pba)
{
    struct zns_nand_plane *pl = get_pl(znssd, pba);
    return &(pl->qblk[pba->blk]);
}

static void zns_mark_block_free(struct ZnsCtrl *znssd, struct pba *pba)
{
    int c = pba->pba;
    struct nand_slc_block *sblk;
    struct nand_qlc_block *qblk;

    switch (c)
    {
    case SLC_BLOCK:
        sblk = get_slc_blk(znssd, pba); 
        sblk->erase_cnt++;
        break;
    case QLC_BLOCK:
        qblk = get_qlc_blk(znssd, pba); 
        qblk->erase_cnt++;
        break;
    default:
        ftl_err("FTL received unknown block type, ERROR\n");
    }
}

static uint16_t zns_ssd_get_mgmt_zone_slba_idx(FemuCtrl *n, NvmeCmd *c,
                                           uint64_t *slba, uint32_t *zone_idx)
{
    NvmeNamespace *ns = &n->namespaces[0];
    uint32_t dw10 = le32_to_cpu(c->cdw10);
    uint32_t dw11 = le32_to_cpu(c->cdw11);

    if (!n->zoned) {
        return NVME_INVALID_OPCODE | NVME_DNR;
    }

    *slba = ((uint64_t)dw11) << 32 | dw10;
    if (unlikely(*slba >= ns->id_ns.nsze)) {
        *slba = 0;
        return NVME_LBA_RANGE | NVME_DNR;
    }

    *zone_idx = zns_ssd_zone_idx(ns, *slba);
    assert(*zone_idx < n->num_zones);

    return NVME_SUCCESS;
}

static void zns_ssd_reset_zone(FemuCtrl *n, NvmeRequest *req)
{
    NvmeCmd *cmd = (NvmeCmd *)&req->cmd;
    struct ZnsCtrl *znssd = n->zns_ctrl;
    struct zone *zone;
    uint64_t slba;
    uint32_t zone_idx = 0;
    uint64_t curlat = 0, maxlat = 0;
    struct pba *pba;

    zns_ssd_get_mgmt_zone_slba_idx(n, cmd, &slba, &zone_idx);   

    zone = &znssd->zone_array[zone_idx];

    int nums = zone->nblks;
    for (size_t i = 0; i < nums; i++)
    {
        pba = &zone->blk_maptbl[i];
#ifdef ZNS_DEBUG
        printf("Erase Block - zone:%d, ch:%d, lun:%d, pl:%d, blk:%d/%ld, type=%d \n",
                zone_idx, pba->ch, pba->lun, pba->pl, pba->blk, zone->nblks, pba->pba);
#endif
        zns_mark_block_free(znssd, pba);
        struct nand_cmd srd;
        srd.type = USER_IO;
        srd.cmd = NAND_ERASE;
        srd.stime = req->stime;
        curlat = zns_ssd_advance_status(n, znssd, pba, &srd);
        maxlat = (curlat > maxlat) ? curlat : maxlat;
    }

    initialize_zone_writer_pointer(&znssd->zp, zone);

    req->reqlat = maxlat;
    req->expire_time += maxlat;
#ifdef ZNS_DEBUG
    printf("Reset Request - arrival time:%ld, latency:%ld, expire time:%ld \n", 
            req->stime, req->reqlat, req->expire_time);
#endif
}

/**
 * Authors: Bingjie Xu (xbj1997@foxmail.com)
 * Date: 2021/11/30
 */