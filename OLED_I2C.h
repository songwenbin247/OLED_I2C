#ifndef __OLED_I2C_H
#define __OLED_I2C_H


typedef unsigned char uint8_t;
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);

void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);//??????
void OLED_Fill(unsigned char fill_Data, int m);//??????
void OLED_CLS(void);//??
void OLED_ON(void);//?OLED??????
void OLED_OFF(void);//?OLED?? -- ?????,OLED????10uA
 /**
  * @brief  OLED_ShowStr???codetab.h??ASCII??,?6*8?8*16???
  * @param  x,y : ?????(x:0~127, y:0~7);
    *                   ch[] :- ???????; 
    *                   TextSize : ????(1:6*8 ; 2:8*16)
    * @retval ?
  */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);

 /**
  * @brief  OLED_ShowCN??
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);

 /**
  * @brief  OLED_DrawBMP???BMP??
  * @param  x0,y0 :?????(x0:0~127, y0:0~7);
    *                   x1,y1 : ?????(???)???(x1:1~128,y1:1~8)
    * @retval ?
  */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);

#endif
