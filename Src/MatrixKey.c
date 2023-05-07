/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2022,--,China, CUIT.
  *                            All Rights Reserved
  *                              
  *                     By(JCML)
    *
  *    
  * FileName   : MatrixKey.c   
  * Version    : v1.0		
  * Author     : JCML			
  * Date       : 2022-12-24         
  * Description:    利用Timer0对按键进行扫描
  * May Be Used Function List:  
1. Head file￡o 
#include "MatrixKey.h" 

2. Function File￡o
Counter1++;
if (Counter1 >= 10)
{
	Counter1 = 0;
	MatrixKey_Loop();
}

KeyNum=MatrixKey();
******************************************************************************
 */

#include <REGX52.H>
#include "sys.h"

u8 KeyNumber;

/***********************************************************
*@fuction	:MatrixKey_GetState
*@brief		:检测按键是否按下
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-24
***********************************************************/

u8 MatrixKey_GetState()
{
	unsigned char KeyNum = 0;
	
	P3=0xFF;
	P3_4=0;
	if(P3_0==0){KeyNum=1;}
	if(P3_1==0){KeyNum=5;}
	if(P3_2==0){KeyNum=9;}
	if(P3_3==0){KeyNum=13;}

	P3=0xFF;
	P3_5=0;
	if(P3_0==0){KeyNum=2;}
	if(P3_1==0){KeyNum=6;}
	if(P3_2==0){KeyNum=10;}
	if(P3_3==0){KeyNum=14;}
	
	P3=0xFF;
	P3_6=0;
	if(P3_0==0){KeyNum=3;}
	if(P3_1==0){KeyNum=7;}
	if(P3_2==0){KeyNum=11;}
	if(P3_3==0){KeyNum=15;}
	
	P3=0xFF;
	P3_7=0;
	if(P3_0==0){KeyNum=4;}
	if(P3_1==0){KeyNum=8;}
	if(P3_2==0){KeyNum=12;}
	if(P3_3==0){KeyNum=16;}
	P3=0xFF;
	return KeyNum;
}

/***********************************************************
*@fuction	:MatrixKey
*@brief		:返回按下的按键值
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-24
***********************************************************/

u8 MatrixKey(void)
{
	u8 Temp = 0;
	Temp = KeyNumber;
	KeyNumber = 0;
	return Temp;
}


/***********************************************************
*@fuction	:MatrixKey_Loop
*@brief		:置于Timer0 的循环扫描函数
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-24
***********************************************************/

void MatrixKey_Loop(void)
{
	static u8 NewState, LastState;
	LastState = NewState;
	NewState = MatrixKey_GetState();
	if(LastState == 1 && NewState == 0){KeyNumber = 1;}
	if(LastState == 2 && NewState == 0){KeyNumber = 2;}
	if(LastState == 3 && NewState == 0){KeyNumber = 3;}
	if(LastState == 4 && NewState == 0){KeyNumber = 4;}
	if(LastState == 5 && NewState == 0){KeyNumber = 5;}
	if(LastState == 6 && NewState == 0){KeyNumber = 6;}
	if(LastState == 7 && NewState == 0){KeyNumber = 7;}
	if(LastState == 8 && NewState == 0){KeyNumber = 8;}
	if(LastState == 9 && NewState == 0){KeyNumber = 9;}
	if(LastState == 10 && NewState == 0){KeyNumber = 10;}
	if(LastState == 11 && NewState == 0){KeyNumber = 11;}
	if(LastState == 12 && NewState == 0){KeyNumber = 12;}
	if(LastState == 13 && NewState == 0){KeyNumber = 13;}
	if(LastState == 14 && NewState == 0){KeyNumber = 14;}
	if(LastState == 15 && NewState == 0){KeyNumber = 15;}
	if(LastState == 16 && NewState == 0){KeyNumber = 16;}
}










