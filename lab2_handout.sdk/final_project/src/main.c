/*****************************************************************************
* main.c for Lab2A of ECE 153a at UCSB
* Date of the Last Update:  October 23,2014
*****************************************************************************/

#include "qpn_port.h"                                       /* QP-nano port */
#include "bsp.h"                             /* Board Support Package (BSP) */
#include "lab2a.h"                               /* application interface */
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>


static QEvent l_lab2aQueue[30];  

QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
	{ (QActive *)0,            (QEvent *)0,          0                    },
	{ (QActive *)&AO_Lab2A,    l_lab2aQueue,         Q_DIM(l_lab2aQueue)  }
};

Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);
extern XGpio ssr;
extern XGpio Gpio;
extern XTmrCtr sys_tmr;
extern XTmrCtr sys_tmr2;
extern XTmrCtr sys_tmr3;
extern int ssr_time;
extern int ssrcount;
extern int sflag;
extern unsigned int btn1;
extern unsigned int btn2;
extern unsigned int btn3;
extern unsigned int btn4;
extern unsigned int check;
extern unsigned int count;
extern unsigned int num;
extern unsigned int ab;
extern unsigned int pflag;
extern unsigned int pushed;
extern int t;
extern int t2;
int t3;
int ledn;
int slc = 1;
int old_t = 0;
unsigned int mode = 1;
int draw;
int old_count;
int old_count3;
int old_x;
int old_coin_y;
int old_block_y;
int roadColor[3] = {22, 72, 124}; //road color
int sideColor[3] = {150, 149, 144}; //roadside color
int topColor[3] = {255, 0, 0};
int frameColor[3] = {0, 0, 0};
int topColor1[3] = {123, 68, 65};
int frameColor1[3] = {32, 195, 230};
unsigned long int rand_next;

int block_x = 130;
int block_y;
int coin_x = 100;
int coin_y;
int coin = 0;
int score_tmp;
int record_tmp;
char score[4];
int records[5];
char record[4];

int movingTreeMode = 2;
unsigned short lfsr = 0xACE1u;
unsigned bit;

