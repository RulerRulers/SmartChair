#include <STC12C5A60S2.H>
#include "menu.h"
#include "menufun.h"

KbdTabStruct code Key[MENU_SIZE] = 
{ 
	  //当前索引  上一索引  下一索引  确定进入索引  返回进入索引  当前操作函数
   {0,0,0,0,0,(*MainWin)},//主界面
   {1,3,2,4,0,(*pressureMenu)},//压力阈值设置选项
   {2,3,1,5,0,(*DistanceMenu)},//距离阈值设置选项
   {3,2,1,6,0,(*DisDiffMenu)},//距离差阈值设置选项

   {4,3,3,0,1,(*ChangePressure)},//调整压力阈值
   {5,4,4,0,2,(*ChangeDistance)},//调整距离阈值
   {6,3,3,0,3,(*changeDisDiff)} 


};






