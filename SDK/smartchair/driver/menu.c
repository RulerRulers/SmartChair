#include <STC12C5A60S2.H>
#include "menu.h"
#include "menufun.h"

KbdTabStruct code Key[MENU_SIZE] = 
{ 
	  //��ǰ����  ��һ����  ��һ����  ȷ����������  ���ؽ�������  ��ǰ��������
   {0,0,0,0,0,(*MainWin)},//������
   {1,2,2,3,0,(*pressureMenu)},//ѹ����ֵ����ѡ��
   {2,1,1,4,0,(*DistanceMenu)},//������ֵ����ѡ��
    
   {3,3,3,0,1,(*ChangePressure)},//����ѹ����ֵ
   {4,4,4,0,2,(*ChangeDistance)},//����������ֵ



};






