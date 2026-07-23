/*在这里定义所有的变量，并允许外部引用*/
#include "stm32f10x.h"                  // Device header
#include "Variable.h"


/*动作相关*/
uint16_t PAnumbers=Chongfunumber;//动作重复次数
uint16_t TiaoTurn=0;             //向前跳
uint16_t TiaoTurn2=0;            //向后跳
volatile uint16_t Action_Mode=0;   //动作模式,上电后是放松趴下状态
volatile uint16_t SpeedDelay=200;  //运动速度
volatile uint16_t SwingDelay=6;    //摇摆速度
volatile uint16_t Face_Mode=0;     //表情切换，上电是睡觉表情
volatile uint8_t Face_Dirty=0;     //表情待刷新标志
volatile uint8_t Face_RefreshReq=0;//表情刷新请求
uint16_t Face_LastMode=0;          //上次已显示的表情
volatile uint8_t WeiBa_Bit=0;      //摇尾巴判断
uint8_t WeiBa_Value=90;            //摇尾巴的compare值
int8_t WeiBa_Dir=1;                //摇尾巴方向判断
volatile uint16_t Sustainedmove=0; //持续运动
volatile uint16_t RxData_Voice=0;  //语音串口接收数据变量
volatile uint16_t RxData_Blue=0;   //蓝牙串口接收数据变量


/*呼吸灯相关*/
uint16_t Time;            //呼吸灯间隔时间
uint16_t HuXi;            //呼吸灯输出脉冲数
uint16_t PanDuan=1;       //呼吸灯模式
uint16_t Wait=0;          //间隔时间
volatile uint16_t AllLed=1;      //开启灯光
volatile uint16_t BreatheLed=0;   //开启呼吸灯
