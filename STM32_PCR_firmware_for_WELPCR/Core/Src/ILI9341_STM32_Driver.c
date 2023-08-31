#include "ILI9341_STM32_Driver.h"

volatile uint16_t LCD_HEIGHT = ILI9341_SCREEN_HEIGHT;
volatile uint16_t LCD_WIDTH	 = ILI9341_SCREEN_WIDTH;


#pragma GCC push_options
#pragma GCC optimize ("O0")





void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Deselect when Tx Complete */
  if(hspi == HSPI_INSTANCE)
  {
	  HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
  }
}

static void ILI9341_SPI_Tx(uint8_t data)
{
	while(!__HAL_SPI_GET_FLAG(HSPI_INSTANCE, SPI_FLAG_TXE));
	HAL_SPI_Transmit_DMA(HSPI_INSTANCE, &data, 1);
	//HAL_SPI_Transmit(HSPI_INSTANCE, &data, 1, 10);
}

static void ILI9341_SPI_TxBuffer(uint8_t *buffer, uint16_t len)
{
	while(!__HAL_SPI_GET_FLAG(HSPI_INSTANCE, SPI_FLAG_TXE));
	HAL_SPI_Transmit_DMA(HSPI_INSTANCE, buffer, len);
	//HAL_SPI_Transmit(HSPI_INSTANCE, buffer, len, 10);
}

void ILI9341_WriteCommand(uint8_t cmd)
{
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET);	//command
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);	//select
	ILI9341_SPI_Tx(cmd);
	//HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);	//deselect
}

void ILI9341_WriteData(uint8_t data)
{
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);	//data
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);	//select
	ILI9341_SPI_Tx(data);
	//HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);	//deselect
}

void ILI9341_WriteBuffer(uint8_t *buffer, uint16_t len)
{
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);	//data
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);	//select
	ILI9341_SPI_TxBuffer(buffer, len);
	//HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);	//deselect
}

void ILI9341_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint8_t buffer[4];
	buffer[0] = x1 >> 8;
	buffer[1] = x1;
	buffer[2] = x2 >> 8;
	buffer[3] = x2;

	ILI9341_WriteCommand(0x2A);
	ILI9341_WriteBuffer(buffer, sizeof(buffer));

	buffer[0] = y1 >> 8;
	buffer[1] = y1;
	buffer[2] = y2 >> 8;
	buffer[3] = y2;

	ILI9341_WriteCommand(0x2B);
	ILI9341_WriteBuffer(buffer, sizeof(buffer));

	ILI9341_WriteCommand(0x2C);
}

void ILI9341_Reset(void)
{
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET);	//Disable
	HAL_Delay(10);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);		//Select
	HAL_Delay(10);
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);		//Enable
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET); 		//Deselect
}

void ILI9341_Enable(void)
{
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);		//Enable
}

