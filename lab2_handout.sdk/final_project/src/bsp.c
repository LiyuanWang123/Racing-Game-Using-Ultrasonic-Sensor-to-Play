/*****************************************************************************
* bsp.c for Lab2A of ECE 153a at UCSB
* Date of the Last Update:  October 23,2014
*****************************************************************************/

/**/
#include "qpn_port.h"
#include "bsp.h"
#include "lab2a.h"
#include "xintc.h"
#include "xil_exception.h"
#include "xspi.h"
#include "xspi_l.h"
#include "lcd.h"

XIntc sys_intc;
Xuint32 data;
static XGpio twist;
static XGpio push;
XGpio ssr;
XGpio ssr2;
static XGpio btn;
static XGpio dc;
static XSpi spi;
XSpi_Config *spiConfig;
extern XTmrCtr sys_tmrctr;
extern XTmrCtr sys_tmr;
extern XTmrCtr sys_tmr2;
u32 status;
u32 controlReg;

static XGpio Gpio;
unsigned int ab = 0;
unsigned int a;
unsigned int b;
unsigned int last_ab;
unsigned int state1 = 1;
unsigned int state2;
unsigned int state3;
unsigned int state4;
unsigned int state5;
unsigned int state6;
unsigned int state7;
unsigned int num;
unsigned int pflag = 0;
unsigned int pushed = 0;
extern unsigned int mode;
unsigned int sflag = 0;
unsigned int sflag2 = 0;
unsigned int check;
unsigned int idle = 1;
unsigned int cleared = 0;
unsigned int ssr_time = 0;
unsigned int state = 0;

unsigned int cnt = 0;
unsigned int power = 0;
unsigned int temp = 0;
unsigned int num2 = 0;
unsigned int btn1;
unsigned int btn2;
unsigned int btn3;
unsigned int btn4;
unsigned int up;
unsigned int down;
extern int count;
extern int ssrcount;

unsigned int t = 0;
unsigned int t2 = 0;

void twist_handler(void* CallbackRef)
{
	ab = XGpio_DiscreteRead(&twist,1);
	XGpio *GpioPtr = (XGpio *)CallbackRef;
	b = ab % 2;
	a = ab / 2;
	//xil_printf("num = %d\n", ab);
	if (last_ab != ab) {

					if (a == 0 && b == 1 && state1 == 1) {
						state1 =0;
						state2 = 1;

						//xil_printf("state2");
					}else if (a == 0 && b == 0 && state2 == 1) {
						state2 =0;
						state3 = 1;
						//xil_printf("state3");
					}else if (a == 1 && b == 0 && state3 == 1) {
						state3 =0;
						state4 = 1;
						//xil_printf("state4");
					}else if (a == 1 && b == 1 && state4 == 1) {
						//if(num <100){
						num ++;
						//up = 1;
						//}
						state4 =0;
						state1 = 1;
						//xil_printf("state1");
					}else if (a == 1 && b == 0 && state1 == 1) {
						state1 =0;
						state5 = 1;
						//xil_printf("state5");
					}else if (a == 0 && b == 0 && state5 == 1) {
						state5 =0;
						state6 = 1;
						//xil_printf("state6");
					}else if (a == 0 && b == 1 && state6 == 1) {
						state6 = 0;
						state7 = 1;
						//xil_printf("state7");
					}else if (a == 1 && b == 1 && state7 == 1) {
						num --;
						state7 =0;
						state1 = 1;

						//xil_printf("state1");
					}
					else if (a == 1 && b == 1 && state2 == 1) {
										state2 = 0;
										state1 = 1;
										//xil_printf("state1");
					}
					else if (a == 1 && b == 1 && state5 == 1) {
										state5 = 0;
										state1 = 1;
										//xil_printf("state1");
					};
		last_ab = ab;
	}
	//xil_printf("n\n");
	XGpio_InterruptClear(GpioPtr, 1);
}

void push_handler(void* CallbackRef)
{

	XGpio *GpioPtr = (XGpio *)CallbackRef;
	pflag = XGpio_DiscreteRead(&push,1);
	if (pflag == 1 && mode == 4) {
		pushed = 1;
	}
	//xil_printf("p = %d\n", pflag);
	//xil_printf("push");
	XGpio_InterruptClear(GpioPtr, 1);

}

