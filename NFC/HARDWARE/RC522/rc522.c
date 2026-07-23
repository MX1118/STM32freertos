#include "RC522.h"
#include "SPI.h"
#include "led.h"   
#include "beep.h"
#include <stdio.h>
#include <string.h>

const uint8_t UID[4] = {0x8E,0xBF,0x51,0x07};
#define CommandReg      0x01
#define FIFODataReg     0x09
#define FIFOLevelReg    0x0A
#define BitFramingReg   0x0D
#define TReloadRegH     0x2C   
#define TxASKReg        0x15
#define ModeReg         0x11
#define TxControlReg    0x14
#define TModeReg        0x2A
#define TPrescalerReg   0x2B
#define TReloadRegL     0x2D
#define CommIrqReg      0x04
#define CommIEnReg      0x02
#define PCD_IDLE        0x00
#define PCD_TRANSCEIVE  0x0C
#define PICC_REQIDL     0x26
#define PICC_ANTICOLL   0x93

void Action_Success(void)
{
    GPIO_ResetBits(GPIOF, GPIO_Pin_10); 
    delay_ms(100); 
    GPIO_SetBits(GPIOF, GPIO_Pin_10);
}

void Action_Error(void)
{
    uint8_t i;
    for(i = 0; i < 2; i++)
    {
        GPIO_ResetBits(GPIOF, GPIO_Pin_9); 
        GPIO_SetBits(GPIOF, GPIO_Pin_8); 
        delay_ms(100);                    
        GPIO_SetBits(GPIOF, GPIO_Pin_9);   
        GPIO_ResetBits(GPIOF, GPIO_Pin_8);   
        delay_ms(100);
    }
}

void RC522_WriteReg(uint8_t addr, uint8_t data)
{
    RC522_CS_L();
    SPI1_ReadWrite((addr << 1) & 0x7E); 
    SPI1_ReadWrite(data);
    RC522_CS_H();
}


 
 uint8_t RC522_ReadReg(uint8_t addr)
{
    uint8_t val;

    RC522_CS_L();


    SPI1_ReadWrite(((addr << 1) & 0x7E) | 0x80);

    val = SPI1_ReadWrite(0x00);


    RC522_CS_H();

    return val;
}


void RC522_Init(void)
{
    
    uint8_t temp;
    uint8_t version; 
    GPIO_InitTypeDef GPIO_InitStructure;
    

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RC522_CS_H();
    
    RC522_RST_H();
    RC522_RST_L();    
    delay_ms(50);      
    RC522_RST_H();     
    delay_ms(50);
    
    RC522_WriteReg(TModeReg, 0x8D);
    RC522_WriteReg(TPrescalerReg, 0x3E);
    RC522_WriteReg(TReloadRegL, 0x30);
    RC522_WriteReg(TReloadRegH, 0x00);
    RC522_WriteReg(TxASKReg, 0x40);
    RC522_WriteReg(ModeReg, 0x3D);
    
    temp = RC522_ReadReg(TxControlReg);
    if (!(temp & 0x03))
   {RC522_WriteReg(TxControlReg, temp | 0x03);}
	 printf("TxControl=%02X\r\n",RC522_ReadReg(TxControlReg));
   
    version = RC522_ReadReg(0x37); 
    printf("RC522 Chip Version: 0x%02X\r\n", version);
}
uint8_t RC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint32_t *backLen)
{
    uint8_t status = 1;
    uint8_t irqEn = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint32_t i;

    if (command == PCD_TRANSCEIVE)
    {
        irqEn = 0x77;
        waitIRq = 0x30;
    }
    RC522_WriteReg(CommIEnReg, irqEn | 0x80);
    RC522_WriteReg(CommIrqReg, 0x7F); 
    RC522_WriteReg(CommandReg, PCD_IDLE);
    RC522_WriteReg(FIFOLevelReg, 0x80);

    for (i = 0; i < sendLen; i++)
    {
        RC522_WriteReg(FIFODataReg, sendData[i]);
    }
    RC522_WriteReg(CommandReg, command);

    if (command == PCD_TRANSCEIVE)
    {
        RC522_WriteReg(BitFramingReg, RC522_ReadReg(BitFramingReg) | 0x80); 
    }

    i = 2000; 
    do
    {
        n = RC522_ReadReg(CommIrqReg);
        i--;
        delay_us(10); 
    } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

    RC522_WriteReg(BitFramingReg, RC522_ReadReg(BitFramingReg) & 0x7F); 

    if (i != 0)
    {
        if (!(RC522_ReadReg(0x06) & 0x1B)) 
        {
            status = 0;
            if (n & irqEn & 0x01) status = 1;
            
            if (command == PCD_TRANSCEIVE)
            {
                n = RC522_ReadReg(FIFOLevelReg);
                lastBits = RC522_ReadReg(BitFramingReg) & 0x07;
                if (lastBits) *backLen = (n - 1) * 8 + lastBits;
                else *backLen = n * 8;

                if (n == 0) n = 1;
                if (n > 16) n = 16;

                for (i = 0; i < n; i++)
                {
                    backData[i] = RC522_ReadReg(FIFODataReg);
                }
            }
        }
    }
    return status;
}

uint8_t RC522_CheckCard(uint8_t *uid)
{
    uint8_t status;
	uint8_t req_data[1];
    uint8_t card_uid[5];
    uint32_t backBits;

  req_data[0]=PICC_REQIDL;

    status=RC522_ToCard(
        PCD_TRANSCEIVE,
        card_uid,
        1,
        card_uid,
        &backBits
    );


    if(status!=0)
        return 0;

        card_uid[0]=PICC_ANTICOLL;
    card_uid[1]=0x20;


    status=RC522_ToCard(
        PCD_TRANSCEIVE,
        card_uid,
        2,
        card_uid,
        &backBits
    );



    if(status==0)
    {

        uid[0]=card_uid[0];
        uid[1]=card_uid[1];
        uid[2]=card_uid[2];
        uid[3]=card_uid[3];


        printf("UID:%02X %02X %02X %02X\r\n",
        uid[0],
        uid[1],
        uid[2],
        uid[3]);


        return 1;
    }


    return 0;

}