void drawCar(int x, int y, int frameColor[3], int topColor[3]) {
	setFont(BigFont);
	setColorBg(roadColor[0],roadColor[1],roadColor[2]);
	setColor(frameColor[0], frameColor[1], frameColor[2]);
	fillRect(x, y, x+15, y+12);
	fillRect(x, y+30, x+15, y+42);

	drawVLine(x, y+12, 5);
	drawVLine(x, y+25, 5);
	drawVLine(x+15, y+12, 5);
	drawVLine(x+15, y+25, 5);
	setColor(topColor[0], topColor[1], topColor[2]);
	fillRect(x, y+17, x+15, y+25);

}
void cleanCar(int x, int y) {
	setFont(BigFont);
	setColorBg(roadColor[0],roadColor[1],roadColor[2]);
	setColor(roadColor[0],roadColor[1],roadColor[2]);
	fillRect(x-5, y, x+20, y+42);
}
 void drawCoin(int x, int y){
	 setFont(BigFont);
	 setColorBg(roadColor[0],roadColor[1],roadColor[2]);
	 setColor(255, 255, 0);
	 int old_count2 = count;
//	 while(count-old_count2<60){
	 lcdPrint("0", x, y);
//	 }
//	 old_count2 = count;
//	 while(count-old_count2<60){
//		 lcdPrint("l", x, y);
//	 }
 }

 void drawTreeL(int x, int y){

	setFont(BigFont);
	setColor(139, 69, 19);
	fillRect(x-8,y+22,x+13,y+38);
	setColorBg(sideColor[0], sideColor[1], sideColor[2]);
	setColor(244, 110, 65);
	fillRect(x,y,x+5,y+30);
	setColor(0, 153, 0);
	lcdPrint("@", x-13, y-8);
	lcdPrint("@", x-8,  y-8);
	lcdPrint("@", x+2,  y-8);
	lcdPrint("@", x-8, y-12);
	lcdPrint("@", x-3, y-12);
 }

 void cleanTreeL(int x, int y){
 	setFont(BigFont);
 	setColorBg(sideColor[0], sideColor[1], sideColor[2]);
 	setColor(sideColor[0], sideColor[1], sideColor[2]);
 	fillRect(x-8,y+22,x+13,y+38);
 	fillRect(x,y,x+5,y+30);
 	lcdPrint("@", x-13, y-8);
 	lcdPrint("@", x-8,  y-8);
 	lcdPrint("@", x+2,  y-8);
 	lcdPrint("@", x-8, y-12);
 	lcdPrint("@", x-3, y-12);
  }

 void drawRoad(){
	setColor(roadColor[0],roadColor[1],roadColor[2]);
	fillRect(0,0,240,320);
	setColor(sideColor[0], sideColor[1], sideColor[2]);
	fillRect(0,0,45,320);
	setColor(sideColor[0], sideColor[1], sideColor[2]);
	fillRect(195,0,240,320);
 }

 void startPage(){
	 setColor(roadColor[0],roadColor[1],roadColor[2]);
	 fillRect(0,0,240,320);
	 setColor(255, 255, 153);
	 fillRect(38,148,202,166);
	 fillRect(38,198,202,216);
	 setColor(255, 0, 0);
	 setColorBg(253, 202, 62);
	 setFont(BigFont);
	 lcdPrint("  START  ", 50, 150);
	 lcdPrint("  RECORD ", 48, 200);
	 setFont(SmallFont);
	 lcdPrint("PRESS 'BTNL or BTNR' TO SELECT", 0, 260);
	 lcdPrint("  PRESS 'BTNU' TO ENTER  ", 25, 280);
 }

 void drawBtn1() {
	 setColor(255, 255, 153);
	fillRect(38,198,40,216);
	fillRect(199,198,202,216);
	fillRect(38,198,202,200);
	fillRect(38,215,202,216);

	setColor(255, 0, 127);
	fillRect(38,148,40,166);
	fillRect(199,148,202,166);
	fillRect(38,148,202,150);
	fillRect(38,165,202,166);
 }

 void drawBtn2(){
	 setColor(255, 0, 127);
	fillRect(38,198,40,216);
	fillRect(199,198,202,216);
	fillRect(38,198,202,200);
	fillRect(38,215,202,216);

	setColor(255, 255, 153);
	fillRect(38,148,40,166);
	fillRect(199,148,202,166);
	fillRect(38,148,202,150);
	fillRect(38,165,202,166);
 }

void waitForStart() {
	btn1 = check % 2;
	btn2 = check % 4 / 2;
	btn3 = check % 8 / 4;
	btn4 = check % 16 / 8;

	setColor(0, 255, 0);
	setColorBg(255, 0, 0);
	setFont(BigFont);
	if(btn2 == 1){
		btn_disable();
		if(slc<2){
		slc++;
		}
		count = 0;
		while(count<500){}
		btn_enable();
	}else if (btn3 == 1) {
		btn_disable();
		if(slc>1){
		slc--;
		}
		count = 0;
		while(count<500){}
		btn_enable();
	}
	if(slc == 1){
		drawBtn1();

	}else if(slc == 2){
		drawBtn2();
	}
	if(btn1 == 1 && slc == 1){
		mode = 2;
	}
	if(btn1 == 1 && slc == 2){
			mode = 4;
		}
}

void returnToStart(){
	//return to start page
	btn4 = check % 16 / 8;
	if(btn4 == 1){
		mode = 1;
	}
}
void drawScore() {
	setFont(SmallFont);
	setColor(roadColor[0],roadColor[1],roadColor[2]);
	score_tmp = coin;
	score[0] = score_tmp / 100 + 48;
	score_tmp -= (score[0] - 48) * 100;
	score[1] = coin / 10 + 48;
	score_tmp -= (score[1] - 48) * 10;
	score[2] = score_tmp + 48;
	score[3] = '\0';
	setColorBg(sideColor[0], sideColor[1], sideColor[2]);
	lcdPrint(score, 20, 20);
}
void drawGame(int x) {
	drawCar(x,260,frameColor,topColor);
	drawCoin(coin_x,coin_y);
	drawCar(block_x,block_y,frameColor1,topColor1);
	drawScore();
}