void ssr_handler(void* CallbackRef)
{
	XGpio *GpioPtr = (XGpio *)CallbackRef;
	sflag = XGpio_DiscreteRead(&ssr,1);
	//xil_printf("p = %d\n", sflag);

		if (sflag == 1) {
			ssrcount = 0;
			t = 0;
			XTmrCtr_Start(&sys_tmr, 1);
		}
		else if(sflag == 0){
			t = XTmrCtr_GetValue(&sys_tmr, 1);
			XTmrCtr_Stop(&sys_tmr, 1);
			XTmrCtr_Reset(&sys_tmr, 1);
			ssr_time = ssrcount;

		}

	//p = XGpio_DiscreteRead(&push,1);
	//XGpio_DiscreteWrite(&Gpio,1,1);
	XGpio_InterruptClear(GpioPtr, 1);
}

void ssr2_handler(void* CallbackRef)
{
	XGpio *GpioPtr = (XGpio *)CallbackRef;
	sflag2 = XGpio_DiscreteRead(&ssr2,1);
		if (sflag2 == 1) {
			t2 = 0;
			XTmrCtr_Start(&sys_tmr2, 0);
		}
		else if(sflag2 == 0){
			t2 = XTmrCtr_GetValue(&sys_tmr2, 0);
			XTmrCtr_Stop(&sys_tmr2, 0);
			XTmrCtr_Reset(&sys_tmr2, 0);
		}

	XGpio_InterruptClear(GpioPtr, 1);
}

void button_handler(void* CallbackRef)
{
	//XGpio_Initialize(&Gpio1, XPAR_AXI_GPIO_1_DEVICE_ID);

//	count1++;
	XGpio *GpioPtr = (XGpio *)CallbackRef;
	check = XGpio_DiscreteRead(&btn,1);
	//xil_printf("btn");
	//p = XGpio_DiscreteRead(&push,1);
	//XGpio_DiscreteWrite(&Gpio,1,1);
	XGpio_InterruptClear(GpioPtr, 1);



}

void BSP_init(void) {
	LCD_init();
	//XGpio_Initialize(&Gpio, XPAR_AXI_GPIO_1_DEVICE_ID);

	twist_initial();
	push_initial();
	button_initial ();
	ssr_initial();
	ssr2_initial();
	timeCount_initial();


}
void QF_onStartup(void) {

	XGpio_InterruptEnable(&twist, 1);
	XGpio_InterruptGlobalEnable(&twist);

	XGpio_InterruptEnable(&push, 1);
	XGpio_InterruptGlobalEnable(&push);

	XGpio_InterruptEnable(&ssr, 1);
	XGpio_InterruptGlobalEnable(&ssr);

	XGpio_InterruptEnable(&ssr2, 1);
	XGpio_InterruptGlobalEnable(&ssr2);

	XGpio_InterruptEnable(&btn, 1);
	XGpio_InterruptGlobalEnable(&btn);

	XIntc_Start(&sys_intc, XIN_REAL_MODE);
	timeCount_enable();

}

