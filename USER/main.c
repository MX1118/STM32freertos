#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "led.h"
#include "beep.h"
#include "key.h"
#include "spi.h"

#include "pwm.h"
#include "rc522.h"

#include <stdio.h>


#define PASSWORD_LEN 4


// ÃÜÂë 1111
u8 CORRECT_PASSWORD[PASSWORD_LEN] =
{
    1,1,1,1
};



void Access_Granted(void);
void Access_Denied(void);



int main(void)
{

    u8 key_val = 0;

    u8 input_pwd[PASSWORD_LEN] = {0};

    u8 pwd_id = 0;

    u8 i;

    u8 pwd_correct;


    uint8_t uid[4];


    uint16_t card_check_counter = 0;



    /*
    ======================
        ³õÊ¼»¯
    ======================
    */


    delay_init(168);


    uart_init(115200);



    LED_Init();

    BEEP_Init();

    KEY_Init();



    SPI1_Init();


    RC522_Init();



    TIM4_PWM_Init(19999,83);



    Servo_Close();


    BEEP = 0;



    printf("SYSTEM START\r\n");




    while(1)
    {



        /*
        ======================
             RFIDË¢¿¨
        ======================
        */


        card_check_counter++;


        if(card_check_counter >= 20)
        {

            card_check_counter = 0;



            if(RC522_CheckCard(uid) == 1)
            {


                printf("CARD FOUND\r\n");


                printf("UID:%02X %02X %02X %02X\r\n",
                       uid[0],
                       uid[1],
                       uid[2],
                       uid[3]);




                /*
                    °×Ãûµ¥ÅÐ¶Ï
                */


                if(uid[0] == 0x8E &&
                   uid[1] == 0xBF &&
                   uid[2] == 0x51 &&
                   uid[3] == 0x07)
                {


                    printf("ACCESS GRANTED\r\n");


                    Action_Success();


                    Access_Granted();


                }

                else
                {


                    printf("ACCESS DENIED\r\n");


                    Action_Error();


                    Access_Denied();


                }



                // µÈ´ý¿¨Àë¿ª

                while(RC522_CheckCard(uid))
                {

                    delay_ms(100);

                }



            }


        }





        /*
        ======================
             ¾ØÕó¼üÅÌ
        ======================
        */


        key_val = KEY_Scan();



        if(key_val != 0)
        {


            BEEP = 1;

            delay_ms(30);

            BEEP = 0;



            if(key_val >=1 && key_val <=16)
            {


                if(pwd_id < PASSWORD_LEN)
                {

                    input_pwd[pwd_id] = key_val;

                    pwd_id++;

                }


            }




            if(pwd_id == PASSWORD_LEN)
            {


                pwd_correct = 1;



                for(i=0;i<PASSWORD_LEN;i++)
                {

                    if(input_pwd[i] != CORRECT_PASSWORD[i])
                    {

                        pwd_correct = 0;

                        break;

                    }

                }





                if(pwd_correct)
                {


                    printf("PASSWORD OK\r\n");


                    Action_Success();


                    Access_Granted();


                }

                else
                {


                    printf("PASSWORD ERROR\r\n");


                    Action_Error();


                    Access_Denied();


                }





                pwd_id = 0;



                for(i=0;i<PASSWORD_LEN;i++)
                {

                    input_pwd[i]=0;

                }



            }



        }



        delay_ms(5);



    }



}






/*
========================
      ¿ªÃÅ
========================
*/


void Access_Granted(void)
{


    Servo_Open();


    delay_ms(3000);


    Servo_Close();



}





/*
========================
      ¾Ü¾ø
========================
*/


void Access_Denied(void)
{


    Servo_Close();



}