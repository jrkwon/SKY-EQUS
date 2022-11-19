package com.sktt.lcdui;

import javax.microedition.lcdui.*;
import com.xce.lcdui.*;
import com.xce.io.CharToByteEUC_KR;
import com.skt.m.*;
import java.util.Timer;
import java.util.TimerTask;
import java.io.*;

public class TCHandler extends TextComponentHandler{
	static final long TIMER_INTERVAL = 100L;
	int TimeCnt = 0;
		
	// Attribute
	private TextComponent tc = null;
	private AutoMata AM;
	private int inputMode;
	private int nextInputMode;
	private int allowedModes;
	
	// Timer
	private Timer timer;
	private TimerTask timerClient;
	
	private final int modes[] = {
		// 나중에 숫자모드 추가할 때 넣어주기
		HANGUL, ENG_CAPITAL, ENG_SMALL, SYMBOL, NUMBER
	};
	
	//ActionListener listener;
	
	// constructor
	public TCHandler(){
		Timer timer = new Timer();
		TimerTask timerClient = new TimerClient();
		AM = new AutoMata(this);
		timer.schedule(timerClient,TIMER_INTERVAL*10,TIMER_INTERVAL);
	}

	// Methods
	public void Insert(char c){
		if (tc != null){
			tc.insert(c);
			// tc.setCaretVisible(false);
			tc.repaint();
		}
	} // Insert method
	
	public void Replace(char c){
		if (tc != null){
			tc.replace(c);
			// tc.setCaretVisible(false);
			tc.repaint();	
		}
	} // Replace method
	
	public void Delete(){
		if (tc != null){
			tc.delete();
			tc.repaint();
		}
		
	} // Delete method
	
	public void Clear(){
		if (tc != null){
			tc.delete();
			tc.repaint();
		}
	} // Clear method
	
	public boolean keyPressed(int keyCode){
		boolean ret = true;
		
		switch (keyCode){
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
				TimeCnt = 0;
				AM.CharProcessing(keyCode);
				break;
			case Canvas.KEY_CLR:
				TimeCnt = 0;
				AM.CharProcessing(keyCode);
				endComposition();
				break;
			case Canvas.KEY_FIRE:
				TimeCnt = 0;
				//tc.repaintIM();
				AM.CharProcessing(keyCode);
				break;
			case Canvas.KEY_VOL_UP: // mode change
				TimeCnt = 0;
				tc.repaintIM();
				AM.CharProcessing(keyCode);
				break;
			case Canvas.KEY_LEFT:
			case Canvas.KEY_RIGHT:
			case Canvas.KEY_UP:
			case Canvas.KEY_DOWN:
				tc.moveCursor (keyCode);
				tc.repaint();
				EndComposition();
				return false;
			default:
				ret = false;
				break;
		}
		return ret;
	} // keyPressed method
	
	public boolean keyReleased(int keyCode) {
		return false;
	}
	
	public boolean keyRepeated(int i) {
		return false;
	}
	
	public void setTextComponent(TextComponent tc) {		
		if (tc != null) {
			this.tc = tc;
			setFocus(true);
		}
		else {
			clear();
			setFocus(false);
			this.tc = null;
		}
	} // setTextComponent method
	
	public TextComponent getTextComponent() {
		return tc;
	}
	
	public void clear() {
//		Clear();
//		EndComposition();
	}
	
	public int getInputMode(){
		return AM.GetEditMode();
	}
	
	private void setFocus(boolean flag) {}
	
	private void SetTimer(long timeOut) {
		if (tc!= null) 
			tc.setCaretVisible(false);
		timerClient.cancel();
		timer.schedule(timerClient, timeOut);
	}	
	
	private void EndComposition(){
		if (tc != null)
			tc.setCaretVisible(true);
		AM.EndComposition();
	}
	
	// 지움 키가 눌린 경우 timer등을 조절하기 위해 새로 정의 (보고)
	private void endComposition(){
		if (tc != null)
			tc.setCaretVisible(true);
		AM.endComposition();
	}
	
