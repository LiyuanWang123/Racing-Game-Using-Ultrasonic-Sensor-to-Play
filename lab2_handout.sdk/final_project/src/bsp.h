/*
 * twist.h
 *
 *  Created on: Oct 25, 2017
 *      Author: liyuan
 */
#include "xtmrctr.h"
#include "xintc.h"
#include "xparameters.h"
#include "xtmrctr_l.h"
#include "xintc_l.h"
#include "mb_interface.h"
#include <xbasic_types.h>
#include <xio.h>

#ifndef bsp_h
#define bsp_h

#define push_baseAddr XPAR_GPIO_2_BASEADDR
#define push_device_ID XPAR_GPIO_2_DEVICE_ID
#define push_MASK XPAR_PUSH_IP2INTC_IRPT_MASK
#define push_INTERRUPT_ID XPAR_MICROBLAZE_0_AXI_INTC_PUSH_IP2INTC_IRPT_INTR

#define twist_device_ID XPAR_GPIO_6_DEVICE_ID
#define twist_baseaddr XPAR_GPIO_6_BASEADDR
#define twist_MASK XPAR_TWIST_IP2INTC_IRPT_MASK
#define twist_INTERRUPT_ID XPAR_MICROBLAZE_0_AXI_INTC_TWIST_IP2INTC_IRPT_INTR

#define btn_baseAddr XPAR_GPIO_0_BASEADDR
#define btn_device_ID XPAR_GPIO_0_DEVICE_ID
#define btn_MASK XPAR_AXI_GPIO_0_IP2INTC_IRPT_MASK
#define btn_INTERRUPT_ID XPAR_MICROBLAZE_0_AXI_INTC_AXI_GPIO_0_IP2INTC_IRPT_INTR

#define ssr_baseAddr XPAR_GPIO_3_BASEADDR
#define ssr_device_ID XPAR_GPIO_3_DEVICE_ID
#define ssr_MASK XPAR_SENSOR_IP2INTC_IRPT_MASK
#define ssr_INTERRUPT_ID XPAR_MICROBLAZE_0_AXI_INTC_SENSOR_IP2INTC_IRPT_INTR

#define ssr2_baseAddr XPAR_GPIO_4_BASEADDR
#define ssr2_device_ID XPAR_GPIO_4_DEVICE_ID
#define ssr2_MASK XPAR_SENSOR2_IP2INTC_IRPT_MASK
#define ssr2_INTERRUPT_ID XPAR_MICROBLAZE_0_AXI_INTC_SENSOR2_IP2INTC_IRPT_INTR

#define INTC_DEVICE_ID                 XPAR_INTC_0_DEVICE_ID


void twist_handler(void* CallbackRef);
void twist_initial ();

void BSP_init(void);
void ISR_gpio(void);
void ISR_timer(void);
void QF_onStartup(void);
#define BSP_showState(prio_, state_) ((void)0)


#endif
