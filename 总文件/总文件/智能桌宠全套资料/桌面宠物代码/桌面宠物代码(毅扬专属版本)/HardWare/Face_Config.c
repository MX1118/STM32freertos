#include "stm32f10x.h"      
#include "OLED.h"
#include "BlueTooth.h"
#include "Variable.h"

void Face_RequestRefresh(void)
{
	Face_Dirty = 1;
	Face_RefreshReq = 1;
}

//实现表情变化，调节是进中断后
void Face_Config(void)
{
	uint16_t CurrentFaceMode = Face_Mode;
	if (!Face_Dirty && !Face_RefreshReq && Face_LastMode == CurrentFaceMode)
	{
		return;
	}

	/*图案处理*/
	switch(CurrentFaceMode)
	{
		case 0:
	    OLED_Clear();
	    OLED_ShowImage(0,0,128,64,Face_mambo);//曼波
		  break;
		case 1:
			OLED_Clear();
		  OLED_ShowImage(0,0,128,64,Face_stare);//瞪大眼
		  break;
		case 2:
			OLED_Clear();
	  	OLED_ShowImage(0,0,128,64,Face_happy);//快乐
		  break;
	  case 3:
			OLED_Clear();
	  	OLED_ShowImage(0,0,128,64,Face_mania);//狂热
		  break;
		case 4:
			OLED_Clear();
	  	OLED_ShowImage(0,0,128,64,Face_very_happy);//非常快乐
		  break;
		case 5:
			OLED_Clear();
	  	OLED_ShowImage(0,0,128,64,Face_eyes);//眼睛
		 break;
		case 6:
			OLED_Clear();
	  	OLED_ShowImage(0,0,128,64,Face_hello);//打招呼
			break;
	}
	

//		OLED_Clear();

//    OLED_Printf(0,0,OLED_6X8,"Mode:%d",Action_Mode);	
//		OLED_Printf(80,0,OLED_6X8,"RX:%X",RxData_Voice);
//		

	/*显示图案*/
		OLED_Update();
	Face_LastMode = CurrentFaceMode;
	Face_Dirty = 0;
	Face_RefreshReq = 0;
}