	////////////////////////////////////////////////////////////////////
	// Timer 
	class TimerClient extends TimerTask {		
		public final void run() {
			if (TimeCnt >= 30){ // 100*20 = 2000mSeconds
				TimeCnt = 0;
				EndComposition();
				if (tc!= null)
					tc.repaint();
			}
			else TimeCnt++;
		}
	} 
}
class TextModeAttribute{
	// Constants 
	// For EditMode
	public final int KOREAN_CHAR_MODE = 0;
	public final int ENGLISH_UCHAR_MODE = 1;
	public final int ENGLISH_LCHAR_MODE = 2;
	public final int SPECIAL_CHAR_MODE = 3;
	// 나중에 숫자모드 추가할 때 넣어주기
	public final int NUMBER_CHAR_MODE = 4;
	
	// For KoreanState
	public final int KOREAN_INIT_STATE = 0;
	public final int KOREAN_JAUM_STATE = 1;
	public final int KOREAN_JAMO_STATE = 2;
	public final int KOREAN_JAMOJA_STATE = 3;
	public final int KOREAN_JAMOBOKJA_STATE = 4;
	
	// Attribute
	int EditMode;
	int KoreanState;
	int SameKeyCount;
	int LastKey;
	int CurrentKey;
	
	// Constructor
	TextModeAttribute(){
		EditMode = KOREAN_CHAR_MODE;
		KoreanState =  KOREAN_INIT_STATE;   
		LastKey = -1;
		SameKeyCount = 0;
	}
	
	// Methods
	public void SetEditMode(int EM){EditMode = EM;}
	public int GetEditMode(){return EditMode;}
	public void SetKoreanState(int KS){KoreanState = KS;}
	public int GetKoreanState(){return KoreanState;}
	public void SetSameKeyCount(int SKC){ SameKeyCount = SKC;}
	public int GetSameKeyCount(){ return SameKeyCount;}
	public void SameKeyCountAddOne(){SameKeyCount++;}
} // TextModeAttribute class

class AutoMata{
	// Attribute
	// 조합형 초성/중성/종성 FILL Code
	final static char FILL_CHO = 0x0000;
	final static char FILL_JUNG = 0x0000;
	final static char FILL_JONG = 0x0000;
	char uTmpCode, uKorCode, uWansung, uJohab, uBokMoum, uRecoverCons;
	char uSaveCode, uSaveCons, uSaveWansung, uSaveJohabCho, uSaveJohabJung, uSaveJohabJong;
	char uJohabCho, uJohabJung, uJohabJong;
	
	TextModeAttribute TMA = null;
	KEYPAD Key = null;
	CodeConvert CC = null;
	TCHandler TH;
	
	// Constructor
	public AutoMata(TCHandler th){
		TMA = new TextModeAttribute();
		CC = new CodeConvert();
		Key = new KEYPAD();
		TH = th;
	}
	

	public void EndComposition(){
		TMA.SetKoreanState(TMA.KOREAN_INIT_STATE);
		TMA.SetSameKeyCount(0);
		TMA.LastKey = -1;
	}
	
	// 지움 키가 눌린 경우 timer등을 조절하기 위해 새로 정의 (보고)
	public void endComposition(){
		if (TMA.EditMode == TMA.KOREAN_CHAR_MODE) 
			TMA.SetSameKeyCount(-1);
		else
			TMA.LastKey = -1;
	}
	
	public void CharProcessing(int KeyCode){
		// 모드전환키일때 핸들하는 부분을 이곳으로 옮김 (보고)
		if (Key.IsChangModeKey(KeyCode))
			NextMode();
		
		if (TMA.EditMode == TMA.KOREAN_CHAR_MODE)
			KoreanCharProcessing(KeyCode);
		else if (TMA.EditMode == TMA.ENGLISH_UCHAR_MODE)
			EnglishUCharProcessing(KeyCode);
		else if (TMA.EditMode == TMA.ENGLISH_LCHAR_MODE)
			EnglishLCharProcessing(KeyCode);
		else if (TMA.EditMode == TMA.SPECIAL_CHAR_MODE)
			SpecialCharProcessing(KeyCode);    
		// 나중에 숫자모드 추가할 때 넣어주기			
		else if (TMA.EditMode == TMA.NUMBER_CHAR_MODE)
			NumberCharProcessing(KeyCode); 
	}
	
