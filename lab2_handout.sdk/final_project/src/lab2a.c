/*****************************************************************************
* lab2a.c for Lab2A of ECE 153a at UCSB
* Date of the Last Update:  October 23,2014
*****************************************************************************/

#define AO_LAB2A

#include "qpn_port.h"
#include "bsp.h"
#include "lab2a.h"
#include "lcd.h"
extern int ab ;
extern int num ;
extern int last_ab;
extern int count;
unsigned int temp;


typedef struct Lab2ATag  {               //Lab2A State machine
	QActive super;
}  Lab2A;

/* Setup state machines */
/**********************************************************************/
static QState Lab2A_initial (Lab2A *me);
static QState Lab2A_on      (Lab2A *me);
static QState Lab2A_stateA  (Lab2A *me);
static QState Lab2A_stateB  (Lab2A *me);


/**********************************************************************/


Lab2A AO_Lab2A;


void Lab2A_ctor(void)  {
	Lab2A *me = &AO_Lab2A;
	QActive_ctor(&me->super, (QStateHandler)&Lab2A_initial);
}


QState Lab2A_initial(Lab2A *me) {
	xil_printf("\n\rInitialization");
	ab = 3;
	num = 0;
	last_ab = 3;
    return Q_TRAN(&Lab2A_on);
}

QState Lab2A_on(Lab2A *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			//xil_printf("\n\rOn");
			setColor(0, 0, 255);
			fillRect(0, 0, 240, 320);
			setColor(0, 255, 0);
			for(int i = 5; i<235;i=i+40){
				for(int j = 5; j<315;j=j+40){
					fillRect(i, j, i+30, j+30);
				}
				}

			}

		case Q_INIT_SIG: {
			return Q_TRAN(&Lab2A_stateA);
		}
	}
	return Q_SUPER(&QHsm_top);

}


/* Create Lab2A_on state and do any initialization code if needed */
/******************************************************************/

QState Lab2A_stateA(Lab2A *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			xil_printf("Startup State A\n");
			return Q_HANDLED();
		}
		
		case ENCODER_UP: {
			xil_printf("Encoder Up from State A%d\n",num);
			setColor(255, 0, 0);
			//fillRect(205,5,235,num*40+5);
			if (num < 6) {
				fillRect(5,5,num*40+5,35);
			}else{
				fillRect(5,5,240,35);
			}
			return Q_HANDLED();
		}

		case ENCODER_DOWN: {
			xil_printf("Encoder Down from State A%d\n",num);
			setColor(255, 0, 0);
			//fillRect(205,5,235,num*40+5);
			if (num < 6) {
				fillRect(5,5,num*40+5,35);
			}else{
				fillRect(5,5,240,35);
			}
			for(int j=num*40+5; j<235; j=j+40){
			setColor(0, 255, 0);
			//fillRect(205,j,235,j+30);
			fillRect(j,5,j+30,35);

			if (j<205) {
				setColor(0, 0, 255);
				fillRect(j+30,5,j+40,35);
			}else{
				setColor(0, 0, 255);
				fillRect(235,5,240,35);
			}
			}

			return Q_HANDLED();
		}
		case ENCODER_CLICK:  {
			//xil_printf("Changing State\n");
			twist_disable();
			temp = num;
			for(int j = 5; j<=num*40+5;j=j+40){
				setColor(0, 255, 0);
				fillRect(j, 5, j+30, 35);
				setColor(0, 0, 255);
				fillRect(j+30, 5, j+40, 35);
			}
			return Q_TRAN(&Lab2A_stateB);
		}

	}


	return Q_SUPER(&Lab2A_on);

}

QState Lab2A_stateB(Lab2A *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			xil_printf("Startup State B\n");
			return Q_HANDLED();
		}
		
		case ENCODER_UP: {
			//xil_printf("Encoder Up from State B\n");

			return Q_HANDLED();
		}

		case ENCODER_DOWN: {
			//xil_printf("Encoder Down from State B\n");

			return Q_HANDLED();
		}

		case ENCODER_CLICK:  {
			//xil_printf("Changing State\n");
			twist_enable();
			num = temp;
			setColor(255, 0, 0);
			fillRect(5, 5, num*40+5, 35);
			return Q_TRAN(&Lab2A_stateA);


		}
	}
	return Q_SUPER(&Lab2A_on);

}