void QF_onIdle(void) {        /* entered with interrupts locked */

    QF_INT_UNLOCK();                       /* unlock interrupts */
    if (idle == 0){
    count = 0;
    idle = 1;
    cleared = 0;
    }
    if (idle && cleared == 0 && count > 4000 ) {
    	//xil_printf("clear\n");
		for(int j = 5; j<=num*40+5;j=j+40){
			setColor(0, 255, 0);
			fillRect(j, 5, j+30, 35);
			if (j<205) {
				setColor(0, 0, 255);
				fillRect(j+30,5,j+40,35);
			}else{
				setColor(0, 0, 255);
				fillRect(235,5,240,35);
			}
		}
		for(int j = 5; j<=2*40+5;j=j+40){
					setColor(0, 255, 0);
					fillRect(j, 45, j+30, 75);
					setColor(0, 0, 255);
					fillRect(j+30, 45, j+40, 75);
				}
		for(int j = 5; j<=2*40+5;j=j+40){
							setColor(0, 255, 0);
							fillRect(j, 85, j+30, 115);
							setColor(0, 0, 255);
							fillRect(j+30, 85, j+40, 115);

						}
		for(int j = 5; j<=2*40+5;j=j+40){
									setColor(0, 255, 0);
									fillRect(j, 125, j+30, 155);
									setColor(0, 0, 255);
									fillRect(j+30, 125, j+40, 155);

								}
		for(int j = 5; j<=2*40+5;j=j+40){
									setColor(0, 255, 0);
									fillRect(j, 165, j+30, 195);
									setColor(0, 0, 255);
									fillRect(j+30, 165, j+40, 195);

								}
    	cleared = 1;
    }
    {
    	                	b = ab % 2;
    	                	a = ab / 2;

    	                	if (pflag == 1 ){
    	                		idle = 0;
    	                		//xil_printf("in");
    	                				push_disable();
    	                				count = 0;
    	                				while(count<400){}
    	                				push_enable();
    	                				QActive_postISR((QActive *)&AO_Lab2A, ENCODER_CLICK);
    	                				//xil_printf("%d\n",pflag);
    	                				//
    	                				}
    	                	if (last_ab != ab) {
    	                		idle = 0;
								cnt++;
//    	                				twist_disable();
//										count = 0;
//										while(count<1){}
//										twist_enable();
	//							if (cnt % 4 == 0) {
									if(up==1){
										//QActive_postISR((QActive *)&AO_Lab2A, ENCODER_UP);
										up = 0;
									}
									else if(down = 1){
										//QActive_postISR((QActive *)&AO_Lab2A, ENCODER_DOWN);
										down = 0;
									}
	//								}
								}

								last_ab = ab;
								//xil_printf("ab = %d\n", ab);

    	                		}

    	            		btn1 = check % 2;
    	            		btn2 = check % 4 / 2;
    	            		btn3 = check % 8 / 4;
    	            		btn4 = check % 16 / 8;

    	            		setColor(0, 255, 0);
    	            		setColorBg(255, 0, 0);
    	            		setFont(BigFont);
    	            		if(btn1 == 1){
    	            			idle = 0;
    	            			btn_disable();
    	            			xil_printf("btn1\n");
    	            			lcdPrint("btn1", 5, 45);
    	            			count = 0;
    	            			while(count<500){}
    	            			btn_enable();
    	            		}else if (btn2 == 1) {
    	            			idle = 0;
    	            			btn_disable();
    	            			xil_printf("btn2\n");
    	            			lcdPrint("btn2", 5, 85);
    	            			count = 0;
    	            			while(count<500){}
    	            			btn_enable();
    	            		}else if (btn3 == 1) {
    	            			idle = 0;
    	            			btn_disable();
    	            			xil_printf("btn3\n");
    	            			lcdPrint("btn3", 5, 125);
    	            			count = 0;
    	            			while(count<500){}
    	            			btn_enable();

    	            		}else if (btn4 == 1) {
    	            			idle = 0;
    	            			btn_disable();
    	            			xil_printf("btn4\n");
    	            			lcdPrint("btn4", 5, 165);
    	            			count = 0;
    	            			while(count<500){}
    	            			btn_enable();

    	            		}
    	// Write code to increment your interrupt counter here.
    	// QActive_postISR((QActive *)&AO_Lab2A, ENCODER_DOWN); is used to post an event to your FSM
    	//QActive_postISR((QActive *)&AO_Lab2A, ENCODER_DOWN);


// 			Useful for Debugging, and understanding your Microblaze registers.
//    		u32 axi_ISR =  Xil_In32(intcPress.BaseAddress + XIN_ISR_OFFSET);
//    	    u32 axi_IPR =  Xil_In32(intcPress.BaseAddress + XIN_IPR_OFFSET);
//    	    u32 axi_IER =  Xil_In32(intcPress.BaseAddress + XIN_IER_OFFSET);
//
//    	    u32 axi_IAR =  Xil_In32(intcPress.BaseAddress + XIN_IAR_OFFSET);
//    	    u32 axi_SIE =  Xil_In32(intcPress.BaseAddress + XIN_SIE_OFFSET);
//    	    u32 axi_CIE =  Xil_In32(intcPress.BaseAddress + XIN_CIE_OFFSET);
//    	    u32 axi_IVR =  Xil_In32(intcPress.BaseAddress + XIN_IVR_OFFSET);
//    	    u32 axi_MER =  Xil_In32(intcPress.BaseAddress + XIN_MER_OFFSET);
//    	    u32 axi_IMR =  Xil_In32(intcPress.BaseAddress + XIN_IMR_OFFSET);
//    	    u32 axi_ILR =  Xil_In32(intcPress.BaseAddress + XIN_ILR_OFFSET) ;
//    	    u32 axi_IVAR = Xil_In32(intcPress.BaseAddress + XIN_IVAR_OFFSET);
//
//    	    // Expect to see 0x00000001
//    	    u32 gpioTestIER  = Xil_In32(sw_Gpio.BaseAddress + XGPIO_IER_OFFSET);
//    	    // Expect to see 0x00000001
//    	    u32 gpioTestISR  = Xil_In32(sw_Gpio.BaseAddress  + XGPIO_ISR_OFFSET ) & 0x00000003;
//
//    	    // Expect to see 0x80000000 in GIER
//    		u32 gpioTestGIER = Xil_In32(sw_Gpio.BaseAddress  + XGPIO_GIE_OFFSET ) & 0x80000000;


    }