	// 모드 전환을 위해서 새로 추가 (보고)
	public void NextMode() {
		if (TMA.EditMode == TMA.KOREAN_CHAR_MODE)
			TMA.SetEditMode(TMA.ENGLISH_UCHAR_MODE);
		else if (TMA.EditMode == TMA.ENGLISH_UCHAR_MODE)
			TMA.SetEditMode(TMA.ENGLISH_LCHAR_MODE);
		else if (TMA.EditMode == TMA.ENGLISH_LCHAR_MODE)
			TMA.SetEditMode(TMA.SPECIAL_CHAR_MODE);
		else if (TMA.EditMode == TMA.SPECIAL_CHAR_MODE)
			// 나중에 숫자모드 추가할 때 넣어주기
			TMA.SetEditMode(TMA.NUMBER_CHAR_MODE);
		else if (TMA.EditMode == TMA.NUMBER_CHAR_MODE)
			TMA.SetEditMode(TMA.KOREAN_CHAR_MODE);
		TMA.SetSameKeyCount(0);
		TMA.SetKoreanState(TMA.KOREAN_INIT_STATE);
	}
	
	public int GetEditMode(){
		int ret = TH.HANGUL;
		if (TMA.EditMode == TMA.KOREAN_CHAR_MODE)
			ret = TH.HANGUL;
		else if (TMA.EditMode == TMA.ENGLISH_UCHAR_MODE)
			ret = TH.ENG_CAPITAL;
		else if (TMA.EditMode == TMA.ENGLISH_LCHAR_MODE)
			ret = TH.ENG_SMALL;
		else if (TMA.EditMode == TMA.SPECIAL_CHAR_MODE)
			ret = TH.SYMBOL;
		// 나중에 숫자모드 추가할 때 넣어주기
		else if (TMA.EditMode == TMA.NUMBER_CHAR_MODE)
			ret = TH.NUMBER;
		return ret;    
	}
	
	private void KoreanCharProcessing(int KeyCode){
		if (Key.IsClearKey(KeyCode)){
			ClearPrevChar();                 
		}
		else InputKoreanChar(KeyCode);
	}
	
	private void EnglishUCharProcessing(int KeyCode){
		if (Key.IsClearKey(KeyCode)){
			ClearPrevChar();                 
		}
		else InputEnglishChar(KeyCode);
	}
	
	private void EnglishLCharProcessing(int KeyCode){
		if (Key.IsClearKey(KeyCode)){
			ClearPrevChar();
		}
		else InputEnglishChar(KeyCode);
	}
	
	private void SpecialCharProcessing (int KeyCode){
		if (Key.IsClearKey(KeyCode))
			ClearPrevChar();
		else InputSpecialChar(KeyCode);
	}
	
	// 나중에 숫자모드 추가할 때 넣어주기
	private void NumberCharProcessing (int KeyCode){
		if (Key.IsClearKey(KeyCode))
			ClearPrevChar();
		else InputNumberChar(KeyCode);
	}
	
	private void ClearPrevChar(){
		switch(TMA.GetKoreanState()) {
			case 1: // 자음
			case 0: // 초기
				TH.Delete();
				TMA.SetKoreanState(0);
				break;
			case 2: // 자음+모음 or 자음+복모음
				uWansung = uJohabCho;
				TH.Replace(uWansung);
				TMA.SetKoreanState(1);
				//g_fEditChar = TRUE;
				break;
			case 3: // 자음+모음+자음
				uWansung = GetUnionCode(2,uJohabCho, uJohabJung, uJohabJong);
				TH.Replace(uWansung);
				TMA.SetKoreanState(2);
				//g_fEditChar = TRUE;
				break;
			case 4: // 자음+모음+복자음           
				uWansung = GetUnionCode(3,uJohabCho, uJohabJung, uSaveCons);
				TH.Replace(uWansung);
				TMA.SetKoreanState(3);
				//g_fEditChar = TRUE;
				break;
		}
	}
	
