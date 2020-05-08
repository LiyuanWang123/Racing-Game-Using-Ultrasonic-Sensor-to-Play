#include "timeCount.h"
#include "xgpio.h"

extern XIntc sys_intc;
XTmrCtr sys_tmrctr;
XTmrCtr sys_tmr;
XTmrCtr sys_tmr2;
XTmrCtr sys_tmr3;
Xuint32 data;
static XGpio Gpio;
unsigned int count = 0;
unsigned int ssrcount = 0;

void timeCount_handler()
{
	// This is the interrupt handler function
	// Do not print inside of this function.
	Xuint32 ControlStatusReg;
	/*
	 * Read the new Control/Status Register content.
	 */
	ControlStatusReg = XTimerCtr_ReadReg(sys_tmrctr.BaseAddress, 0, XTC_TCSR_OFFSET);
	//XGpio_Initialize(&Gpio, XPAR_AXI_GPIO_1_DEVICE_ID);
	//xil_printf("Timer interrupt occurred. Count= %d\r\n", count);
	//XGpio_DiscreteWrite(&Gpio,1,count/1000);
	count++;
	ssrcount++;// increment count
	/*
	 * Acknowledge the interrupt by clearing the interrupt
	 * bit in the timer control status register
	 */
	XTmrCtr_WriteReg(sys_tmrctr.BaseAddress, 0, XTC_TCSR_OFFSET, ControlStatusReg |XTC_CSR_INT_OCCURED_MASK);

}

void timeCount2_handler(){}

void timeCount_disable()
{
	XIntc_Disable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_0_INTERRUPT_INTR);
}

void timeCount_enable()
{
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_0_INTERRUPT_INTR);
}

int timeCount_initial()
{
	//xil_printf("I'm in the main() method\r\n");

	XStatus Status;
	/*
	 * Initialize the interrupt controller driver so that
	 * it is ready to use, specify the device ID that is generated in
	 * xparameters.h
	 */
	Status = XST_SUCCESS;

	/*
	 * Connect the application handler that will be called when an interrupt
	 * for the timer occurs
	 */
	Status = XIntc_Connect(&sys_intc,XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_0_INTERRUPT_INTR,
			(XInterruptHandler)timeCount_handler, &sys_tmrctr);

	// XIntc_Connect(&sys_intc,XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_1_INTERRUPT_INTR,
	//			(XInterruptHandler)timeCount2_handler, &sys_tmr2);

	if ( Status != XST_SUCCESS )
	{
		xil_printf("Failed to connect the application handlers to the interrupt controller...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Connected to Interrupt Controller!\r\n");


	XTmrCtr_Initialize(&sys_tmrctr, XPAR_TMRCTR_0_DEVICE_ID);
	XTmrCtr_Initialize(&sys_tmr, XPAR_TMRCTR_0_DEVICE_ID);
	Status = XTmrCtr_Initialize(&sys_tmr2, XPAR_TMRCTR_1_DEVICE_ID);
	XTmrCtr_Initialize(&sys_tmr3, XPAR_TMRCTR_1_DEVICE_ID);
	if ( Status != XST_SUCCESS )
	{
		xil_printf("Timer initialization failed...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Initialized Timer!\r\n");
	/*
	 * Enable the interrupt of the timer counter so interrupts will occur
	 * and use auto reload mode such that the timer counter will reload
	 * itself automatically and continue repeatedly, without this option
	 * it would expire once only
	 */
	XTmrCtr_SetOptions(&sys_tmrctr, 0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
	XTmrCtr_SetOptions(&sys_tmr, 1, XTC_AUTO_RELOAD_OPTION);
	XTmrCtr_SetOptions(&sys_tmr2, 0, XTC_AUTO_RELOAD_OPTION);
	XTmrCtr_SetOptions(&sys_tmr3, 1, XTC_AUTO_RELOAD_OPTION);
	/*
	 * Set a reset value for the timer counter such that it will expire
	 * eariler than letting it roll over from 0, the reset value is loaded
	 * into the timer counter when it is started
	 */

	XTmrCtr_SetResetValue(&sys_tmrctr, 0, 0xFFFFFFFF-RESET_VALUE);		// 0x17D7840 = 25*10^6 clk cycles @ 50MHz = 500ms
	XTmrCtr_SetResetValue(&sys_tmr, 1, 0);
	XTmrCtr_SetResetValue(&sys_tmr2, 0, 0);
	XTmrCtr_SetResetValue(&sys_tmr3, 1, 0);
	/*
	 * Start the timer counter such that it's incrementing by default,
	 * then wait for it to timeout a number of times
	 */
	XTmrCtr_Start(&sys_tmrctr, 0);
	/*
	 * Register the intc device driver’s handler with the Standalone
	 * software platform’s interrupt table
	 */
	microblaze_register_handler((XInterruptHandler)XIntc_DeviceInterruptHandler,
			(void*)XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);
//	microblaze_register_handler((XInterruptHandler)XIntc_DeviceInterruptHandler,
//			(void*)PUSHBUTTON_DEVICE_ID);
	//xil_printf("Registers handled!\r\n");

	/*
	 * Enable interrupts on MicroBlaze
	 */
	microblaze_enable_interrupts();
	xil_printf("time Count Interrupts enabled!\r\n");
	/*
	 * At this point, the system is ready to respond to interrupts from the timer
	 */

	return XST_SUCCESS;
}
