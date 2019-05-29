#include "ILI9341.h"

struct {
	SPI_HandleTypeDef *spi;
	GPIO_TypeDef* dcPort;
	uint16_t dcPin;
	GPIO_TypeDef* csPort;
	uint16_t csPin;
	GPIO_TypeDef* rsPort;
	uint16_t rsPin;
	int inited;
	int orientation;
	int width;
	int height;
	uint16_t background;
	uint16_t foreground;
	unsigned char *font;
	int fontWidth;
	int fontHeight;
	int fontBytesPerLine;
	unsigned char firstascii;
	unsigned char lastascii;
} lcd;

void writeCmd8(uint8_t command) {
	HAL_GPIO_WritePin(lcd.dcPort, lcd.dcPin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(lcd.spi, &command, sizeof(uint8_t), 200);

	HAL_GPIO_WritePin(lcd.dcPort, lcd.dcPin, GPIO_PIN_SET);
}

void writeData8(uint8_t data) {
	HAL_SPI_Transmit(lcd.spi, &data, sizeof(uint8_t), 200);
}

void writeCmd16(uint16_t command) {
	uint8_t hi = command >> 8;
	uint8_t lo = command & 0xFF;

	HAL_GPIO_WritePin(lcd.dcPort, lcd.dcPin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(lcd.spi, &hi, sizeof(uint8_t), 200);
	HAL_SPI_Transmit(lcd.spi, &lo, sizeof(uint8_t), 200);

	HAL_GPIO_WritePin(lcd.dcPort, lcd.dcPin, GPIO_PIN_SET);
}

void writeData16(uint16_t data) {
	uint8_t hi = data >> 8;
	uint8_t lo = data & 0xFF;
	HAL_SPI_Transmit(lcd.spi, &hi, sizeof(uint8_t), 200);
	HAL_SPI_Transmit(lcd.spi, &lo, sizeof(uint8_t), 200);
}

void writeGram(uint16_t data) {
	uint8_t hi = data >> 8;
	uint8_t lo = data & 0xFF;
	HAL_SPI_Transmit(lcd.spi, &hi, sizeof(uint8_t), 200);
	HAL_SPI_Transmit(lcd.spi, &lo, sizeof(uint8_t), 200);
}

void writeGramCount(uint16_t data, uint16_t count) {
	uint8_t hi;
	uint8_t lo;

	if((data >> 8) == (data & 0xFF)) {
		count <<= 1;
		lo = data & 0xFF;
		while(count) {
			HAL_SPI_Transmit(lcd.spi, &lo, sizeof(uint8_t), 200);
			count--;
		}
	} else {
		hi = data >> 8;
		lo = data & 0xFF;
		while(count) {
			HAL_SPI_Transmit(lcd.spi, &hi, sizeof(uint8_t), 200);
			HAL_SPI_Transmit(lcd.spi, &lo, sizeof(uint8_t), 200);
			count--;
		}
	}
}

void writeGramBuf(uint16_t *data, uint16_t length) {
	uint8_t hi;
	uint8_t lo;
	while(length) {
		hi = *data >> 8;
		lo = *data & 0xFF;
		HAL_SPI_Transmit(lcd.spi, &hi, sizeof(uint8_t), 200);
		HAL_SPI_Transmit(lcd.spi, &lo, sizeof(uint8_t), 200);
		data++;
		length--;
	}
}


void ILI9341_Init(SPI_HandleTypeDef *spi, uint16_t csPin, GPIO_TypeDef *csPort, uint16_t dcPin, GPIO_TypeDef *dcPort, uint16_t rsPin, GPIO_TypeDef *rsPort) {
	lcd.spi = spi;
	lcd.dcPort = dcPort;
	lcd.dcPin = dcPin;
	lcd.csPort = csPort;
	lcd.csPin = csPin;
	lcd.rsPort = rsPort;
	lcd.rsPin = rsPin;

	HAL_GIPO_WritePin(lcd.dcPort, lcd.dcPin, GPIO_PIN_SET);
	HAL_GIPO_WritePin(lcd.csPort, lcd.csPin, GPIO_PIN_SET);
	HAL_GIPO_WritePin(lcd.rsPort, lcd.rsPin, GPIO_PIN_RESET);

	HAL_Delay(5);
	HAL_GIPO_WritePin(lcd.rsPort, lcd.rsPin, GPIO_PIN_SET);
	HAL_Delay(50);

	writeCmd8(0x01);
	HAL_Delay(5);
	writeCmd8(0x1B);


    /* Start Initial Sequence ----------------------------------------------------*/

    writeCmd8(0xCB);  // POWER_ON_SEQ_CONTROL
	writeData8(0x39);
	writeData8(0x2C);
	writeData8(0x00);
	writeData8(0x34);
	writeData8(0x02);

	writeCmd8(0xCF);  // POWER_CONTROL_B
	writeData8(0x00);
	writeData8(0xC1); // Applic Notes 81, was 83, C1 enables PCEQ: PC and EQ operation for power saving
	writeData8(0x30);

	writeCmd8(0xE8);  // DRIVER_TIMING_CONTROL_A
	writeData8(0x85);
	writeData8(0x00); // AN 10, was 01
	writeData8(0x78); // AN 7A, was 79

	writeCmd8(0xEA);  // DRIVER_TIMING_CONTROL_B
	writeData8(0x00);
	writeData8(0x00);

	writeCmd8(0xED);
	writeData8(0x64);
	writeData8(0x03);
	writeData8(0x12);
	writeData8(0x81);

	writeCmd8(0xF7);  // PUMP_RATIO_CONTROL
	writeData8(0x20);

	writeCmd8(0xC0);  // POWER_CONTROL_1
	writeData8(0x23); // AN 21, was 26

	writeCmd8(0xC1);  // POWER_CONTROL_2
	writeData8(0x10); // AN 11, was 11

	writeCmd8(0xC5);  // VCOM_CONTROL_1
	writeData8(0x3E); // AN 3F, was 35
	writeData8(0x28); // AN 3C, was 3E

	writeCmd8(0xC7);  // VCOM_CONTROL_2
	writeData8(0x86); // AN A7, was BE

	writeCmd8(0xB1);  // Frame Rate
	writeData8(0x00);
	writeData8(0x18); // AN 1B, was 1B  1B=70hz

	writeCmd8(0xB6);  // display function control, INTERESTING
	writeData8(0x08); // AN 0A, was 0A
	writeData8(0x82); // AN A2
	writeData8(0x27); // AN not present
	//   writeData8(0x00);  // was present

	writeCmd8(0xF2);  // Gamma Function Disable
	writeData8(0x00); // AN 00, was 08

	writeCmd8(0x26);
	writeData8(0x01); // gamma set for curve 01/2/04/08

	writeCmd8(0xE0);  // positive gamma correction
	writeData8(0x0F);
	writeData8(0x31);
	writeData8(0x2B);
	writeData8(0x0C);
	writeData8(0x0E);
	writeData8(0x08);
	writeData8(0x4E);
	writeData8(0xF1);
	writeData8(0x37);
	writeData8(0x07);
	writeData8(0x10);
	writeData8(0x03);
	writeData8(0x0E);
	writeData8(0x09);
	writeData8(0x00);

	writeCmd8(0xE1);  // negativ gamma correction
	writeData8(0x00);
	writeData8(0x0E);
	writeData8(0x14);
	writeData8(0x03);
	writeData8(0x11);
	writeData8(0x07);
	writeData8(0x31);
	writeData8(0xC1);
	writeData8(0x48);
	writeData8(0x08);
	writeData8(0x0F);
	writeData8(0x0C);
	writeData8(0x31);
	writeData8(0x36);
	writeData8(0x0F);

	//writeCmd8(0x34);                     // tearing effect off

	//writeCmd8(0x35);                     // tearing effect on

	//   writeCmd8(0xB7);                       // ENTRY_MODE_SET
	//   writeData8(0x07);

	writeCmd8(0x36);      // MEMORY_ACCESS_CONTROL (orientation stuff)
	writeData8(0x48);

	writeCmd8(0x3A);      // COLMOD_PIXEL_FORMAT_SET
	writeData8(0x55);     // 16 bit pixel

	writeCmd8(0x13); // Nomal Displaymode

	writeCmd8(0x11);                     // sleep out
	HAL_Delay(150);

	writeCmd8(0x29);                     // display on
	HAL_Delay(150);

	lcd.width = 240;
	lcd.height = 320;
	lcd.background = Color_Black;
	lcd.foreground = Color_White;
	lcd.inited = 1;
}


void ILI9341_SetOrientation(int orient) {
	lcd.orientation = orient;
	writeCmd8(0x36);
	switch(lcd.orientation) {
	case 0:		// portrait 0 deg
		writeData8(0x48);
		lcd.width = 240;
		lcd.height = 320;
		break;
	case 1:		// landscape +90
		writeData8(0x28);
		lcd.width = 320;
		lcd.height = 240;
		break;
	case 2:		// portrait +180
		writeData8(0x09);
		lcd.width = 240;
		lcd.height = 320;
		break;
	case 3:		// landscape -90
		writeData8(0x2B);
		lcd.width = 320;
		lcd.height = 240;
		break;
	}
}

void ILI9341_Invert(int flag) {
	if(flag == 0) {
		writeCmd8(0x20);
	} else {
		writeCmd8(0x21);
	}
}

void ILI9341_SetWindow(int x, int y, int w, int h) {
	writeCmd8(0x2A);
	writeData16(x);
	writeData16(x + w - 1);

	writeCmd8(0x2B);
	writeData16(y);
	writeData16(y + h - 1);
}

void ILI9341_Pixel(int x, int y, uint16_t color) {
	ILI9341_SetWindow(x, y, 1, 1);
	writeGram(color);

}

void ILI9341_Clear() {
	ILI9341_SetWindow(0, 0, lcd.width, lcd.height);
	uint16_t pixels = lcd.width * lcd.height;
	for(uint16_t i = 0; i < pixels; i++) {
		writeGram(lcd.background);
	}
}

uint16_t ILI9341_GetForeground() {
	return lcd.foreground;
}

uint16_t ILI9341_GetBackeground() {
	return lcd.background;
}


void ILI9341_Circle(int x, int y, int r, uint16_t color) {

}

void ILI9341_FillCircle(int x, int y, int r, uint16_t color) {

}

void ILI9341_Line(int x0, int y0, int x1, int y1, uint16_t color) {

}

void ILI9341_Rect(int x0, int y0, int x1, int y1, uint16_t color) {

}

void ILI9341_FillRect(int x0, int y0, int x1, int y1, uint16_t color) {

}

void ILI9341_SetFont(unsigned char *f, unsigned char firstascii, unsigned char lastascii, int proportional) {

}

void ILI9341_PutChar(int value) {

}

void ILI9341_Character(int x, int y, int c) {

}