	private void InputKoreanChar(int KeyCode){
		boolean fDupKey = false;
		int     Index;
		
		if (!Key.IsCharKey(KeyCode)) return;
		if (TMA.LastKey == KeyCode){
			TMA.SameKeyCountAddOne();
			TMA.SetSameKeyCount(TMA.GetSameKeyCount()%3);
			fDupKey = true;
		}
		else{
			TMA.SetSameKeyCount(0);
			fDupKey = false;
		}
		TMA.LastKey = KeyCode;
		if (Key.IsStarKey(KeyCode)) Index = 10;
		else if (Key.IsPoundKey(KeyCode)) Index = 11;
		else Index = KeyCode - Canvas.KEY_NUM0;
		
		if (CC.KoreanTable[TMA.GetSameKeyCount()][Index] == 0x2020)
			TMA.SetSameKeyCount(0);
			// 2byte 완성형 한글 code  
		uKorCode = CC.KoreanTable[TMA.GetSameKeyCount()][Index];
			
		switch (TMA.GetKoreanState()){
			case 0:	// 초기
				uJohabCho = FILL_CHO;
				uJohabJung = FILL_JUNG;
				uJohabJong = FILL_JONG;
				uSaveJohabCho = FILL_CHO;
				uSaveJohabJung = FILL_JUNG;
				uSaveJohabJong = FILL_JONG;
				uSaveWansung = 0;
				uBokMoum = 0;
				if(fDupKey)
					TH.Replace(uKorCode);
				else
					TH.Insert(uKorCode);
						
				if(CC.ISJAUM(uKorCode)) {
					uJohabCho = uKorCode;					
					TMA.SetKoreanState(1);
				}
				else {
					TMA.SetKoreanState(0);
				}
				break;
			case 1:	// 자음 
				if(CC.ISJAUM(uKorCode)) {   // 자음
					if(fDupKey) {
						uRecoverCons = uKorCode;
						if(uJohabJong == FILL_JONG) {
							uJohabJong = uKorCode;
							uWansung = GetUnionCode(3,uSaveJohabCho, uSaveJohabJung, uJohabJong);
							if(uWansung!=0) {           // 자음/쌍자음 받침
								TH.Delete();
								TH.Replace(uWansung);
								uJohabCho = uSaveJohabCho;
								uJohabJung = uSaveJohabJung;
								uSaveJohabCho = FILL_CHO;
								uSaveJohabJung = FILL_JUNG;
								uSaveCons = uKorCode;
								TMA.SetKoreanState(3);
							}
							else {
								TH.Replace(uKorCode);
								uJohabCho = uKorCode;
								uJohabJung = FILL_JUNG;
								uJohabJong = FILL_JONG;
							}
						}
						else {                  // 복자음 받침
							uTmpCode = CC.GetBokJaum(uSaveCons, uRecoverCons);
							uWansung = GetUnionCode(4,uSaveJohabCho, uSaveJohabJung,uTmpCode);
							if(uWansung!=0) {
								TH.Delete();
								TH.Replace(uWansung);
								uJohabCho = uSaveJohabCho;
								uJohabJung = uSaveJohabJung;
								TMA.SetKoreanState(4);
							}
							else {
								TH.Replace(uKorCode);
								uJohabCho = uKorCode;
								uJohabJung = FILL_JUNG;
							}
						}
					}
					else {
						TH.Insert(uKorCode);
						uJohabCho = uKorCode;
						uJohabJung = FILL_JUNG;
						uJohabJong = FILL_JONG;    
						uSaveJohabCho = FILL_CHO;
						uSaveJohabJung = FILL_JUNG;
					}
				}
            	else {		// 모음
            		uSaveCode = uKorCode;	
            		uJohabJung = uKorCode;
					uJohabJong = FILL_JONG;
            		uWansung = GetUnionCode(2,uJohabCho, uJohabJung, uJohabJong);
            		
					if(uWansung!=0) {
            			TH.Replace(uWansung);
            			TMA.SetKoreanState(2);
            		}
            		else {
            			TH.Insert(uKorCode);
            			TMA.SetKoreanState(0);
            		}
            		
					uSaveJohabCho = FILL_CHO;
            		uSaveJohabJung = FILL_JUNG;
            		uSaveJohabJong = FILL_JONG;
            	}
            	break;
            case 2:	// 자음 + 모음 or 자음 + 복모음
            	if(CC.ISJAUM(uKorCode)) {	// 자음
            		uSaveCons = uKorCode;
            		uSaveJohabCho = uJohabCho;
            		uSaveJohabJung = uJohabJung;
            		uSaveJohabJong = uJohabJong;
            		uJohabJong = uKorCode;

					uWansung = GetUnionCode(3,uJohabCho, uJohabJung, uSaveCons);
            		//uWansung = GetUnionCode(3,uJohabCho, uJohabJung, uJohabJong);
            		if(uWansung!=0) {
            			TH.Replace(uWansung);
            			TMA.SetKoreanState(3);
            		}
            		else {
            			TH.Insert(uKorCode);
            			uJohabJung = FILL_JUNG;
            			uJohabJong = FILL_JONG;
            			uJohabCho = uKorCode;
            			TMA.SetKoreanState(1);
            		}
            	}
            	else {		// 모음
            		if(fDupKey) {
            			uSaveCode = uKorCode;	
						uJohabJung = uKorCode;
						uJohabJong = FILL_JONG;
						uWansung = GetUnionCode(2,uJohabCho, uJohabJung, uJohabJong);
						if(uWansung!=0)
						{
							TH.Replace(uWansung);
							TMA.SetKoreanState(2);
						}
						else
						{
							TH.Insert(uKorCode);
							TMA.SetKoreanState(0);
        				}
					}
					else
					{
						uBokMoum = CC.GetBokMoum(uSaveCode, uKorCode);
						uSaveCode = uBokMoum;
						uJohabJung = uBokMoum;
						uJohabJong = FILL_JONG;
						uWansung = GetUnionCode(2,uJohabCho, uJohabJung, uJohabJong);
						
						if((uBokMoum!=0) && (uWansung!=0))
						{
							TH.Replace(uWansung);
							TMA.SetKoreanState(2);
								
						}
						else
						{
							TH.Insert(uKorCode);
							TMA.SetKoreanState(0);
						}
					}
			}
	   		break;

	    case 3:		// 자음 + 모음 + 자음
		    if(CC.ISJAUM(uKorCode))
            {
			    if(fDupKey)
                {
				    uSaveCons = uKorCode;
                    uSaveJohabCho = uJohabCho;
                    uSaveJohabJung = uJohabJung;
       			    uSaveWansung = uWansung;
					uJohabJong = uKorCode;
                    uSaveJohabJong = uJohabJong;
					uWansung = GetUnionCode(3, uJohabCho, uJohabJung, uJohabJong);
				    if(uWansung!=0)
                    {
                        TH.Replace(uWansung);
                        TMA.SetKoreanState(3);
				    }
                    else
                    {
						uWansung = GetUnionCode(2, uJohabCho, uJohabJung, FILL_JONG);
				        //uWansung = GetUnionCode(3, uJohabCho, uJohabJung, FILL_JONG);
				        TH.Replace(uWansung);
                        TH.Insert(uKorCode);
                            
                        uJohabJung = FILL_JUNG;
					    uJohabJong = FILL_JONG;
					    uJohabCho = uKorCode;
                        TMA.SetKoreanState(1);
				    }
			    }
                else
                {
				    uRecoverCons = uKorCode;
                    uSaveJohabCho = uJohabCho;
                    uSaveJohabJung = uJohabJung;

				    uSaveWansung = uWansung;
				    uTmpCode = CC.GetBokJaum(uJohabJong, uKorCode);
				   	//uTmpCode = CC.GetBokJaum(uSaveCons, uKorCode);
				   	uWansung = GetUnionCode(4, uJohabCho, uJohabJung, uTmpCode);
				    
					if((uTmpCode!=0) && (uWansung!=0))
                    {	// 복자음
                        TH.Replace(uWansung);
                        TMA.SetKoreanState(4);
				    }
                    else
                    {
                        TH.Insert(uKorCode);
                        uJohabCho = uKorCode;
					    uJohabJung = FILL_JUNG;
					    TMA.SetKoreanState(1);
				    }
			    }
		    }
            else
            {
			    uSaveWansung = GetUnionCode(2, uJohabCho, uJohabJung, FILL_JONG);
			    uSaveCode = uKorCode;
			    uJohabCho = uSaveCons;
			    uJohabJung = uKorCode;
			    uJohabJong = FILL_JONG;
				uWansung = GetUnionCode(2, uJohabCho, uJohabJung, uJohabJong);
			    
			    if(uWansung!=0)
                {
                    TH.Replace(uSaveWansung);
                    TH.Insert(uWansung);
                    TMA.SetKoreanState(2);
                }
                else
                {
                    TH.Insert(uKorCode);
                    TMA.SetKoreanState(0);
                }
		    }

		    break;

	    case 4:	// 자 + 모 + 복자음
		    if(CC.ISJAUM(uKorCode))
            {
			    if(fDupKey)
                {
				    uRecoverCons = uKorCode;
				    uTmpCode = CC.GetBokJaum(uSaveCons, uRecoverCons);
				    uWansung = GetUnionCode(4, uJohabCho, uJohabJung, uTmpCode);
				    if((uTmpCode!=0) && (uWansung!=0))
                    {	// 복자음
                        TH.Replace(uWansung);
                        TMA.SetKoreanState(4);
				    }
                    else
                    {
                        uWansung = GetUnionCode(3, uJohabCho, uJohabJung, uSaveCons);
                        TH.Replace(uWansung);
                        TH.Insert(uRecoverCons);
                        uJohabCho = uKorCode;
					    uJohabJung = FILL_JUNG;
					    TMA.SetKoreanState(1);						
				    }
			    }
                else
                {
                    TH.Insert(uKorCode);
                    uJohabCho = uKorCode;
				    uJohabJung = FILL_JUNG;
				    TMA.SetKoreanState(1);						
			    }
		    }
            else
            {
			    
				uJohabCho = uRecoverCons;
                uJohabJung = uKorCode;
			    uJohabJong = FILL_JONG;
			    uSaveCode = uKorCode;
			    uWansung = GetUnionCode(2, uJohabCho, uJohabJung, uJohabJong);
			    
				if(uWansung!=0)
                {
                    TH.Replace(uSaveWansung);
                    TH.Insert(uWansung);
                    TMA.SetKoreanState(2);
                }
                else
                {
                    TH.Insert(uKorCode);
                    TMA.SetKoreanState(0);
                }
		    }

		    break;
        default:
		    break;
	    }
	}

