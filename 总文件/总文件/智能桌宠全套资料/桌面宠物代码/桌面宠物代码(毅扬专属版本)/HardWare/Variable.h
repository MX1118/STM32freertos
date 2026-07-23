#ifndef __VARIABLE_H_
#define __VARIABLE_H_


/*动作相关*/
#define Chongfunumber 2          //动作重复次数、前进后退左转右转
#define SwingRepeatnumber 3      //摇摆重复次数
#define HelloRepeatnumber 4      //打招呼重复次数
extern uint16_t PAnumbers;       //动作重复次数
extern uint16_t TiaoTurn;        //向前跳
extern uint16_t TiaoTurn2;       //向后跳
extern volatile uint16_t Action_Mode;     //动作模式
extern volatile uint16_t SpeedDelay;      //运动速度
extern volatile uint16_t SwingDelay;      //摇摆速度
extern volatile uint16_t Face_Mode;       //表情切换
extern volatile uint8_t Face_Dirty;       //表情待刷新标志
extern volatile uint8_t Face_RefreshReq;  //表情刷新请求
extern uint16_t Face_LastMode;            //上次已显示的表情
extern volatile uint8_t WeiBa_Bit;        //摇尾巴判断
extern uint8_t WeiBa_Value;               //摇尾巴的compare值
extern int8_t WeiBa_Dir;                  //摇尾巴方向判断
extern volatile uint16_t Sustainedmove;   //持续运动
extern volatile uint16_t RxData_Voice;    //语音串口接收数据变量
extern volatile uint16_t RxData_Blue;     //蓝牙串口接收数据变量


/*呼吸灯相关*/
extern uint16_t Time;            //呼吸灯间隔时间
extern uint16_t HuXi;            //呼吸灯输出脉冲数
extern uint16_t PanDuan;         //呼吸灯模式
extern uint16_t Wait;            //间隔时间
extern volatile uint16_t AllLed;      //开启灯光
extern volatile uint16_t BreatheLed;   //开启呼吸灯

#endif
