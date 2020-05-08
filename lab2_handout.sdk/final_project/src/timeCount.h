#include "xtmrctr.h"
#include "xintc.h"
#include "xparameters.h"
#include "xtmrctr_l.h"
#include "xintc_l.h"
#include "mb_interface.h"
#include <xbasic_types.h>
#include <xio.h>




//#define GPIO_DEVICE_ID_ROTARY          XPAR_XPS_GPIO_1_DEVICE_ID

#define INTC_DEVICE_ID                 XPAR_INTC_0_DEVICE_ID


#define RESET_VALUE                   0xC350//0xBEBC //0xF4240 //0x10000 //0x017D7840

//#define TMRCTR_INTERRUPT_ID         XPAR_INTC_0_TMRCTR_0_VEC_ID


void timeCount_handler();
void timeCount_disable();
void timeCount_enable();
int timeCount_initial();
