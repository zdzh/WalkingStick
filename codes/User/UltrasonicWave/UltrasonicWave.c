/******************** (C) 1209 Lab **************************
 * �ļ���  : UltrasonicWave.c
 * ����    �����������ģ�飬UltrasonicWave_Configuration��������
             ��ʼ������ģ�飬UltrasonicWave_StartMeasure��������
			 ������࣬������õ�����ͨ������1��ӡ����         
 * ʵ��ƽ̨��Mini STM32������  STM32F103RBT6
 * Ӳ�����ӣ�------------------
 *          | PC8  - TRIG      |
 *          | PC7  - ECHO      |
 *           ------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��Lee 
*********************************************************************************/

#include "UltrasonicWave.h"





static void UltrasonicWave_StartMeasure(GPIO_TypeDef *  port, int32_t pin);              

float UltrasonicWave_Distance[AVER_NUM];      //������ľ���    
static int8_t index = 0;
static int16_t MAX_DISTACE = 150;        //������
int8_t  IT_TAG = 0;          //��ȡ��־��Ϊ1ʱ��ʾ�Զ�ȡ������

/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN1;					
	GPIO_Init(TRIG_PORT1, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN2;					
	GPIO_Init(TRIG_PORT2, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN3;					
	GPIO_Init(TRIG_PORT3, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN4;					
	GPIO_Init(TRIG_PORT4, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN5;					
	GPIO_Init(TRIG_PORT5, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN6;					
	GPIO_Init(TRIG_PORT6, &GPIO_InitStructure);			
}



/*
 * ��������dealTIM_ICUserValueStructureData
 * ����  ������׽����ʱ������ת��Ϊ����
 * ����  ��TIM_ICUserValueStructurex ��TIM_ICUserValueStructure1-6
			i����Ӧ���������
 * ���  ����	
 */
void dealTIM_ICUserValueStructureData(TIM_ICUserValueTypeDef TIM_ICUserValueStructurex, int i)
{

	uint32_t time;
	double ftime;
	// ����ߵ�ƽʱ��ļ�������ֵ
	time = TIM_ICUserValueStructurex.Capture_CcrValue+1;
	// ��ӡ�ߵ�ƽ����ʱ��
	ftime = ((double)time)/TIM_PscCLK;
	UltrasonicWave_Distance[i] = ftime * 340 / 2  * 100;
//	printf( "\r\n time %d\r\n",time );	
//	printf( "\r\n ftime %lf\r\n",ftime );
	printf( "\r\n%d : distance %f\r\n",i, UltrasonicWave_Distance[i]);
//	printf( "\r\n��%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );	
}



/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ��port = TRIG_PORTX ,pin = TRIG_PINX
 * ���  ����	
 */
void UltrasonicWave_StartMeasure(GPIO_TypeDef *  port, int32_t pin)
{
  GPIO_SetBits(port,pin); 		  //��>10US�ĸߵ�ƽ�TRIG_PORT,TRIG_PIN��������define����?
  delayUs(11);		                      //��ʱ20US
  GPIO_ResetBits(port,pin);

}


/*
 * ��������isObase
 * ����  ���ж�ͬ�鳬�����Ƿ�����ϰ���
 * ����  ��ͬ��i, j���������
 * ���  ��0 ���ϰ�� 1 iǰ�����ϰ�� 2 jǰ�����ϰ�� 3 i��jǰ�������ϰ���
 */
int isObase(int i, int j)
{
	int num = 0;
	if( UltrasonicWave_Distance[i] < MAX_DISTACE)
	{
		num += 1;
	}	
	if( UltrasonicWave_Distance[j] < MAX_DISTACE)
	{
		num += 2;
	}
	return num;
}
/*
 * ��������hasObase
 * ����  ���ж��Ƿ�����ϰ���
 * ����  �� num��i����¼��Ӧ������ģ���ϰ��������0 ���ϰ�� 1 iǰ�����ϰ�� 2 jǰ�����ϰ�� 3 i��jǰ�������ϰ���
 * ���  ����
 */
void hasObase(int* num)
{
	int i;
	for( i = 0; i < AVER_NUM; i+=2 )
	{
		num[i/2] = isObase(i, i+1);
	}

}


/*
 * ��������addDistance
 * ����  �����Ӽ����룬��󲻳���250����
 * ����  ��num ��ⷶΧ����num����
 * ���  ���޸ĺ�ļ�ⷶΧ
 */
int addDistance(int distance)
{
	MAX_DISTACE += distance;
	if( MAX_DISTACE > 250 )
		MAX_DISTACE = 250;
	return MAX_DISTACE;
}


/*
 * ��������addDistance
 * ����  ����С�����룬��󲻳���250����
 * ����  ��distance ��ⷶΧ��Сdistanc����
 * ���  ���޸ĺ�ļ�ⷶΧ
 */
int minusDistance(int distance)
{
	MAX_DISTACE -= distance;
	if( MAX_DISTACE <30 )
		MAX_DISTACE = 30;
	return MAX_DISTACE;
}

/*
 * ��������getDistance
 * ����  ����õ�ǰ��ⷶΧ
 * ����  ����
 * ���  ����ǰ�ļ�ⷶΧ
 */
int getDistance()
{
	return MAX_DISTACE;
}


/****************************************************************************
* ��    �ƣ�void UltrasonicWave(void *arg)
* ��    �ܣ�����������߳�
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void UltrasonicWave(int* num)
{
	static int8_t tag = 0;
	int i= 0;	
	hasObase(num);
	if( TIM_ICUserValueStructure.Capture_FinishFlag == 1 )  
	{
		TIM_ICUserValueStructure.Capture_FinishFlag = 0;
	    dealTIM_ICUserValueStructureData(TIM_ICUserValueStructure, tag);
	}
	else if( TIM_ICUserValueStructure2.Capture_FinishFlag == 1 )  
	{
		TIM_ICUserValueStructure2.Capture_FinishFlag = 0;
	    dealTIM_ICUserValueStructureData(TIM_ICUserValueStructure2, tag);
	}	
	switch(tag)          //��ʼ��࣬����һ��>10us�����壬
	{
		case 0: UltrasonicWave_StartMeasure(TRIG_PORT1,TRIG_PIN1); break;
		case 1: UltrasonicWave_StartMeasure(TRIG_PORT2,TRIG_PIN2); break;
		case 2: UltrasonicWave_StartMeasure(TRIG_PORT3,TRIG_PIN3); break;
		case 3: UltrasonicWave_StartMeasure(TRIG_PORT4,TRIG_PIN4); break;
		case 4: UltrasonicWave_StartMeasure(TRIG_PORT5,TRIG_PIN5); break;
		case 5: UltrasonicWave_StartMeasure(TRIG_PORT6,TRIG_PIN6); break;
	}
	tag = (tag +1) % AVER_NUM;
//	printf("\r\n tag : %d \r\n", tag);
	
}


/******************* (C) 1209 Lab *****END OF FILE************/