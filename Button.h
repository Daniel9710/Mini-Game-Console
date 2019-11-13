#ifndef BUTTON_H_
#define BUTTON_H_

#define arrow_left !(PINB&0x01)
#define arrow_up !(PINB&0x02)
#define arrow_right !(PINB&0x04)
#define arrow_down !(PINB&0x08)
#define push_button1 !(PIND&0x20)
#define push_button2 !(PIND&0x40)

void Button_Event_0(int cs);
void Button_Event_1(int cs);
void Button_Event_2(int cs);
void Button_Event_3(int cs);
void Button_Event_4(int cs);
void Button_Event_5(int cs);


#define Button_Check(_key,_num,_mode)												\
	if(_key)																									\
	{																													\
		if(button_del[_num] == 0 || button_del[_num] >= 4)			\
		{																												\
			button_del[_num] = 1;																	\
			Button_Event_##_num(_mode);														\
		}																												\
		else																										\
			button_del[_num]++;																		\
	}																													\
	else																											\
		button_del[_num] = 0;		

#endif
