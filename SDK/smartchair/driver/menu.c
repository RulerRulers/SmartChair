#include <STC12C5A60S2.H>
#include "menu.h"
#include "menufun.h"

KbdTabStruct code Key[MENU_SIZE] = 
{ 
	  //��ǰ����  ��һ����  ��һ����  ȷ����������  ���ؽ�������  ��ǰ��������
   {0,0,0,0,0,(*MainWin)},//������
   {1,3,2,4,0,(*pressureMenu)},//ѹ����ֵ����ѡ��
   {2,3,1,5,0,(*DistanceMenu)},//������ֵ����ѡ��
   {3,2,1,6,0,(*DisDiffMenu)},//�������ֵ����ѡ��

   {4,3,3,0,1,(*ChangePressure)},//����ѹ����ֵ
   {5,4,4,0,2,(*ChangeDistance)},//����������ֵ
   {6,3,3,0,3,(*changeDisDiff)} 


};






