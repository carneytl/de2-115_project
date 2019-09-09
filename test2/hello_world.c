/*
 * gpio_test.c
 *
 *  Created on: Jun 21, 2019
 *      Author: carneytl
 */
#include "altera_avalon_pio_regs.h"
#include "altera_up_avalon_character_lcd_regs.h"
#include "altera_up_avalon_character_lcd.h"
#include "sys/alt_irq.h"
#include "alt_types.h"
#include "system.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void io_switch_isr(void * context);
void io_switch_setup();
int count = 1;
alt_u8 sw;
alt_u8 key;
char hello[] = "Hello, NIOS!";
char bye[] = "Goodbye, NIOS!";
volatile int edge_val = 0x00;


void io_switch_isr(void * context){
	volatile int * edge_ptr;
	edge_ptr = (volatile int *) context;
	*edge_ptr = IORD_ALTERA_AVALON_PIO_EDGE_CAP(SW_PIO_BASE);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SW_PIO_BASE, 0);
}
void io_switch_setup(void){ // Enable interrupts on 2 switches
IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SW_PIO_BASE, 0x03);
IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SW_PIO_BASE, 0x01);
void * edge_val_ptr;
edge_val_ptr = (void *) &edge_val;
alt_ic_isr_register(SW_PIO_IRQ_INTERRUPT_CONTROLLER_ID, SW_PIO_IRQ, io_switch_isr, edge_val_ptr, 0x00);
}

// end io_sw_setuP
void delay(int t){
	volatile int i = 0;
	while(i < t*10000){
		i++;
	}
}
void clearAllHex(int clear){
	IOWR_ALTERA_AVALON_PIO_DATA(HEX0_PIO_BASE, clear);
	IOWR_ALTERA_AVALON_PIO_DATA(HEX1_PIO_BASE, clear);
	IOWR_ALTERA_AVALON_PIO_DATA(HEX2_PIO_BASE, clear);
	IOWR_ALTERA_AVALON_PIO_DATA(HEX3_PIO_BASE, clear);
}
void BCDtoHEX(int c){
	switch (c){
		case 0:
			IOWR_ALTERA_AVALON_PIO_DATA(HEX0_PIO_BASE, 0xC0);
			break;
		case 1:
			IOWR_ALTERA_AVALON_PIO_DATA(HEX0_PIO_BASE, 0xF9);
			break;
		case 2:
			IOWR_ALTERA_AVALON_PIO_DATA(HEX0_PIO_BASE, 0xA4);
			break;
	}
}

int main(){
io_switch_setup();

	alt_up_character_lcd_dev * character_lcd_open_dev;
	character_lcd_open_dev = alt_up_character_lcd_open_dev("/dev/character_lcd_0");


	while(1){
		//printf("check\n");
		alt_up_character_lcd_erase_pos(character_lcd_open_dev, 0, 0);
		sw = IORD_ALTERA_AVALON_PIO_DATA(SW_PIO_BASE);
		key = IORD_ALTERA_AVALON_PIO_DATA(KEY_PIO_BASE);
		//clearAllHex(0x00);
		int count = 0;
		//printf("check\n");
		if(edge_val & 0x01){
			printf("edge\n");
			count++;
			edge_val = 0; //Detecting falling edge?
		}
		else if(edge_val & 0x02){
			count--;
			edge_val = 0;
		}
		IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE, count);

		//printf("%i\n",count);


	}
	return 0;
}
