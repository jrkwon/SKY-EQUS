package com.sktt.lcdui;

import javax.microedition.lcdui.*;

public class KEYPAD {
	// Constructor
	KEYPAD(){}
	
	// Method
	// 모드 전환키가 결정되면 여기서 바꿔주기. (보고)
	public boolean IsChangModeKey(int KeyCode) {
		//return (KeyCode == Canvas.KEY_FIRE ? true  : false );
		return (KeyCode == Canvas.KEY_VOL_UP ? true  : false );
	}
	public boolean IsClearKey(int KeyCode){
		return ( KeyCode == Canvas.KEY_CLR ? true : false );
	}
	public boolean IsStarKey(int KeyCode){
		return ( KeyCode == Canvas.KEY_STAR ? true : false );
	}
	public boolean IsPoundKey(int KeyCode){
		return ( KeyCode == Canvas.KEY_POUND ? true : false );
	}
	
	public boolean IsCharKey(int KeyCode){
		boolean ret = false;
		switch (KeyCode){
			case Canvas.KEY_NUM0:
			case Canvas.KEY_NUM1:
			case Canvas.KEY_NUM2:
			case Canvas.KEY_NUM3:
			case Canvas.KEY_NUM4:
			case Canvas.KEY_NUM5:
			case Canvas.KEY_NUM6:
			case Canvas.KEY_NUM7:
			case Canvas.KEY_NUM8:
			case Canvas.KEY_NUM9:
			case Canvas.KEY_STAR:
			case Canvas.KEY_POUND:
				ret = true;
				break;
			default:
				break;
		}
		return ret;
	} // IsCharKey method
} // KEYPAD class