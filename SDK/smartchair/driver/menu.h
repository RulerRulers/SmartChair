#ifndef _MENU_H
#define _MENU_H

#define MENU_SIZE 20

typedef struct
{
	unsigned char KeyStateIndex;// ��ǰ״̬������
	unsigned char KeyLeftState;// ���¡����󡱼�ʱת���״̬������
	unsigned char KeyRightState;// ���¡����ҡ���ʱת���״̬������
	unsigned char KeyEntertState;// ���¡��س�����ʱת���״̬������
	unsigned char KeyBackState;// ���¡��˻ء���ʱת���״̬������
	void (*CurrentOperate)(); // ��ǰ״̬Ӧ��ִ�еĹ��ܲ���
}KbdTabStruct;

extern KbdTabStruct code Key[MENU_SIZE];




#endif