void drawrecordpage(){
		setFont(BigFont);
		setColor(roadColor[0],roadColor[1],roadColor[2]);
		fillRect(0,0,240,320);
		setColorBg(roadColor[0],roadColor[1],roadColor[2]);
		setColor(255, 226, 7);
		lcdPrint("Gold",40,100);
		setColor(232, 230, 220);
		lcdPrint("Silver",40,120);
		setColor(219, 114, 76);
		lcdPrint("Bronze",40,140);
		setColor(76, 219, 197);
		lcdPrint("4. ",40,160);
		setColor(76, 219, 197);
		lcdPrint("5. ",40,180);
}
void drawrecord(){
	setColor(255,255,0);
	setColorBg(roadColor[0],roadColor[1],roadColor[2]);
	for(int i=0; i<5; i++){
		record_tmp = records[i];
		record[0] = record_tmp / 100 + 48;
		record_tmp -= (record[0] - 48) * 100;
		record[1] = record_tmp / 10 + 48;
		record_tmp -= (record[1] - 48) * 10;
		record[2] = record_tmp + 48;
		record[3] = '\0';
		lcdPrint(record, 180, 100+20*i);
	}
	returnToStart();
}
void updateCointPosition(int x){
	coin_y = coin_y+5;
	if(coin_y>250&&coin_y<310){
		if(x-coin_x<10&&x-coin_x>-10){
			coin++;
			for(int i=0; i<5; i++){
				if(coin>records[i]){
					records[i]= coin;
					break;
				}
			}
			coin_y = 0;
			coin_x =abs((t3)%50)+abs((t)%50)+60;
			//coin_x = 100;//test
		}
	}
	if(coin_y>310){
		coin_y = 0;
		coin_x =abs((t3)%50)+abs((t)%50)+60;
		//coin_x = 100;//test

	}
}

void updateblockPosition(int x){
	block_y = block_y+5;
		if(block_y>220&&block_y<310){
			if(x-block_x<10&&x-block_x>-10){
				block_y = 0;
				mode = 3;
				block_x =abs((t3)%50)+abs((t2)%50)+60;
				//block_x = 150;//test
			}
		}
		if(block_y>310){
			block_y = 0;
			block_x =abs((t3)%50)+abs((t2)%50)+60;
			//block_x = 150;//test
		}
}
void cleanStuff(int x) {
	//cleanTreeL(15,old_coin_y+60);
	old_block_y = block_y;
	setColor(roadColor[0],roadColor[1],roadColor[2]);
	cleanCar(block_x,old_block_y);

	if(x != old_x) {
		old_x=x;
		cleanCar(old_x,260);
	}

	old_coin_y = coin_y;
	fillRect(coin_x,old_coin_y,coin_x+20,old_coin_y+20);

}

void movingTree(int x, int y) {

 if (count-old_count3 > 1000 && movingTreeMode == 1){
	 movingTreeMode = 2;
	 cleanTreeL(x,y);
	 cleanTreeL(x,3*y);
	 cleanTreeL(x,5*y);
	 drawTreeL(x,2*y);
 	 drawTreeL(x,4*y);
 	 old_count3 = count;
 }else if(count-old_count3 > 1000 && movingTreeMode == 2) {
	 cleanTreeL(x,2*y);
	 cleanTreeL(x,4*y);
	 movingTreeMode = 1;
	 drawTreeL(x,y);
	 drawTreeL(x,3*y);
	 drawTreeL(x,5*y);
	 old_count3 = count;
 }
}
void drawMTree(int x, int y) {
	for(int i=0; i<5; i++) {
		drawTreeL(x,y+i*60);
	}
}