	private char GetUnionCode(int state,char cho,char jung,char jong){
	
		int unionCode = 0;
		char tempCode = 0;
	    
		switch(state){
			case 1: // 자음 
				unionCode = uJohabCho;
				break;
			case 2: // 자음 + 모음 
				unionCode = 0xAC00 + (cho-CC.LBase)*CC.NCount + (jung-CC.VBase)*CC.TCount;
				break;
			case 3: // 자음 + 모음 + 자음 
				tempCode = CC.Cho2Jong[jong-CC.LBase];
				if (tempCode != 0) 
					unionCode = 0xAC00 + (cho-CC.LBase)*CC.NCount + (jung-CC.VBase)*CC.TCount + (tempCode-CC.TBase);
				else return 0;
				// 표ㅉ -> 습... 으로 되는 버그를 해결하기 위해서...
					//unionCode = 0xAC00 + (cho-CC.LBase)*CC.NCount + (jung-CC.VBase)*CC.TCount +  (jong-CC.TBase);
				break;
			case 4: // // 자 + 모 + 복자음 
			//;tempCode = CC.Cho2Jong[jong-CC.LBase];
				if (jong != 0)
				   unionCode = 0xAC00 + (cho-CC.LBase)*CC.NCount + (jung-CC.VBase)*CC.TCount + (jong-CC.TBase);
				else
				   return 0;
				break;
		}

		if (CharToByteEUC_KR.unicodeToEUCKR(unionCode) == 0) {
			return 0;
		}
		return (char)unionCode;
	}

