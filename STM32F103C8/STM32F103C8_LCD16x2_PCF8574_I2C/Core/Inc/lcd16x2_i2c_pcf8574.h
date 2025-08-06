#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "main.h"

void init_lcd(void);
void send_command(char command);
void send_data(char data);
void set_cursor(int row, int column);
void send_string(char *string);
void clear_lcd(void);

#endif
