/*
 * box.c
 * Lab 05
 *  Created on: Oct 16, 2018
 *      Author: Terry Carney
 */

#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "system.h"
#include <stdio.h>
#include <unistd.h>       // remove for using timer, not usleep()

int main()
{
	//global variables
	alt_u16 color = 0x0000;
	alt_u8 sw;
	alt_u8 key;

	//Used to control x and y coordinates.
	int x;
	int y;

	//Used to manually set initial position of first pixel. Couldn't place it exactly in screen center.
	int x0 = 100;
	int y0 = 100;
	int x1 = 100;
	int y1 = 100;


	alt_up_pixel_buffer_dma_dev* pixel_buf_dma_dev;
	pixel_buf_dma_dev = alt_up_pixel_buffer_dma_open_dev("/dev/video_pixel_buffer_dma_0");

	if(pixel_buf_dma_dev == NULL)
	{
		printf("Pixel buffer not opened\n");
	}
	else
		printf("Pixel buffer opened successfully!\n");

	alt_up_pixel_buffer_dma_clear_screen(pixel_buf_dma_dev, 0);
	usleep(1000000);
	alt_up_pixel_buffer_dma_draw_line(pixel_buf_dma_dev, x0, y0, x1, y1, color, 0);

	while(1)
	{
		//Use of switches and keybuttons.
		sw = IORD_ALTERA_AVALON_PIO_DATA(SW_PIO_BASE);
		key = IORD_ALTERA_AVALON_PIO_DATA(KEY_PIO_BASE);

		//if KEY1 is open, color of pixel will change.
		//Color-- is used multiple times for a noticeable change in color while in use.

		 if((key & 0x02)){
			 color--;
			 color--;
			 color--;
			 color--;
			 color--;
			 color--;
			 color--;
			 color--;
		 }
		 if (sw & 0x01){
			 //right
			 x++;
		 }
		 if (sw & 0x02){
			 //up
			 y++;
		 }
		 if (sw & 0x04){
			 //left
			 x--;
		 }
		 if (sw & 0x08){
			 //down
			 y--;
		 }
		 if (!(key & 0x01)){
			 //function is called to clear screen.
			 alt_up_pixel_buffer_dma_clear_screen(pixel_buf_dma_dev, 0);
		 }
		 	 //int x and int y are added to coordinates of function to move line.
			 alt_up_pixel_buffer_dma_draw_line(pixel_buf_dma_dev, x0 + x, y0 + y, x1 + x, y1 + y, color, 0);
		usleep(100000);
	}
	return 0;
}