	private void InputEnglishChar(int KeyCode){
		if (!Key.IsCharKey(KeyCode)) return;
		
		boolean   fInsert;
		int       Index;
		char      inputChar;
		
		if (TMA.LastKey == KeyCode){
			TMA.SameKeyCountAddOne();
			TMA.SetSameKeyCount(TMA.GetSameKeyCount()%4);
			fInsert = true;
		}
		else{
			TMA.SetSameKeyCount(0);
			fInsert = false;
		}
		TMA.LastKey = KeyCode;
		
		//g_fEditChar = TRUE;
		if (Key.IsStarKey(KeyCode)){ Index = 10; fInsert = false;}
		else if (Key.IsPoundKey(KeyCode)){Index = 11;fInsert = false;}
		else Index = KeyCode - Canvas.KEY_NUM0;
		
		if(TMA.GetEditMode() == TMA.ENGLISH_UCHAR_MODE) {
			if(CC.EnglishUTable[TMA.GetSameKeyCount()].charAt(Index) == ' ') 
				TMA.SetSameKeyCount(0);
			inputChar = CC.EnglishUTable[TMA.GetSameKeyCount()].charAt(Index);
		}
		else {
			if(CC.EnglishLTable[TMA.GetSameKeyCount()].charAt(Index) == ' ')
				TMA.SetSameKeyCount(0);
			inputChar = CC.EnglishLTable[TMA.GetSameKeyCount()].charAt(Index);
		}
		if(fInsert)
			TH.Replace(inputChar);
		else
			TH.Insert(inputChar);
	}
	
