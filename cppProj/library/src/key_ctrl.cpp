#include <sys/ioctl.h>
#include <unistd.h>
#include "scan_code.h"
#include "key_ctrl.h"

bool KeyCtrl::KeyCore::bIdentifying=false;		//controller is identifying control code
int KeyCtrl::KeyCore::scanKeyCode=0;
FIFOCtrl<unsigned char> KeyCtrl::KeyCore::scFIFO(8);
class KeyCtrl::KeyCore KeyCtrl::core(APP_KEY_DEPTH);

TOnKeyValidEvent KeyCtrl::on_key_valid=NULL;

KeyCtrl::KeyCore::KeyCore(unsigned int depth) : FIFOCtrl<KeyCode>(depth)
{	
}

KeyCtrl::KeyCore::~KeyCore(void)
{	
}

KeyCtrl::KeyCtrl(void) : bEmpty(this, &KeyCtrl::_f_check_key_empty)
{
}

bool 
KeyCtrl::_f_check_key_empty(void)
{
	return core.bEmpty;
}

KeyCtrl& 
KeyCtrl::operator >> (KeyCode &c)
{
	core >> c;
	return *this;
}

KeyCtrl& 
KeyCtrl::operator << (KeyCode c)
{
	core.key_code_determine(c.key, c.ascii);
	return *this;
}

void 
KeyCtrl::KeyCore::key_code_determine(char code, char ascii)
{
	KeyCode c;
	c.key = code;
	c.ascii = ascii;

	KeyCtrl::core << (KeyCode){ code, ascii };//c;
	
	if(on_key_valid != NULL)
	{
		on_key_valid(code, ascii);
		//if set event mode, need assume user have deal this key code already
		//so ,drop it
		KeyCtrl::core >> c;
	}
}

void 
KeyCtrl::KeyCore::decode_sc(void)
{
	static int idxTmrSlot;
	unsigned char ch=0;
	unsigned char key;
	//key decode
	
	if(!scFIFO.bEmpty)
	{
		scFIFO >> ch;
		
		if(bIdentifying)
		{//identifying special key
			scanKeyCode <<= 8;
			scanKeyCode |= ch;
			if(   (ch == SC_SPECIAL_END)
				||((ch >= (SC_UP&0xFF))&&(ch <= (SC_LEFT&0xFF)))
				||((ch >= (SC_F2&0xFF))&&(ch <= (SC_F4&0xFF)))
				||(ch == (SC_HOME&0xFF))
				||(ch == (SC_END&0xFF))
				)
			{// verify a virtual key
				
				switch(scanKeyCode)
				{
					
					case SC_F2:
						key = KEY_F2;
						break;
					case SC_F3:
						key = KEY_F3;
						break;
					case SC_F4:
						key = KEY_F4;
						break;
					case SC_F5:
						key = KEY_F5;
						break;
					case SC_F6:
						key = KEY_F6;
						break;
					case SC_F7:
						key = KEY_F7;
						break;
					case SC_F8:
						key = KEY_F8;
						break;
					case SC_F9:
						key = KEY_F9;
						break;
					case SC_F12:
						key = KEY_F12;
						break;
					case SC_UP:
						key = KEY_UP;
						break;
					case SC_DOWN:
						key = KEY_DOWN;
						break;
					case SC_LEFT:
						key = KEY_LEFT;
						break;
					case SC_RIGHT:
						key = KEY_RIGHT;
						break;
					case SC_HOME:
						key = KEY_HOME;
						break;
					case SC_END:
						key = KEY_END;
						break;
					case SC_PAGEUP:
						key = KEY_PAGE_UP;
						break;
					case SC_PAGEDOWN:
						key = KEY_PAGE_DOWN;
						break;
					case SC_INSERT:
						key = KEY_INSERT;
						break;
					case SC_DELETE:
						key = KEY_DEL;
						break;																			
					default:
						key = KEY_NOT_ALLOW;
						break;
				}

				bIdentifying = false;
				key_code_determine(key, 0xFF);
			}			
		}
		else
		{//normal key decode
			if(ch!=SC_ESC)
			{//number key
				if(	  ((ch >= '0')&&(ch <= '9')) )
				{
					key = KEY_NUM(ch);
				}
				else
				if(	  ((ch >= 'A')&&(ch <= 'Z'))
					||((ch >= 'a')&&(ch <= 'z')) )
				{//alpha key
					key = KEY_ALPHA(ch);
				}
				else
				{//control key
					switch(ch)
					{
						case SC_BACK:
							key = KEY_BACK;
							break;
						case SC_ENTER:
							key = KEY_ENTER;
							break;
						case SC_TAB:
							key = KEY_TAB;
							break;
						default:						
							key = KEY_NOT_ALLOW;
							break;							
					}
				}
				
				if((ch<0x20)||(ch>0x7E))
				{
					ch = 0xFF;
				}
				
				key_code_determine(key, ch);
				
			}
			else
			{//first ESC charater
				scanKeyCode = 0;
				bIdentifying = true;
				idxTmrSlot = TimerCtrl() << (class TimerService){SS_ONE_SHOT, SPEICAL_KEY_IDENTIFY_MS, NULL};
			}
		}
	}
	else
	if(bIdentifying)
	{//check if identifying
		//if(timer_decrement_expired(DECREASEMENT_SLOT_KEY_IDENTIFY))
		if(TimerCtrl().bExpired[idxTmrSlot])
		{			
			key_code_determine(KEY_ESC, 0xFF);
			bIdentifying = false;
		}
	}		

}

int KeyCtrl::KeyCore::kbhit(void)
{
    int bytesWaiting=0;    
    ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

void 
KeyCtrl::routine(void)
{

	int byteCnt;
	unsigned char ch;		
	
	byteCnt = core.kbhit();
	while(byteCnt)
	{		
		read(0,&ch,1);		
		core.scFIFO << ch;
		byteCnt--;
	}
	
	core.decode_sc();
	
}

