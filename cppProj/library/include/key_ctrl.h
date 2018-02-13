#ifndef _KEY_CTRL_H_
#define _KEY_CTRL_H_
#include "fifo_ctrl.h"
#include "property.h"
#include "timer_ctrl.h"
#include "utils.h"

#define APP_KEY_DEPTH	4

#define SPEICAL_KEY_IDENTIFY_MS	2

typedef struct{	
	char key;
	char ascii;
}KeyCode;

typedef void(*TOnKeyValidEvent)(char &, char &);

/*
 * Class name : KeyCtrl
 * Description: Generic Standard input controller
 * Usage :
 *      KeyCtrl *keyIn;							//declare a key controller
 * 		keyIn = new KeyCtrl(4, &tmr);			//create key fifo controller with depth 4, and give parent used timer
 * 		keyIn->on_key_valid = when_key_press;	//assert on key valid service
 * 		while(1)
 * 		{
 * 			...
 * 			keyIn->routine();				//put key controller main routine to while 1 loop
 *	 	}
 */
class KeyCtrl
{
private:
	class KeyCore : public FIFOCtrl<KeyCode>{
	public:
	static bool bIdentifying;		//controller is identifying control code
	static int scanKeyCode;
	static FIFOCtrl<unsigned char> scFIFO;
	static void key_code_determine(char code, char ascii);
	static void decode_sc(void);
	static int kbhit(void);
	KeyCore(unsigned int);
	~KeyCore(void);
	};
	static KeyCore core;
	bool _f_check_key_empty(void);
			
protected:
public:	
	static void routine(void);
	static TOnKeyValidEvent on_key_valid;
	//operator
	KeyCtrl& operator >> (KeyCode &);
	KeyCtrl& operator << (KeyCode);
	//useful properties
	property_ro<KeyCtrl, bool> bEmpty;	
	//constructor
	KeyCtrl(void);
	//destructor
	//~KeyCtrl(void);
};

//key definition
//normal key 0~9 + A~F
#define NORMAL_KEY		(0<<5)
#define HOT_KEY			(1<<5)
#define CONTROL_KEY		(2<<5)
#define NXGN_KEY		(3<<5)

#define KEY_NUM(arg)	(NORMAL_KEY|(arg&0x0F))
#define KEY_0			KEY_NUM('0')
#define KEY_1			KEY_NUM('1')
#define KEY_2			KEY_NUM('2')
#define KEY_3			KEY_NUM('3')
#define KEY_4			KEY_NUM('4')
#define KEY_5			KEY_NUM('5')
#define KEY_6			KEY_NUM('6')
#define KEY_7			KEY_NUM('7')
#define KEY_8			KEY_NUM('8')
#define KEY_9			KEY_NUM('9')

#define KEY_ALPHA(arg)	((NORMAL_KEY|(arg&0x1F))+9)
#define KEY_A			KEY_ALPHA('A')
#define KEY_B			KEY_ALPHA('B')
#define KEY_C			KEY_ALPHA('C')
#define KEY_D			KEY_ALPHA('D')
#define KEY_E			KEY_ALPHA('E')
#define KEY_F			KEY_ALPHA('F')
#define KEY_G			KEY_ALPHA('G')
#define KEY_H			KEY_ALPHA('H')
#define KEY_I			KEY_ALPHA('I')
#define KEY_J			KEY_ALPHA('J')
#define KEY_K			KEY_ALPHA('K')
#define KEY_L			KEY_ALPHA('L')
#define KEY_M			KEY_ALPHA('M')
#define KEY_N			KEY_ALPHA('N')
#define KEY_O			KEY_ALPHA('O')
#define KEY_P			KEY_ALPHA('P')
#define KEY_Q			KEY_ALPHA('Q')
#define KEY_R			KEY_ALPHA('R')
#define KEY_S			KEY_ALPHA('S')
#define KEY_T			KEY_ALPHA('T')
#define KEY_U			KEY_ALPHA('U')
#define KEY_V			KEY_ALPHA('V')
#define KEY_W			KEY_ALPHA('W')
#define KEY_X			KEY_ALPHA('X')
#define KEY_Y			KEY_ALPHA('Y')
#define KEY_Z			KEY_ALPHA('Z')

//Hot Key
#define KEY_ESC			(HOT_KEY|0x00)
#define KEY_F1			(HOT_KEY|0x01)
#define KEY_F2			(HOT_KEY|0x02)
#define KEY_F3			(HOT_KEY|0x03)
#define KEY_F4			(HOT_KEY|0x04)
#define KEY_F5			(HOT_KEY|0x05)
#define KEY_F6			(HOT_KEY|0x06)
#define KEY_F7			(HOT_KEY|0x07)
#define KEY_F8			(HOT_KEY|0x08)
#define KEY_F9			(HOT_KEY|0x09)
#define KEY_F10			(HOT_KEY|0x0A)
#define KEY_F11			(HOT_KEY|0x0B)
#define KEY_F12			(HOT_KEY|0x0C)

//Control Key
#define KEY_UP			(CONTROL_KEY|0x00)
#define KEY_DOWN		(CONTROL_KEY|0x01)
#define KEY_LEFT		(CONTROL_KEY|0x02)
#define KEY_RIGHT		(CONTROL_KEY|0x03)
#define KEY_BACK		(CONTROL_KEY|0x04)
#define KEY_TAB			(CONTROL_KEY|0x05)
#define KEY_ENTER		(CONTROL_KEY|0x06)
#define KEY_HOME		(CONTROL_KEY|0x07)
#define KEY_END			(CONTROL_KEY|0x08)
#define KEY_DEL			(CONTROL_KEY|0x09)
#define KEY_PAGE_UP		(CONTROL_KEY|0x0A)
#define KEY_PAGE_DOWN	(CONTROL_KEY|0x0B)
#define KEY_INSERT		(CONTROL_KEY|0x0C)


#define KEY_COMPLETE	(NXGN_KEY|0x00)
#define KEY_ERROR		(NXGN_KEY|0x01)
//#define KEY_DATA_ABORT	(NXGN_KEY|0x02)
#define KEY_CANCEL		(NXGN_KEY|0x03)
#define KEY_NOT_EXIST	(NXGN_KEY|0x04)

#define KEY_UNDEFINE	(NXGN_KEY|0xFF)


#define KEY_NOT_ALLOW	0xFF



#endif //ndef _KEY_CTRL_H_