	// 이 부분을 구현하였음 (보고)
	private void InputSpecialChar(int KeyCode){
		SymbolTable symbolTable = new SymbolTable(TH, TMA);
//		Image backImg;
//		try {
//			backImg = Graphics2D.captureLCD(0, 0, XDisplay.width, XDisplay.height + XDisplay.commandHeight);
//			symbolTable.setImage(backImg);
//		} catch (IOException ioe) {}
		symbolTable.showTable();
	} // InputSpecialChar method
	
	// 나중에 숫자모드 추가할 때 넣어주기
	private void InputNumberChar (int KeyCode){
		if (!Key.IsCharKey(KeyCode)) return;
		
		boolean   fInsert;
		int       Index;
		char      inputChar;
		
		if (Key.IsStarKey(KeyCode)){ Index = 10; fInsert = false;}
		else if (Key.IsPoundKey(KeyCode)){Index = 11;fInsert = false;}
		else Index = KeyCode - Canvas.KEY_NUM0;
		
		inputChar = CC.NumberTable[0].charAt(Index);
		TH.Insert(inputChar);

	} // InputNumberChar method
}

class SymbolTable extends Canvas {
	
	// 한페이지에 12개씩 심볼을 보여준다.
	final static char[][] table = {
		{' ', '.', ',', '!', ':', ';', '?', '\"', '\'', '`', '~', '^'},
		{'+', '-', '_', '*', '#', '@', '<', '>', '=', '\\', '$', '%'},
		{'(', ')', '[', ']', '{', '}', '/', '|', '&', ' ', ' ', ' '},
				
		{'※', '§', '℃', '￥', '♂', '♀', '∞', '∴', '°', '·', '…', '〃'},
		{'◈', '▣', '◐', '◑', '☏', '☎', '♨', '☜', '☞', '♩', '♪', '♬'},
		{'☆', '○', '◎', '◇', '□', '△', '▽', '◁', '▷', '♤', '♡', '♧'},
		{'★', '●', '⊙', '◆', '■', '▲', '▼', '◀', '▶', '♠', '♥', '♣'},
		{'→', '←', '↔', '↑', '↓', '↕', '↗', '↙', '↖', '↘', '⇒', '⇔'}
	};