void ILI9341_Init(void)
{

	ILI9341_Enable();
	ILI9341_Reset();

	//SOFTWARE RESET
	ILI9341_WriteCommand(0x01);
	HAL_Delay(10);

	//POWER CONTROL A
	ILI9341_WriteCommand(0xCB);
	ILI9341_WriteData(0x39);
	ILI9341_WriteData(0x2C);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0x34);
	ILI9341_WriteData(0x02);

	//POWER CONTROL B
	ILI9341_WriteCommand(0xCF);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0xC1);
	ILI9341_WriteData(0x30);

	//DRIVER TIMING CONTROL A
	ILI9341_WriteCommand(0xE8);
	ILI9341_WriteData(0x85);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0x78);

	//DRIVER TIMING CONTROL B
	ILI9341_WriteCommand(0xEA);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0x00);

	//POWER ON SEQUENCE CONTROL
	ILI9341_WriteCommand(0xED);
	ILI9341_WriteData(0x64);
	ILI9341_WriteData(0x03);
	ILI9341_WriteData(0x12);
	ILI9341_WriteData(0x81);

	//PUMP RATIO CONTROL
	ILI9341_WriteCommand(0xF7);
	ILI9341_WriteData(0x20);

	//POWER CONTROL,VRH[5:0]
	ILI9341_WriteCommand(0xC0);
	ILI9341_WriteData(0x23);

	//POWER CONTROL,SAP[2:0];BT[3:0]
	ILI9341_WriteCommand(0xC1);
	ILI9341_WriteData(0x10);

	//VCM CONTROL
	ILI9341_WriteCommand(0xC5);
	ILI9341_WriteData(0x3E);
	ILI9341_WriteData(0x28);

	//VCM CONTROL 2
	ILI9341_WriteCommand(0xC7);
	ILI9341_WriteData(0x86);

	//MEMORY ACCESS CONTROL
	ILI9341_WriteCommand(0x36);
	ILI9341_WriteData(0x48);

	//PIXEL FORMAT
	ILI9341_WriteCommand(0x3A);
	ILI9341_WriteData(0x55);

	//FRAME RATIO CONTROL, STANDARD RGB COLOR
	ILI9341_WriteCommand(0xB1);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0x18);

	//DISPLAY FUNCTION CONTROL
	ILI9341_WriteCommand(0xB6);
	ILI9341_WriteData(0x08);
	ILI9341_WriteData(0x82);
	ILI9341_WriteData(0x27);

	//3GAMMA FUNCTION DISABLE
	ILI9341_WriteCommand(0xF2);
	ILI9341_WriteData(0x00);

	//GAMMA CURVE SELECTED
	ILI9341_WriteCommand(0x26);
	ILI9341_WriteData(0x01);

	//POSITIVE GAMMA CORRECTION
	ILI9341_WriteCommand(0xE0);
	ILI9341_WriteData(0x0F);
	ILI9341_WriteData(0x31);
	ILI9341_WriteData(0x2B);
	ILI9341_WriteData(0x0C);
	ILI9341_WriteData(0x0E);
	ILI9341_WriteData(0x08);
	ILI9341_WriteData(0x4E);
	ILI9341_WriteData(0xF1);
	ILI9341_WriteData(0x37);
	ILI9341_WriteData(0x07);
	ILI9341_WriteData(0x10);
	ILI9341_WriteData(0x03);
	ILI9341_WriteData(0x0E);
	ILI9341_WriteData(0x09);
	ILI9341_WriteData(0x00);

	//NEGATIVE GAMMA CORRECTION
	ILI9341_WriteCommand(0xE1);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0x0E);
	ILI9341_WriteData(0x14);
	ILI9341_WriteData(0x03);
	ILI9341_WriteData(0x11);
	ILI9341_WriteData(0x07);
	ILI9341_WriteData(0x31);
	ILI9341_WriteData(0xC1);
	ILI9341_WriteData(0x48);
	ILI9341_WriteData(0x08);
	ILI9341_WriteData(0x0F);
	ILI9341_WriteData(0x0C);
	ILI9341_WriteData(0x31);
	ILI9341_WriteData(0x36);
	ILI9341_WriteData(0x0F);

	//EXIT SLEEP
	ILI9341_WriteCommand(0x11);
	HAL_Delay(100);

	//TURN ON DISPLAY
	ILI9341_WriteCommand(0x29);

	//STARTING ROTATION
	ILI9341_SetRotation(SCREEN_VERTICAL_1);
}

void ILI9341_SetRotation(uint8_t rotation)
{
	ILI9341_WriteCommand(0x36);
	HAL_Delay(1);

	switch(rotation)
	{
	case SCREEN_VERTICAL_1:
		ILI9341_WriteData(0x40|0x08);
		LCD_WIDTH = 240;
		LCD_HEIGHT = 320;
		break;
	case SCREEN_HORIZONTAL_1:
		ILI9341_WriteData(0x20|0x08);
		LCD_WIDTH  = 320;
		LCD_HEIGHT = 240;
		break;
	case SCREEN_VERTICAL_2:
		ILI9341_WriteData(0x80|0x08);
		LCD_WIDTH  = 240;
		LCD_HEIGHT = 320;
		break;
	case SCREEN_HORIZONTAL_2:
		ILI9341_WriteData(0x40|0x80|0x20|0x08);
		LCD_WIDTH  = 320;
		LCD_HEIGHT = 240;
		break;
	default:
		break;
	}
}

