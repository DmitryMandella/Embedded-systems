#include "lcd16x2_i2c_pcf8574.h"

extern I2C_HandleTypeDef hi2c1;

extern uint8_t SLAVE_ADDRESS_LCD;

void init_lcd(void){
	// 4 bit initialisation
		HAL_Delay(50);  // wait for >40ms
		send_command (0x30);
		HAL_Delay(5);  // wait for >4.1ms
		send_command (0x30);
		HAL_Delay(1);  // wait for >100us
		send_command (0x30);
		HAL_Delay(10);
		send_command (0x20);  // 4bit mode
		HAL_Delay(10);

	  // dislay initialisation
		send_command (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
		HAL_Delay(1);
		send_command (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
		HAL_Delay(1);
		send_command (0x01);  // clear display
		HAL_Delay(1);
		HAL_Delay(1);
		send_command (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
		HAL_Delay(1);
		send_command (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}


void send_command(char command) {
    char cmd_t[2];
    uint8_t command_to_send[4];

    cmd_t[0] = command & 0xF0;
    cmd_t[1] = (command << 4) & 0xF0;

    for (int i = 0; i < 2; i++) {
    	command_to_send[i * 2]     = cmd_t[i] | 0x0C; // EN=1
    	command_to_send[i * 2 + 1] = cmd_t[i] | 0x08; // EN=0
    }

    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t *)command_to_send, 4, 100);
}
void send_data(char data)
{
	char data_t[2];
	    uint8_t data_to_send[4];

	    data_t[0] = data & 0xF0;
	    data_t[1] = (data << 4) & 0xF0;

	    for (int i = 0; i < 2; i++) {
	    	data_to_send[i * 2]     = data_t[i] | 0x0D; // EN=1
	    	data_to_send[i * 2 + 1] = data_t[i] | 0x09; // EN=0
	    }

	    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t *)data_to_send, 4, 100);
}
void set_cursor(int row, int col) {
    if (row == 0) {
        col |= 0x80;
    } else if (row == 1) {
        col |= 0xC0;
    }
    send_command(col);
}

void send_string(char *string){
	while (*string) send_data (*string++);
}
void clear_lcd(void){
	send_command (0x01);
}
