#include "fault_handler.h"

void
mem_manage_handler(void)
{
    REPORT_STACK_FRAME
    ReportMemanageFault();
}

void
hard_fault_handler(void)
{
    REPORT_STACK_FRAME
    ReportHardFault();
}

void
bus_fault_handler(void)
{
    REPORT_STACK_FRAME
    ReportBusFault();
}

void
usage_fault_handler(void)
{
    REPORT_STACK_FRAME
    ReportUsageFault();
}
