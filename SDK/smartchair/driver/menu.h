#ifndef _MENU_H
#define _MENU_H

#define MENU_SIZE 20

typedef struct
{
	unsigned char KeyStateIndex;// 当前状态索引号
	unsigned char KeyLeftState;// 按下“向左”键时转向的状态索引号
	unsigned char KeyRightState;// 按下“向右”键时转向的状态索引号
	unsigned char KeyEntertState;// 按下“回车”键时转向的状态索引号
	unsigned char KeyBackState;// 按下“退回”键时转向的状态索引号
	void (*CurrentOperate)(); // 当前状态应该执行的功能操作
}KbdTabStruct;

extern KbdTabStruct code Key[MENU_SIZE];




#endif

