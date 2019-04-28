#include "includes.h"

QUEUE infoCmd;     //�����͵�����
HOST  infoHost;    //������״̬

MENU infoMenu;     //�˵��ṹ��

void Hardware_Config(void)
{
  Delay_init(72);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  OS_TimerInit(9999,71);								//ϵͳʱ�� 10ms
  XPT2046_Init();
  W25Qxx_Init();
  LCD_Init();
  LCD_DMA_Config();

  if(readStoredPara() == false)
  {    
    TSC_Calibration();
    infoSettings.baudrate = 115200;
    infoSettings.language = ENGLISH;
    storePara();
  }
  LOGO_ReadDisplay();
  
  USART1_Config(infoSettings.baudrate);

  infoMenu.cur = 0;
  infoMenu.menu[infoMenu.cur] = menuMain;

  u32 startUpTime = OS_GetTime();
  heatSetUpdateTime(100);
  while(OS_GetTime() - startUpTime < 1500)                //Startup-delay 15sec
  {                                                                                                                     
    loopProcess();	
  }
  heatSetUpdateTime(300);
}


int main(void)
{	
  SCB->VTOR = FLASH_BASE|0x6000;

  Hardware_Config();
  
  for(;;)
  {
    (*infoMenu.menu[infoMenu.cur])();
  }
}
