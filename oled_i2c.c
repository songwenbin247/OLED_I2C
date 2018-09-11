#include "OLED_I2C.h"
#include "codetab.h"//????
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>




static int file;

void I2C_WriteByte(uint8_t addr,uint8_t data)
{
    char buf[2];
	 buf[0] = addr;
	 buf[1] = data;
	if (write(file,buf,2) != 2) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");

	}
}


 /**
  * @brief  WriteCmd??OLED????
  * @param  I2C_Command?????
  * @retval ?
  */
void WriteCmd(unsigned char I2C_Command)//???
{
    I2C_WriteByte(0x00, I2C_Command);
}


 /**
  * @brief  WriteDat??OLED????
  * @param  I2C_Data???
  * @retval ?
  */
void WriteDat(unsigned char I2C_Data)//???
{
    I2C_WriteByte(0x40, I2C_Data);
}

int i2c_init(void) 
{
	char *filename = "/dev/i2c-1";
	const char *buffer;
    int addr = 0x3c;        // The I2C address of the ADC

	if ((file = open(filename, O_RDWR)) < 0) {
		/* ERROR HANDLING: you can check errno to see what went wrong */
		perror("Failed to open the i2c bus");
		exit(1);
	}
    if (ioctl(file,I2C_SLAVE,addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
}

 /**
  * @brief  OLED_Init????OLED
  * @param  ?
  * @retval ?
  */
void OLED_Init(void)
{
  
    WriteCmd(0xAE); //display off
    WriteCmd(0x20); //Set Memory Addressing Mode    
    WriteCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8); //Set COM Output Scan Direction
    WriteCmd(0x00); //---set low column address
    WriteCmd(0x10); //---set high column address
    WriteCmd(0x40); //--set start line address
    WriteCmd(0x81); //--set contrast control register
    WriteCmd(0xff); //???? 0x00~0xff
    WriteCmd(0xa1); //--set segment re-map 0 to 127
    WriteCmd(0xa6); //--set normal display
    WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
    WriteCmd(0x3F); //
    WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    WriteCmd(0xd3); //-set display offset
    WriteCmd(0x00); //-not offset
    WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
    WriteCmd(0xf0); //--set divide ratio
    WriteCmd(0xd9); //--set pre-charge period
    WriteCmd(0x22); //
    WriteCmd(0xda); //--set com pins hardware configuration
    WriteCmd(0x12);
    WriteCmd(0xdb); //--set vcomh
    WriteCmd(0x20); //0x20,0.77xVcc
    WriteCmd(0x8d); //--set DC-DC enable
    WriteCmd(0x14); //
    WriteCmd(0xaf); //--turn on oled panel
}


 /**
  * @brief  OLED_SetPos?????
  * @param  x,??x??
    *                   y???y??
  * @retval ?
  */
void OLED_SetPos(unsigned char x, unsigned char y) //???????
{ 
    WriteCmd(0xb0+y);
    WriteCmd(((x&0xf0)>>4)|0x10);
    WriteCmd((x&0x0f)|0x01);
}

 /**
  * @brief  OLED_Fill???????
  * @param  fill_Data:??????
    * @retval ?
  */
void OLED_Fill(unsigned char fill_Data, int m)
{
    unsigned char n;
   
        WriteCmd(0xb0+m);       //page0-page1
        WriteCmd(0x00);     //low column start address
        WriteCmd(0x10);     //high column start address
        for(n=0;n<128;n++)
            {
                WriteDat(fill_Data);
            }

}

 /**
  * @brief  OLED_CLS???
  * @param  ?
    * @retval ?
  */
void OLED_CLS(void)//??
{
    int m = 0;
	for (m=0; m< 8; m++)
		OLED_Fill(0x00, m);
}


 /**
  * @brief  OLED_ON??OLED??????
  * @param  ?
    * @retval ?
  */
void OLED_ON(void)
{
    WriteCmd(0X8D);  //?????
    WriteCmd(0X14);  //?????
    WriteCmd(0XAF);  //OLED??
}


 /**
  * @brief  OLED_OFF??OLED?? -- ?????,OLED????10uA
  * @param  ?
    * @retval ?
  */
void OLED_OFF(void)
{
    WriteCmd(0X8D);  //?????
    WriteCmd(0X10);  //?????
    WriteCmd(0XAE);  //OLED??
}


 /**
  * @brief  OLED_ShowStr???codetab.h??ASCII??,?6*8?8*16???
  * @param  x,y : ?????(x:0~127, y:0~7);
    *                   ch[] :- ???????; 
    *                   TextSize : ????(1:6*8 ; 2:8*16)
    * @retval ?
  */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
    unsigned char c = 0,i = 0,j = 0;
    switch(TextSize)
    {
        case 1:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 126)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<6;i++)
                    WriteDat(F6x8[c][i]);
                x += 6;
                j++;
            }
        }break;
        case 2:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 120)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<8;i++)
                    WriteDat(F8X16[c*16+i]);
                OLED_SetPos(x,y+1);
                for(i=0;i<8;i++)
                    WriteDat(F8X16[c*16+i+8]);
                x += 8;
                j++;
            }
        }break;
    }
}

 /**
  * @brief  OLED_ShowCN???codetab.h????,16*16??
  * @param  x,y: ?????(x:0~127, y:0~7); 
    *                   N:???codetab.h????
    * @retval ?
  */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
    unsigned char wm=0;
    unsigned int  adder=32*N;
    OLED_SetPos(x , y);
    for(wm = 0;wm < 16;wm++)
    {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(x,y + 1);
    for(wm = 0;wm < 16;wm++)
    {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
}



 /**
  * @brief  OLED_DrawBMP???BMP??
  * @param  x0,y0 :?????(x0:0~127, y0:0~7);
    *                   x1,y1 : ?????(???)???(x1:1~128,y1:1~8)
    * @retval ?
  */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

  if(y1%8==0)
        y = y1/8;
  else
        y = y1/8 + 1;
    for(y=y0;y<y1;y++)
    {
        OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
        {
            WriteDat(BMP[j++]);
        }
    }
}

int main(int argc, void **argv)
{
	int m = 0;

	if (strcmp("ON", argv[1]) == 0)
		{
			i2c_init();
			OLED_Init();
			OLED_ON();
			
		}
	else if(strcmp("OFF", argv[1]) == 0)
	    
		{
			i2c_init();
			OLED_ON();
		}
	else if(strcmp("CLS", argv[1]) == 0)
		{
			i2c_init();
			if (argc > 2)
			    OLED_Fill(0,atoi(argv[2]) & 0xFF);
			else
				OLED_CLS();
			
		}
	else if(strcmp("FILL", argv[1]) == 0)
		{
			i2c_init();
			if (argc > 3)
				OLED_Fill(atoi(argv[3]) & 0xFF, atoi(argv[2]) & 0xFF);
			else
				for (m = 0; m < 8; m++)
					OLED_Fill(atoi(argv[2]) & 0xFF, m);
			
		}
	else 
		{
			i2c_init();
            		m = 1;
			if  (argc > 3)
			   m = atoi(argv[4]) & 0x3;
			OLED_ShowStr(atoi(argv[1]) & 0xFF, atoi(argv[2]) & 0xFF, argv[3], m);
			  
		}
	return 0;
	
}