int setPosition() {
	drawMTree(15,60);
	drawMTree(225, 30);
	int x = 150;
	mode = 4;
	setColor(255, 0, 0);
	setColorBg(roadColor[0], roadColor[1], roadColor[2]);
	setFont(SmallFont);
	lcdPrint("Set start position", 50, 150);
	drawCar(x,260,frameColor,topColor);
	while(pushed != 1){
	old_x = x;
	x = 52 + num;
	if (num > 118) {
		num = 118;
	}else if(x < 52) {
		num = 0;
	}
	if(x != old_x) {
		cleanCar(old_x,260);
		drawCar(x,260,frameColor,topColor);
	   }
	}
	num = 0;
	pushed = 0;
	setFont(SmallFont);
	setColor(roadColor[0], roadColor[1], roadColor[2]);
	lcdPrint("Set start position", 50, 150);
	mode = 2;
	setFont(BigFont);
	setColor(255, 0, 0);
	lcdPrint("3", 100, 150);
	setColor(roadColor[0], roadColor[1], roadColor[2]);
	count = 0;
	while(count<1000){}
	lcdPrint("3", 100, 150);
	setColor(255, 0, 0);
	lcdPrint("2", 100, 150);
	setColor(roadColor[0], roadColor[1], roadColor[2]);
	count = 0;
	while(count<1000){}
	lcdPrint("2", 100, 150);
	setColor(255, 0, 0);
	lcdPrint("1", 100, 150);
	setColor(roadColor[0], roadColor[1], roadColor[2]);
	count = 0;
	while(count<1000){}
	lcdPrint("1", 100, 150);
	setColor(255, 0, 0);
	lcdPrint("GOOOOO!", 60, 150);
	setColor(roadColor[0], roadColor[1], roadColor[2]);
	count = 0;
	while(count<1000){}
	lcdPrint("GOOOOO!", 60, 150);
	return x;
}
void playing(int x) {

	//old_count3 = count;
	while(mode == 2){
		t3 = XTmrCtr_GetValue(&sys_tmr3, 1);
		returnToStart(); //if user wants to return to start page, press 4
		old_count = count;
		drawGame(x);
		cleanStuff(x);
		//movingTree(15,60);
		//x=x+num; //control by rotary encoder
		x=x+(int)((t2-t)/2500); //control by sensors
		updateblockPosition(x);
		updateCointPosition(x);
		if(x<45 || x>185){
			mode = 3;
		}
	}
}

void gameOver() {
	setColorBg(0, 153, 0);
	setColor(253, 0, 0);
	setFont(SmallFont);
	lcdPrint("press 4 to restart", 40, 100);
	lcdPrint("press 1 to go to menu", 40, 120);
	setFont(BigFont);
	lcdPrint("Game Over", 40, 20);
	coin_x = 100;
	coin_y = 0;
	coin = 0;
	score_tmp = 0;
	score[4] = 0;
	num = 0;
}

int main(void) {
	Lab2A_ctor(); // inside of lab2a.c
	xil_printf("BSP start\n");
	BSP_init(); // inside of bsp.c, starts out empty!
	xil_printf("BSP end\n");
	QF_onStartup();
	ab = 3;
	XTmrCtr_Start(&sys_tmr3, 1);
	while(1) {

		if(mode == 1){
			startPage();
			while(mode == 1){
				waitForStart();
			}
		}else if(mode == 2){
			drawRoad();
			playing(setPosition());

		}else if(mode == 3){
			gameOver();
			while (mode == 3){
				btn1 = check % 2;
				btn4 = check % 16 / 8;
				if(btn4 == 1){
					mode = 1;
				}
				if(btn1 == 1){
					mode = 2;
				}
			}
		}else if(mode == 4){
			drawrecordpage();
			while(mode == 4){
				drawrecord();
			}

		}


	}

	return 0;
}