/* Do not touch Q_onAssert */
/*..........................................................................*/

void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_LOCK();
    for (;;) {
    }
}

void debounceTwistInterrupt(){
	// Read both lines here? What is twist[0] and twist[1]?
	// How can you use reading from the two GPIO twist input pins to figure out which way the twist is going?
}

void debounceInterrupt() {
	QActive_postISR((QActive *)&AO_Lab2A, ENCODER_CLICK);
	// XGpio_InterruptClear(&sw_Gpio, GPIO_CHANNEL1); // (Example, need to fill in your own parameters
}

void twist_initial ()
{
	 u32 DataRead;

	 int status;

	       print("\r\n Running IntcSelfTestExample() for microblaze_0_axi_intc...\r\n");

	       if (status == 0) {
	          print("IntcSelfTestExample PASSED\r\n");
	       }
	       else {
	          print("IntcSelfTestExample FAILED\r\n");
	       }

	       {
	              int Status;

	              Status = IntcInterruptSetup(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);
	              if (Status == 0) {
	                 print("Intc Interrupt Setup PASSED\r\n");
	              }
	              else {
	                print("Intc Interrupt Setup FAILED\r\n");
	             }
	          }
	       {
	            u32 status;

	            print("\r\nRunning GpioInputExample() for twist...\r\n");

	            u32 DataRead;

	            status = XGpio_Initialize(&twist, XPAR_TWIST_DEVICE_ID);
	            if (status != XST_SUCCESS) {
	            	status = XST_FAILURE;
	            }

	            /* Set the direction for all signals to be inputs */
	            XGpio_SetDataDirection(&twist, 1, 0xFFFFFFFF);

	            if (status == 0) {
	               xil_printf("GpioInputExample PASSED. Read data:0x%X\r\n", DataRead);
	            }
	            else {
	               print("GpioInputExample FAILED.\r\n");
	            }
	         }

	            int Status;



	            print(" Press button to Generate Interrupt\r\n");

	            Status = GpioIntrExample(&sys_intc, &twist, \
	                                     XPAR_TWIST_DEVICE_ID, \
	                                     XPAR_MICROBLAZE_0_AXI_INTC_TWIST_IP2INTC_IRPT_INTR, \
										 1, &DataRead);
}

void push_initial ()
{
	u32 DataRead;


	GpioInputExample(XPAR_PUSH_DEVICE_ID, &DataRead);

	XGpio_Initialize(&push, push_device_ID);

	XIntc_Connect(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_PUSH_IP2INTC_IRPT_INTR,
			      (Xil_ExceptionHandler)push_handler, &push);
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_PUSH_IP2INTC_IRPT_INTR);
	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */
	XGpio_WriteReg(push_baseAddr,XGPIO_TRI_OFFSET,0xffffffff);
//	XGpio_InterruptEnable(&push, 1);
//	XGpio_InterruptGlobalEnable(&push);
//	XIntc_Start(&sys_intc, XIN_REAL_MODE);

}

