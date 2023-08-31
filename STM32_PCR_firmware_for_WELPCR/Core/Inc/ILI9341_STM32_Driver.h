#ifndef ILI9341_STM32_DRIVER_H
#define ILI9341_STM32_DRIVER_H

#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef hspi1;

#define ILI9341_SCREEN_HEIGHT 	240
#define ILI9341_SCREEN_WIDTH 	320

#define xpt2046_X_OFFSET 	200
#define xpt2046_Y_OFFSET 	420
#define xpt2046_MAX         3600
#define TOUCH_X_MULT_FACTOR (float)((float)ILI9341_SCREEN_HEIGHT/(float)xpt2046_MAX)
#define TOUCH_Y_MULT_FACTOR (float)((float)ILI9341_SCREEN_WIDTH/(float)xpt2046_MAX)

/* PIN Configuration */
#define HSPI_INSTANCE			&hspi1
#define LCD_CS_PORT				GPIOC
#define LCD_CS_PIN				GPIO_PIN_14
#define LCD_DC_PORT				GPIOB
#define LCD_DC_PIN				GPIO_PIN_2
#define	LCD_RST_PORT			GPIOB
#define	LCD_RST_PIN				GPIO_PIN_5
#define	touch_select_port       GPIOB
#define	touch_select_pin		GPIO_PIN_3
#define	touch_IRQ_port          GPIOB
#define	touch_IRQ_pin		    GPIO_PIN_8


#define BURST_MAX_SIZE 			500
#define BLACK      				0x0000
#define NAVY        			0x000F
#define DARKGREEN   			0x03E0
#define DARKCYAN    			0x03EF
#define MAROON      			0x7800
#define PURPLE      			0x780F
#define OLIVE       			0x7BE0
#define LIGHTGREY   			0xC618
#define DARKGREY    			0x7BEF
#define BLUE        			0x001F
#define GREEN       			0x07E0
#define CYAN        			0x07FF
#define RED         			0xF800
#define MAGENTA     			0xF81F
#define YELLOW      			0xFFE0
#define WHITE       			0xFFFF
#define ORANGE      			0xFD20
#define GREENYELLOW 			0xAFE5
#define PINK        			0xF81F

#define SCREEN_VERTICAL_1		0
#define SCREEN_HORIZONTAL_1		1
#define SCREEN_VERTICAL_2		2
#define SCREEN_HORIZONTAL_2		3

void ILI9341_WriteCommand(uint8_t cmd);
void ILI9341_WriteData(uint8_t data);
void ILI9341_WriteBuffer(uint8_t *buffer, uint16_t len);
void ILI9341_Reset(void);
void ILI9341_Enable(void);
void ILI9341_Init(void);
void ILI9341_SetRotation(uint8_t rotation);
void ILI9341_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ILI9341_DrawColor(uint16_t color);
void ILI9341_DrawColorBurst(uint16_t color, uint32_t size);
void ILI9341_FillScreen(uint16_t color);
void ILI9341_DrawPixel(uint16_t x,uint16_t y,uint16_t color);
void ILI9341_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void ILI9341_DrawHLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color);
void ILI9341_DrawVLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color);



typedef enum
{
  itpNONE = -1,
  itpNotPressed, // = 0
  itpPressed,    // = 1
  itpCOUNT       // = 2
}
ili9341_touch_pressed_t;


// convert value at addr to little-endian (16-bit)
#define __LEu16(addr)                                      \
    ( ( (((uint16_t)(*(((uint8_t *)(addr)) + 1)))      ) | \
        (((uint16_t)(*(((uint8_t *)(addr)) + 0))) << 8U) ) )
#endif