	int index;
	int page;
	private TCHandler TH;
	private TextModeAttribute TMA;
//	Image backImg;

	public SymbolTable(TCHandler ath, TextModeAttribute atma){
		this.TH = ath;
		this.TMA = atma;
	}

	public void paint( Graphics g) {
		g.setClip(0, 0, XDisplay.width, XDisplay.height + XDisplay.commandHeight);		
//		g.drawImage(backImg, 0, 0, Graphics.TOP | Graphics.LEFT);
		g.setColor(Toolkit.WHITE);
		g.fillRect(1, 18, XDisplay.width,XDisplay.height + XDisplay.commandHeight );

		g.drawImage(Toolkit.sBackImg(), 5, 20, Graphics.TOP | Graphics.LEFT);
		g.setColor(Toolkit.BLACK);
		
		int x = 21;
		int y = 23;
		
		int count = 0;
		for (int i = index; i < table[page].length; i++) {
			if (x > 90) {
				x = 21;
				y += Toolkit.FONT_HEIGHT;
			}		
				
			g.drawChar(table[page][i], x, y, Graphics.TOP | Graphics.LEFT);
				
			x += 28;
			count++;
			
			if (count == 12) {
				break;
			}
		}			
		
		g.drawString ((page + 1) + "/" + (table.length),
			105, 40, Graphics.TOP | Graphics.HCENTER);
			
		XDisplay.refresh(5, 20, XDisplay.width - 10, 57);	
	} // paint method

	public void keyPressed(int keyCode) {
		switch (keyCode) {
			case Canvas.KEY_LEFT:
			case Canvas.KEY_UP:
				index = 0;
				page--;
				if (page < 0) {
					page = table.length - 1;
				}
				repaint();
				break;
					
			case Canvas.KEY_RIGHT:
			case Canvas.KEY_DOWN:
				index = 0;
				page++;
				if (page == table.length) {
					page = 0;
				}
				repaint();
				break;
			case Canvas.KEY_NUM0:
				insert(10);
				break;
			case Canvas.KEY_NUM1:
			case Canvas.KEY_NUM2:
			case Canvas.KEY_NUM3:
			case Canvas.KEY_NUM4:
			case Canvas.KEY_NUM5:
			case Canvas.KEY_NUM6:
			case Canvas.KEY_NUM7:
			case Canvas.KEY_NUM8:
			case Canvas.KEY_NUM9:
				insert(keyCode - 49);
				break;
			case Canvas.KEY_STAR:					
				insert(9);
				break;		
			case Canvas.KEY_POUND:
				insert(11);
				break;
			default:
				hideTable();
		} // switch
	} // keyPressed method
		
	void insert(int pos) {
		if ((index + pos) < table[page].length) {
			TH.Insert((char)table[page][index + pos]);
			hideTable();				
		}
	} // insert method

//	public void setImage(Image img){
//		backImg = img;
//	}
	
	public void showTable() {
		XEventHandler.eventHandler.setSystemCurrent(this);
	}	
	
	public void hideTable() {
		XEventHandler.eventHandler.restoreDisplay();
		// 여기에 다음 모드로 바꿔주는 부분이 필요하다.
		//TMA.SetEditMode(TMA.NUMBER_CHAR_MODE);
		// 나중에 숫자모드 추가할 때 넣어주기
		TMA.SetEditMode(TMA.NUMBER_CHAR_MODE);
		//TMA.SetEditMode(TMA.KOREAN_CHAR_MODE);
	}
} // SymbolTable class
