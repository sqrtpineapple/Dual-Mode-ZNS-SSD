/* This file is autogenerated by tracetool, do not edit. */

#include "qemu/osdep.h"
#include "qemu/module.h"
#include "trace-hw_core.h"

uint16_t _TRACE_LOADER_WRITE_ROM_DSTATE;
uint16_t _TRACE_QDEV_RESET_DSTATE;
uint16_t _TRACE_QDEV_RESET_ALL_DSTATE;
uint16_t _TRACE_QDEV_RESET_TREE_DSTATE;
uint16_t _TRACE_QBUS_RESET_DSTATE;
uint16_t _TRACE_QBUS_RESET_ALL_DSTATE;
uint16_t _TRACE_QBUS_RESET_TREE_DSTATE;
uint16_t _TRACE_QDEV_UPDATE_PARENT_BUS_DSTATE;
uint16_t _TRACE_RESETTABLE_RESET_DSTATE;
uint16_t _TRACE_RESETTABLE_RESET_ASSERT_BEGIN_DSTATE;
uint16_t _TRACE_RESETTABLE_RESET_ASSERT_END_DSTATE;
uint16_t _TRACE_RESETTABLE_RESET_RELEASE_BEGIN_DSTATE;
uint16_t _TRACE_RESETTABLE_RESET_RELEASE_END_DSTATE;
uint16_t _TRACE_RESETTABLE_CHANGE_PARENT_DSTATE;
uint16_t _TRACE_RESETTABLE_PHASE_ENTER_BEGIN_DSTATE;
uint16_t _TRACE_RESETTABLE_PHASE_ENTER_EXEC_DSTATE;
uint16_t _TRACE_RESETTABLE_PHASE_ENTER_END_DSTATE;
uint16_t _TRACE_RESETTABLE_PHASE_HOLD_BEGIN_DSTATE;
uint16_t _TRACE_RESETTABLE_PHASE_HOLD_EXEC_DSTATE;
uint16_t _TRACE_RESETTABLE_PHASE_HOLD_END_DSTATE;
uint16_t _TRACE_RESETTABLE_PHASE_EXIT_BEGIN_DSTATE;
uint16_t _TRACE_RESETTABLE_PHASE_EXIT_EXEC_DSTATE;
uint16_t _TRACE_RESETTABLE_PHASE_EXIT_END_DSTATE;
uint16_t _TRACE_RESETTABLE_TRANSITIONAL_FUNCTION_DSTATE;
uint16_t _TRACE_CLOCK_SET_SOURCE_DSTATE;
uint16_t _TRACE_CLOCK_DISCONNECT_DSTATE;
uint16_t _TRACE_CLOCK_SET_DSTATE;
uint16_t _TRACE_CLOCK_PROPAGATE_DSTATE;
uint16_t _TRACE_CLOCK_UPDATE_DSTATE;
TraceEvent _TRACE_LOADER_WRITE_ROM_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "loader_write_rom",
    .sstate = TRACE_LOADER_WRITE_ROM_ENABLED,
    .dstate = &_TRACE_LOADER_WRITE_ROM_DSTATE 
};
TraceEvent _TRACE_QDEV_RESET_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "qdev_reset",
    .sstate = TRACE_QDEV_RESET_ENABLED,
    .dstate = &_TRACE_QDEV_RESET_DSTATE 
};
TraceEvent _TRACE_QDEV_RESET_ALL_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "qdev_reset_all",
    .sstate = TRACE_QDEV_RESET_ALL_ENABLED,
    .dstate = &_TRACE_QDEV_RESET_ALL_DSTATE 
};
TraceEvent _TRACE_QDEV_RESET_TREE_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "qdev_reset_tree",
    .sstate = TRACE_QDEV_RESET_TREE_ENABLED,
    .dstate = &_TRACE_QDEV_RESET_TREE_DSTATE 
};
TraceEvent _TRACE_QBUS_RESET_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "qbus_reset",
    .sstate = TRACE_QBUS_RESET_ENABLED,
    .dstate = &_TRACE_QBUS_RESET_DSTATE 
};
TraceEvent _TRACE_QBUS_RESET_ALL_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "qbus_reset_all",
    .sstate = TRACE_QBUS_RESET_ALL_ENABLED,
    .dstate = &_TRACE_QBUS_RESET_ALL_DSTATE 
};
TraceEvent _TRACE_QBUS_RESET_TREE_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "qbus_reset_tree",
    .sstate = TRACE_QBUS_RESET_TREE_ENABLED,
    .dstate = &_TRACE_QBUS_RESET_TREE_DSTATE 
};
TraceEvent _TRACE_QDEV_UPDATE_PARENT_BUS_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "qdev_update_parent_bus",
    .sstate = TRACE_QDEV_UPDATE_PARENT_BUS_ENABLED,
    .dstate = &_TRACE_QDEV_UPDATE_PARENT_BUS_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_RESET_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_reset",
    .sstate = TRACE_RESETTABLE_RESET_ENABLED,
    .dstate = &_TRACE_RESETTABLE_RESET_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_RESET_ASSERT_BEGIN_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_reset_assert_begin",
    .sstate = TRACE_RESETTABLE_RESET_ASSERT_BEGIN_ENABLED,
    .dstate = &_TRACE_RESETTABLE_RESET_ASSERT_BEGIN_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_RESET_ASSERT_END_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_reset_assert_end",
    .sstate = TRACE_RESETTABLE_RESET_ASSERT_END_ENABLED,
    .dstate = &_TRACE_RESETTABLE_RESET_ASSERT_END_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_RESET_RELEASE_BEGIN_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_reset_release_begin",
    .sstate = TRACE_RESETTABLE_RESET_RELEASE_BEGIN_ENABLED,
    .dstate = &_TRACE_RESETTABLE_RESET_RELEASE_BEGIN_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_RESET_RELEASE_END_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_reset_release_end",
    .sstate = TRACE_RESETTABLE_RESET_RELEASE_END_ENABLED,
    .dstate = &_TRACE_RESETTABLE_RESET_RELEASE_END_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_CHANGE_PARENT_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_change_parent",
    .sstate = TRACE_RESETTABLE_CHANGE_PARENT_ENABLED,
    .dstate = &_TRACE_RESETTABLE_CHANGE_PARENT_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_PHASE_ENTER_BEGIN_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_phase_enter_begin",
    .sstate = TRACE_RESETTABLE_PHASE_ENTER_BEGIN_ENABLED,
    .dstate = &_TRACE_RESETTABLE_PHASE_ENTER_BEGIN_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_PHASE_ENTER_EXEC_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_phase_enter_exec",
    .sstate = TRACE_RESETTABLE_PHASE_ENTER_EXEC_ENABLED,
    .dstate = &_TRACE_RESETTABLE_PHASE_ENTER_EXEC_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_PHASE_ENTER_END_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_phase_enter_end",
    .sstate = TRACE_RESETTABLE_PHASE_ENTER_END_ENABLED,
    .dstate = &_TRACE_RESETTABLE_PHASE_ENTER_END_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_PHASE_HOLD_BEGIN_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_phase_hold_begin",
    .sstate = TRACE_RESETTABLE_PHASE_HOLD_BEGIN_ENABLED,
    .dstate = &_TRACE_RESETTABLE_PHASE_HOLD_BEGIN_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_PHASE_HOLD_EXEC_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_phase_hold_exec",
    .sstate = TRACE_RESETTABLE_PHASE_HOLD_EXEC_ENABLED,
    .dstate = &_TRACE_RESETTABLE_PHASE_HOLD_EXEC_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_PHASE_HOLD_END_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_phase_hold_end",
    .sstate = TRACE_RESETTABLE_PHASE_HOLD_END_ENABLED,
    .dstate = &_TRACE_RESETTABLE_PHASE_HOLD_END_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_PHASE_EXIT_BEGIN_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_phase_exit_begin",
    .sstate = TRACE_RESETTABLE_PHASE_EXIT_BEGIN_ENABLED,
    .dstate = &_TRACE_RESETTABLE_PHASE_EXIT_BEGIN_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_PHASE_EXIT_EXEC_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_phase_exit_exec",
    .sstate = TRACE_RESETTABLE_PHASE_EXIT_EXEC_ENABLED,
    .dstate = &_TRACE_RESETTABLE_PHASE_EXIT_EXEC_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_PHASE_EXIT_END_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_phase_exit_end",
    .sstate = TRACE_RESETTABLE_PHASE_EXIT_END_ENABLED,
    .dstate = &_TRACE_RESETTABLE_PHASE_EXIT_END_DSTATE 
};
TraceEvent _TRACE_RESETTABLE_TRANSITIONAL_FUNCTION_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "resettable_transitional_function",
    .sstate = TRACE_RESETTABLE_TRANSITIONAL_FUNCTION_ENABLED,
    .dstate = &_TRACE_RESETTABLE_TRANSITIONAL_FUNCTION_DSTATE 
};
TraceEvent _TRACE_CLOCK_SET_SOURCE_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "clock_set_source",
    .sstate = TRACE_CLOCK_SET_SOURCE_ENABLED,
    .dstate = &_TRACE_CLOCK_SET_SOURCE_DSTATE 
};
TraceEvent _TRACE_CLOCK_DISCONNECT_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "clock_disconnect",
    .sstate = TRACE_CLOCK_DISCONNECT_ENABLED,
    .dstate = &_TRACE_CLOCK_DISCONNECT_DSTATE 
};
TraceEvent _TRACE_CLOCK_SET_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "clock_set",
    .sstate = TRACE_CLOCK_SET_ENABLED,
    .dstate = &_TRACE_CLOCK_SET_DSTATE 
};
TraceEvent _TRACE_CLOCK_PROPAGATE_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "clock_propagate",
    .sstate = TRACE_CLOCK_PROPAGATE_ENABLED,
    .dstate = &_TRACE_CLOCK_PROPAGATE_DSTATE 
};
TraceEvent _TRACE_CLOCK_UPDATE_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "clock_update",
    .sstate = TRACE_CLOCK_UPDATE_ENABLED,
    .dstate = &_TRACE_CLOCK_UPDATE_DSTATE 
};
TraceEvent *hw_core_trace_events[] = {
    &_TRACE_LOADER_WRITE_ROM_EVENT,
    &_TRACE_QDEV_RESET_EVENT,
    &_TRACE_QDEV_RESET_ALL_EVENT,
    &_TRACE_QDEV_RESET_TREE_EVENT,
    &_TRACE_QBUS_RESET_EVENT,
    &_TRACE_QBUS_RESET_ALL_EVENT,
    &_TRACE_QBUS_RESET_TREE_EVENT,
    &_TRACE_QDEV_UPDATE_PARENT_BUS_EVENT,
    &_TRACE_RESETTABLE_RESET_EVENT,
    &_TRACE_RESETTABLE_RESET_ASSERT_BEGIN_EVENT,
    &_TRACE_RESETTABLE_RESET_ASSERT_END_EVENT,
    &_TRACE_RESETTABLE_RESET_RELEASE_BEGIN_EVENT,
    &_TRACE_RESETTABLE_RESET_RELEASE_END_EVENT,
    &_TRACE_RESETTABLE_CHANGE_PARENT_EVENT,
    &_TRACE_RESETTABLE_PHASE_ENTER_BEGIN_EVENT,
    &_TRACE_RESETTABLE_PHASE_ENTER_EXEC_EVENT,
    &_TRACE_RESETTABLE_PHASE_ENTER_END_EVENT,
    &_TRACE_RESETTABLE_PHASE_HOLD_BEGIN_EVENT,
    &_TRACE_RESETTABLE_PHASE_HOLD_EXEC_EVENT,
    &_TRACE_RESETTABLE_PHASE_HOLD_END_EVENT,
    &_TRACE_RESETTABLE_PHASE_EXIT_BEGIN_EVENT,
    &_TRACE_RESETTABLE_PHASE_EXIT_EXEC_EVENT,
    &_TRACE_RESETTABLE_PHASE_EXIT_END_EVENT,
    &_TRACE_RESETTABLE_TRANSITIONAL_FUNCTION_EVENT,
    &_TRACE_CLOCK_SET_SOURCE_EVENT,
    &_TRACE_CLOCK_DISCONNECT_EVENT,
    &_TRACE_CLOCK_SET_EVENT,
    &_TRACE_CLOCK_PROPAGATE_EVENT,
    &_TRACE_CLOCK_UPDATE_EVENT,
  NULL,
};

static void trace_hw_core_register_events(void)
{
    trace_event_register_group(hw_core_trace_events);
}
trace_init(trace_hw_core_register_events)