void ILI9341_DrawColor(uint16_t color)
{
	uint8_t buffer[2] = {color>>8, color};
	ILI9341_WriteBuffer(buffer, sizeof(buffer));
}

void ILI9341_DrawColorBurst(uint16_t color, uint32_t size)
{
	uint32_t BufferSize = 0;

	if((size*2) < BURST_MAX_SIZE)
	{
		BufferSize = size;
	}
	else
	{
		BufferSize = BURST_MAX_SIZE;
	}

	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);

	uint8_t chifted = color>>8;
	uint8_t BurstBuffer[BufferSize];

	for(uint32_t j = 0; j < BufferSize; j+=2)
	{
		BurstBuffer[j] = chifted;
		BurstBuffer[j+1] = color;
	}

	uint32_t SendingSize = size * 2;
	uint32_t SendingInBlock = SendingSize / BufferSize;
	uint32_t RemainderFromBlock = SendingSize % BufferSize;

	if(SendingInBlock != 0)
	{
		for(uint32_t j = 0; j < (SendingInBlock); j++)
		{
			HAL_SPI_Transmit(HSPI_INSTANCE, BurstBuffer, BufferSize, 10);
		}
	}

	HAL_SPI_Transmit(HSPI_INSTANCE, BurstBuffer, RemainderFromBlock, 10);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
}

void ILI9341_FillScreen(uint16_t color)
{
	ILI9341_SetAddress(0, 0, LCD_WIDTH, LCD_HEIGHT);
	ILI9341_DrawColorBurst(color, LCD_WIDTH*LCD_HEIGHT);
}

void ILI9341_DrawPixel(uint16_t x,uint16_t y,uint16_t color)
{
	if((x >=LCD_WIDTH) || (y >=LCD_HEIGHT)) return;

	uint8_t bufferX[4] = {x>>8, x, (x+1)>>8, (x+1)};
	uint8_t bufferY[4] = {y>>8, y, (y+1)>>8, (y+1)};
	uint8_t bufferC[2] = {color>>8, color};

	ILI9341_WriteCommand(0x2A);						//ADDRESS
	ILI9341_WriteBuffer(bufferX, sizeof(bufferX));	//XDATA

	ILI9341_WriteCommand(0x2B);						//ADDRESS
	ILI9341_WriteBuffer(bufferY, sizeof(bufferY));	//YDATA

	ILI9341_WriteCommand(0x2C);						//ADDRESS
	ILI9341_WriteBuffer(bufferC, sizeof(bufferC));	//COLOR
}

void ILI9341_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
	if((x >=LCD_WIDTH) || (y >=LCD_HEIGHT)) return;

	if((x+width-1)>=LCD_WIDTH)
	{
		width=LCD_WIDTH-x;
	}

	if((y+height-1)>=LCD_HEIGHT)
	{
		height=LCD_HEIGHT-y;
	}

	ILI9341_SetAddress(x, y, x+width-1, y+height-1);
	ILI9341_DrawColorBurst(color, height*width);
}

void ILI9341_DrawHLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
{
	if((x >=LCD_WIDTH) || (y >=LCD_HEIGHT)) return;

	if((x+width-1)>=LCD_WIDTH)
	{
		width=LCD_WIDTH-x;
	}

	ILI9341_SetAddress(x, y, x+width-1, y);
	ILI9341_DrawColorBurst(color, width);
}