void ssr_initial ()
{
	u32 DataRead;


	GpioInputExample(ssr_device_ID, &DataRead);

	XGpio_Initialize(&ssr, ssr_device_ID);

	XIntc_Connect(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_SENSOR_IP2INTC_IRPT_INTR,
			      (Xil_ExceptionHandler)ssr_handler, &ssr);
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_SENSOR_IP2INTC_IRPT_INTR);
	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */
	XGpio_WriteReg(ssr_baseAddr,XGPIO_TRI_OFFSET,0xffffffff);

}

void ssr2_initial ()
{
	u32 DataRead;


	GpioInputExample(ssr2_device_ID, &DataRead);

	XGpio_Initialize(&ssr2, ssr2_device_ID);

	XIntc_Connect(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_SENSOR2_IP2INTC_IRPT_INTR,
			      (Xil_ExceptionHandler)ssr2_handler, &ssr2);
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_SENSOR2_IP2INTC_IRPT_INTR);
	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */
	XGpio_WriteReg(ssr2_baseAddr,XGPIO_TRI_OFFSET,0xffffffff);

}

void ssr_disable()
{
	XIntc_Disable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_SENSOR_IP2INTC_IRPT_INTR);
}

void ssr_enable()
{
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_SENSOR_IP2INTC_IRPT_INTR);
}

void ssr2_disable()
{
	XIntc_Disable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_SENSOR2_IP2INTC_IRPT_INTR);
}

void ssr2_enable()
{
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_SENSOR2_IP2INTC_IRPT_INTR);
}

void push_disable()
{
	XIntc_Disable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_PUSH_IP2INTC_IRPT_INTR);
}

void push_enable()
{
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_PUSH_IP2INTC_IRPT_INTR);
}

void twist_disable()
{
	XIntc_Disable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_TWIST_IP2INTC_IRPT_INTR);
}

void twist_enable()
{
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_TWIST_IP2INTC_IRPT_INTR);
}

void btn_disable()
{
	XIntc_Disable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_GPIO_0_IP2INTC_IRPT_INTR);
}

void btn_enable()
{
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_GPIO_0_IP2INTC_IRPT_INTR);
}
void LCD_init(void) {
	status = XGpio_Initialize(&dc, XPAR_SPI_DC_DEVICE_ID);
			if (status != XST_SUCCESS)  {
				xil_printf("Initialize GPIO dc fail!\n");
				return XST_FAILURE;
			}

			/*
			 * Set the direction for all signals to be outputs
			 */
			XGpio_SetDataDirection(&dc, 1, 0x0);



			/*
			 * Initialize the SPI driver so that it is  ready to use.
			 */
			spiConfig = XSpi_LookupConfig(XPAR_SPI_DEVICE_ID);
			if (spiConfig == NULL) {
				xil_printf("Can't find spi device!\n");
				return XST_DEVICE_NOT_FOUND;
			}

			status = XSpi_CfgInitialize(&spi, spiConfig, spiConfig->BaseAddress);
			if (status != XST_SUCCESS) {
				xil_printf("Initialize spi fail!\n");
				return XST_FAILURE;
			}

			/*
			 * Reset the SPI device to leave it in a known good state.
			 */
			XSpi_Reset(&spi);

			/*
			 * Setup the control register to enable master mode
			 */
			controlReg = XSpi_GetControlReg(&spi);
			XSpi_SetControlReg(&spi,
					(controlReg | XSP_CR_ENABLE_MASK | XSP_CR_MASTER_MODE_MASK) &
					(~XSP_CR_TRANS_INHIBIT_MASK));

			// Select 1st slave device
			XSpi_SetSlaveSelectReg(&spi, ~0x01);

			initLCD();

			clrScr();
			xil_printf("LCD initialized\n");
}

void button_initial ()
{
	u32 DataRead;


	GpioInputExample(btn_device_ID, &DataRead);

	XGpio_Initialize(&btn, btn_device_ID);

	XIntc_Connect(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_GPIO_0_IP2INTC_IRPT_INTR,
			      (Xil_ExceptionHandler)button_handler, &btn);
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_GPIO_0_IP2INTC_IRPT_INTR);
	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */
	XGpio_WriteReg(btn_baseAddr,XGPIO_TRI_OFFSET,0xffffffff);
//	XGpio_InterruptEnable(&push, 1);
//	XGpio_InterruptGlobalEnable(&push);
//	XIntc_Start(&sys_intc, XIN_REAL_MODE);

}
