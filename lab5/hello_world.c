/*

 * lab05.c
 *
 *  Created on: Oct 13, 2018
 *      Author: carneytl
 */
#include "alt_types.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "system.h"
#include <stdio.h>

int main(void){
	alt_up_char_buffer_dev * char_buf_dev;

	char_buf_dev = alt_up_char_buffer_open_dev("/dev/video_character_buffer_with_dma_0");

	if(char_buf_dev == NULL){
		printf("Error...");
	}
		else {
			printf("Opened");
		}
	char text = 'X';
	char text_top_row[40] = "Altera DE10_lite\0";
	char text_bottom_row[40] = "Character Buffer\0";

	alt_up_char_buffer_clear(char_buf_dev);
	alt_up_char_buffer_draw(char_buf_dev, text, 0, 0);
	alt_up_char_buffer_draw(char_buf_dev, text, 0, 59);
	alt_up_char_buffer_draw(char_buf_dev, text, 79, 0);
	alt_up_char_buffer_draw(char_buf_dev, text, 79, 59);
	alt_up_char_buffer_string(char_buf_dev, text_top_row, 20,20);
	alt_up_char_buffer_string(char_buf_dev, text_bottom_row, 40,40);

	printf("Done");

	return 0;
}
