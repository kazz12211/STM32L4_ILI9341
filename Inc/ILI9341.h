#ifndef __ILI9341__
#define __ILI9341__

#include "stm32l4xx_hal.h"

#define Color_Black           0x0000      /*   0,   0,   0 */
#define Color_Navy            0x000F      /*   0,   0, 128 */
#define Color_DarkGreen       0x03E0      /*   0, 128,   0 */
#define Color_DarkCyan        0x03EF      /*   0, 128, 128 */
#define Color_Maroon          0x7800      /* 128,   0,   0 */
#define Color_Purple          0x780F      /* 128,   0, 128 */
#define Color_Olive           0x7BE0      /* 128, 128,   0 */
#define Color_LightGrey       0xC618      /* 192, 192, 192 */
#define Color_DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Color_Blue            0x001F      /*   0,   0, 255 */
#define Color_Green           0x07E0      /*   0, 255,   0 */
#define Color_Cyan            0x07FF      /*   0, 255, 255 */
#define Color_Red             0xF800      /* 255,   0,   0 */
#define Color_Magenta         0xF81F      /* 255,   0, 255 */
#define Color_Yellow          0xFFE0      /* 255, 255,   0 */
#define Color_White           0xFFFF      /* 255, 255, 255 */
#define Color_Orange          0xFD20      /* 255, 165,   0 */
#define Color_GreenYellow     0xAFE5      /* 173, 255,  47 */

void ILI9341_Init(SPI_HandleTypeDef *spi, uint16_t csPin, GPIO_TypeDef *csPort, uint16_t dcPin, GPIO_TypeDef *dcPort, uint16_t resetPin, GPIO_TypeDef *resetPort);
void ILI9341_SetOrientation(int orient);
void ILI9341_Invert(int flag);
void ILI9341_SetWindow(int x, int y, int w, int h);
void ILI9341_Pixel(int x, int y, uint16_t color);
void ILI9341_Clear();
uint16_t ILI9341_GetForeground();
uint16_t ILI9341_GetBackeground();
void ILI9341_Circle(int x, int y, int r, uint16_t color);
void ILI9341_FillCircle(int x, int y, int r, uint16_t color);
void ILI9341_Line(int x0, int y0, int x1, int y1, uint16_t color);
void ILI9341_Rect(int x0, int y0, int x1, int y1, uint16_t color);
void ILI9341_FillRect(int x0, int y0, int x1, int y1, uint16_t color);
void ILI9341_SetFont(unsigned char *f, unsigned char firstascii, unsigned char lastascii, int proportional);
void ILI9341_PutChar(int value);
void ILI9341_Character(int x, int y, int c);

#endif