void ILI9341_DrawVLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color)
{
	if((x >=LCD_WIDTH) || (y >=LCD_HEIGHT)) return;

	if((y+height-1)>=LCD_HEIGHT)
	{
		height=LCD_HEIGHT-y;
	}

	ILI9341_SetAddress(x, y, x, y+height-1);
	ILI9341_DrawColorBurst(color, height);
}



void ili9341_spi_touch_select(void)
{
  // clear bit indicates the touch screen is -active- slave SPI device
  HAL_GPIO_WritePin(touch_select_port, touch_select_pin, GPIO_PIN_RESET);
}

void ili9341_spi_touch_release(void)
{
  // set bit indicates the touch screen is -inactive- slave SPI device
  HAL_GPIO_WritePin(touch_select_port, touch_select_pin, GPIO_PIN_SET);
}

void ili9341_touch_coordinate(int16_t *x_pos, int16_t *y_pos)
{
  // XPT2046 8-bit command patterns
  static uint8_t x_cmd[]  = { 0xD3 };
  static uint8_t y_cmd[]  = { 0x93 };
  static uint8_t sleep[]  = { 0x00 };

  uint32_t x_avg = 0U;
  uint32_t y_avg = 0U;

  int32_t x_pos_raw = 0U;
  int32_t y_pos_raw = 0U;

  uint16_t sample = 8;
  uint16_t num_samples = 0U;

  // change SPI clock to 2MHz, max rate supported by XPT2046
  // TODO: based on STM32G4, which is clocked at 170MHz. support other chips.
  MODIFY_REG(hspi1.Instance->CR1, SPI_CR1_BR, SPI_BAUDRATEPRESCALER_128);

  ili9341_spi_touch_select();

  while (sample--) {

    uint8_t x_raw[2];
    uint8_t y_raw[2];

    HAL_SPI_Transmit(HSPI_INSTANCE, (uint8_t*)x_cmd, sizeof(x_cmd), HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(HSPI_INSTANCE, (uint8_t*)x_cmd, x_raw, sizeof(x_raw), HAL_MAX_DELAY);

    HAL_SPI_Transmit(HSPI_INSTANCE, (uint8_t*)y_cmd, sizeof(y_cmd), HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(HSPI_INSTANCE, (uint8_t*)y_cmd, y_raw, sizeof(y_raw), HAL_MAX_DELAY);

    x_avg += __LEu16(x_raw) >> 3;
    y_avg += __LEu16(y_raw) >> 3;

  }
  HAL_SPI_Transmit(HSPI_INSTANCE, (uint8_t*)sleep, sizeof(sleep), HAL_MAX_DELAY);

  ili9341_spi_touch_release();

  // restore SPI clock to maximum for TFT
  // TODO: based on STM32G4, which is clocked at 170MHz. support other chips.
  MODIFY_REG(hspi1.Instance->CR1, SPI_CR1_BR, SPI_BAUDRATEPRESCALER_8);


  x_pos_raw =(uint16_t)(((x_avg/8)-xpt2046_X_OFFSET)*TOUCH_X_MULT_FACTOR);
  y_pos_raw =(uint16_t)(((y_avg/8)-xpt2046_Y_OFFSET)*TOUCH_Y_MULT_FACTOR);
  //*x_pos = x_pos_raw;
  //*y_pos = y_pos_raw;

  if(x_pos_raw > ILI9341_SCREEN_HEIGHT ||  y_pos_raw>ILI9341_SCREEN_WIDTH)
  {
	  *x_pos = -1;
	  *y_pos = -1;
  }
  else
  {
	  *x_pos = y_pos_raw ;
	  *y_pos = x_pos_raw ;
  }



}


#pragma GCC pop_options

/*
void ili9341_read_touch(uint16_t *x_pos, uint16_t *y_pos)
{
  //uint16_t x_pos;
  //uint16_t y_pos;

  // read the new/incoming state of the touch screen
      ili9341_touch_coordinate(&x_pos, &y_pos);
}
*/
