#ifndef __ZNS_FTL_H
#define __ZNS_FTL_H

#include "../bbssd/ftl.h"
void zns_ssd_init(FemuCtrl *n);

/**
 * @brief SLC NAND latency numbers in naoseconds
 */
#define SLC_PAGE_READ_LAT_NS          (30000)
#define SLC_PAGE_WRITE_LAT_NS         (160000)
#define SLC_BLOCK_ERASE_LAT_NS        (3000000)

/**
 * @brief QLC NAND latency numbers in naoseconds
 */
#define QLC_PAGE_READ_LAT_NS          (140000)
#define QLC_PAGE_WRITE_LAT_NS         (3102000)
#define QLC_BLOCK_ERASE_LAT_NS        (3500000)

/**
 * @brief Nand flash block and zone types
 * 
 * @SLC/MLC/TLC/QLC_BLOCK: Physical blocks consist of SLC/MLC/TLC/QLC pages.
 * @SLC/MLC/TLC/QLC_ZONE: Zones consist of SLC/MLC/TLC/QLC blocks.
 */
enum{
    SLC_BLOCK = 0,
    MLC_BLOCK = 1,
    TLC_BLOCK = 2,
    QLC_BLOCK = 3
};

enum{
    SLC_ZONE = 0,
    MLC_ZONE = 1,
    TLC_ZONE = 2,
    QLC_ZONE = 3
};

/**
 * @brief Zone write pointer
 * 
 * Write pointer to a zone.
 */
struct zone_wp
{
    uint32_t i;             /* Nand flash block offset in zone */
    uint32_t stripe;        /* Nand flash page offset in block */
};

/**
 * @brief Zone descriptor data structure
 *
 * Provide information on a zone with nand flash features.
 */
struct zone
{
    uint64_t zt;            /* Zone type */
    uint64_t zcap;          /* Zone capacity */
    uint64_t zslba;         /* Zone start logical block address */
    uint64_t nblks;         /* # of blocks per zone */
    uint64_t *map;          /* Lpn in zone offset <-> chucks mapping table */

    struct pba *blk_maptbl;    /* Block-level mapping table */
    struct zone_wp zwp;        /* Zone write pointer */
};

struct nand_slc_block {
    struct nand_page *pg;
    int npgs;
    int erase_cnt;
    int wp; /* current write pointer */
};

struct nand_qlc_block {
    struct nand_page *pg;
    int npgs;
    int erase_cnt;
    int wp; /* current write pointer */
};

struct zns_nand_plane {
    struct nand_slc_block *sblk;
    struct nand_qlc_block *qblk;
    int nblks;
    int nsblks;
    int nqblks;
};

struct zns_nand_lun {
    struct zns_nand_plane *pl;
    int npls;
    uint64_t next_lun_avail_time;
    bool busy;
    uint64_t gc_endtime;
};

struct zns_ssd_channel
{
    struct zns_nand_lun *lun;
    int nluns;
    uint64_t next_ch_avail_time;
    bool busy;
    uint64_t gc_endtime;
};

struct znsparams {
    /* configurable device characteristics */
    uint64_t secsz;                /* sector size in bytes */
    uint64_t secs_per_pg;           /* # of sectors per page */
    uint32_t pgs_per_slc_blk;      /* # of pages per SLC block */
    uint32_t pgs_per_qlc_blk;      /* # of pages per QLC block */
    uint32_t blks_per_pl;          /* # of blocks per plane */
    uint32_t virtual_blks_per_pl;
    uint8_t pls_per_lun;           /* # of planes per LUN (Die) */
    uint8_t luns_per_ch;           /* # of LUNs per channel */
    uint8_t nchs;                  /* # of channels in the SSD */

    uint32_t slc_pg_rd_lat;    /* NAND page read latency in nanoseconds */
    uint32_t slc_pg_wr_lat;    /* NAND page program latency in nanoseconds */
    uint32_t slc_blk_er_lat;   /* NAND block erase latency in nanoseconds */
    
    uint32_t qlc_pg_rd_lat;
    uint32_t qlc_pg_wr_lat;
    uint32_t qlc_blk_er_lat;

    uint32_t ch_xfer_lat;  /* channel transfer latency for one page in nanoseconds
                            * this defines the channel bandwith
                            */

    double slc_blks_pcent; /* # percent of SLC blocks per LUN */

    /* calculated values */
    uint64_t slc_blk_sz;
    uint64_t qlc_blk_sz;
    uint64_t slc_spblk_sz;
    uint64_t qlc_spblk_sz;
    uint32_t zsblks;       /* # of SLC block numbers per zone */ 
    uint32_t zqblks;       /* # of QLC block numbers per zone */

    int secs_per_slc_blk;  /* # of sectors per block */
    int secs_per_qlc_blk;
    int blks_per_lun;      /* # of blocks per LUN */
    int sblks_per_lun;     /* # of SLC blocks per LUN */
    int qblks_per_lun;     /* # of QLC blocks per LUN */
    
    int blks_per_ch;       /* # of blocks per channel */
    int tt_blks;      /* total # of blocks in the SSD */
    int tt_slc_blks;  /* total # of SLC blocks in the SSD */
    int tt_qlc_blks;  /* total # of QLC blocks in the SSD */

    int nslc_zone;    /* SLC zone numbers */
    int nqlc_zone;    /* QLC zone numbers */
};

struct pba{
    uint32_t blk;
    uint8_t pl;
    uint8_t lun;
    uint8_t ch;
    uint32_t pba;          /* block type */
    uint32_t rmap;         /* reverse mapptbl, assume it's stored in OOB */
};

struct w_ptr {
    int ch;
    int lun;
    int pl;
    int blk;
};

typedef struct ZnsCtrl {
    struct znsparams zp;
    struct zns_ssd_channel *ch;
    struct zone *zone_array;
    struct w_ptr wp;
} ZnsCtrl;

#endif

/**
 * Authors: Bingjie Xu (xbj1997@foxmail.com)
 * Date: 2021/11/30
 */