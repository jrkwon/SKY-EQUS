/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_slcd.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-12-25 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define DISPLAY_START_LINE			0x40	// Set Display Start Line

#if MODEL_ID == MODEL_B2
#define	ID_LCD_OUTP_CMD(Value)      outp(0x2000010, (Value))
#define	ID_LCD_OUTP_DATA(Value)     outp(0x2000012, (Value))
#else
#define	ID_LCD_OUTP_CMD(Value)      outp(0x2000004, (Value))
#define	ID_LCD_OUTP_DATA(Value)     outp(0x2000006, (Value))
#endif
//#define PIXEL2BYTES(n)              ((n+7)/8)
#define WIDTH_IN_BYTE               PIXEL2BYTES(SKY_SUB_SCREEN_WIDTH)
#define CONVERT_UNIT                8
#define MSB_BIT_MASK                0x80
#define MAX_BANK                    4

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void sLCDCommand(BYTE bLCDCommand);
LOCAL void sLCDData(WORD wCount, BYTE *pLCDData);
LOCAL BYTE *SKY_API sConvertBuffer(BYTE* pBuffer, const BYTE* pFromBuffer);

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/

//LOCAL BOOL l_fActiveSubLCD = TRUE;

LOCAL void sLCDCommand(BYTE bLCDCommand)
{
    //INTLOCK();
    /* bits of a byte output */
    ID_LCD_OUTP_CMD(bLCDCommand);

    //INTFREE();
}

LOCAL void sLCDData(WORD wCount, BYTE *pLCDData)
{
    WORD i;
    
    /* bits of a byte output */
    for(i = 0; i < wCount; i++) 
    {
        //INTLOCK();
        
        ID_LCD_OUTP_DATA(*pLCDData++);

        //INTFREE();
    }
}/* end of sLCDData */

LOCAL void SKY_API sBootLogoDisplay(void)
{
    BYTE aBuffer[SKY_SUB_SCREEN_WIDTH*MAX_BANK]; /* sub lcd buffer */
//    ROM BYTE abImage[] = {
//        #include "slcd_logo84x32xBW.ebi" 
//    };
    INT8 i;

    sConvertBuffer(aBuffer, g_BmpBootLogoSub.pbData);//abImage);

    for(i = 0; i < MAX_BANK; i++)
    {
        sLCDCommand(0xB0 + (MAX_BANK-1) - i);
        sLCDCommand(0x11);
        sLCDCommand(0x08);
        sLCDData(SKY_SUB_SCREEN_WIDTH, aBuffer + (SKY_SUB_SCREEN_WIDTH*i));
    }
}

void SKY_API SKY_InitSubLCD(void)
{
#ifdef SW_RESET_ENABLE
	if(mc_bSwReset)
	{
        return;
    }
#endif
	sLCDCommand(0x40);		
    sLCDCommand(0xd3);      // Set Display Offest			   Extended CMD05.
    sLCDCommand(0x11);      // Set Display Offest			   Extended CMD05.	
	sLCDCommand(0xa3);		// Set LCD Bias					
	sLCDCommand(0xc0);		// 2001.06.08 Set COM Output Scan Direction
	sLCDCommand(0xa6);		// 2001.06.08 Display Reverse
	sLCDCommand(0xa0);		// 2001.06.08 Segment Remap
	sLCDCommand(0x2f);		// Set Set Power Control Resgister4.
    sLCDCommand(0x22);		// Set Internal RRR				03.(20,21,*22,23,24,...,27(0010 0111))
    sLCDCommand(0x81);		// Set Contrast (CMD)
    sLCDCommand(SUBLCD_CONTRAST_DEFAULT);		// Set Contrast (DAT)	..............42................
//    sLCDCommand(0xa9);		// Set Bias Ratio, Set TC Value, Modify Osc.Freq(CMD)	Extended CMD02. 
//    sLCDCommand(0x6a);
    sLCDCommand(0xa8);		// Set Duty Ratio (CMD)				Extended CMD01.
    sLCDCommand(0x1c);		// Set Duty Ratio (DAT)				Extended CMD01.	---- 32
	sLCDCommand(0xaf);		// Set Display On				11.	

    sBootLogoDisplay();
}

void SKY_API SKY_ReInitSubLCD(BYTE bIRR)
{
	sLCDCommand(DISPLAY_START_LINE);	
    sLCDCommand(0xd3);     // Set Display Offest			    Extended CMD05.
    sLCDCommand(0x11);     // Set Display Offest			    Extended CMD05.
	sLCDCommand(0xa0);		// Set Segment Remap			07.
	sLCDCommand(0xa3);		// Set LCD Bias					08.
	sLCDCommand(0xc8);		// Set COM Output Scan Direction13.
	sLCDCommand(0x2f);		// Set Set Power Control Resgister4.
//    hs_idlcd_cmd(0x22);		// Set Internal RRR				03.
    sLCDCommand(0x20 + bIRR);		// Set Internal RRR				03.
    sLCDCommand(0x81);		// Set Contrast (CMD)
    sLCDCommand(0x2b);		// Set Contrast (DAT)
    sLCDCommand(0xa8);		// Set Duty Ratio (CMD)				Extended CMD01.
    sLCDCommand(0x1c);		// Set Duty Ratio (DAT)				Extended CMD01.	---- 32
	sLCDCommand(0xaf);		// Set Display On				11.
}

void SKY_API SKY_SubLCDOn(void)
{
    //sLCDCommand(LCD_SUB_DISPLAY_ON);
    //l_fActiveSubLCD = TRUE;
}

void SKY_API SKY_SubLCDOff(void)
{
    //sLCDCommand(LCD_SUB_DISPLAY_OFF);
    //l_fActiveSubLCD = FALSE;
}

void SKY_API SKY_SetSubLCDContrast(HS_CONTRAST_E wContrast, BYTE wContrastLevel)
{
	BYTE Result_contrast;

    if(/*wContrast >= 0 && */wContrastLevel <= LCD_SUB_CONTOFFSET_MAX) 
    {
		Result_contrast = (wContrast - HCL_D) * SLCD_CONT_STEP + wContrastLevel;
        sLCDCommand(0x81);
        sLCDCommand(Result_contrast);
        /* contrast value must be changed to logical value not physical value(WORD) */
        //SKY_SET_CONTRAST(wContrast);
    }
}

LOCAL BYTE *SKY_API sConvertBuffer(BYTE* pBuffer, const BYTE* pFromBuffer)
{
    BYTE *pScreenBuffer;                /* the general screen buffer */
    BYTE aDest[CONVERT_UNIT];
    BYTE aSrc[CONVERT_UNIT];
    SCREEN_INDEX_E Screen = SI_MAX_SCREEN;
    INT8 i, j, k, l;
    INT8 nWidthInByte = WIDTH_IN_BYTE;  /* for better speed */

    /* set the sub screen as the active screen and save old screen index */
    INTLOCK();
    {
        if(!pFromBuffer)
            Screen = SelectScreen(SI_SUB_SCREEN);
        /* get the screen buffer */
        pScreenBuffer = pFromBuffer ? (BYTE*)pFromBuffer : GetScreenBuffer();
        /* restore original screen index */
        if(!pFromBuffer)
            SelectScreen(Screen);
    }
    INTFREE();

    /* convert from general screen buffer to sub lcd buffer */
    for(i = 0; i < MAX_BANK; i++)
    {
        for(j = 0; j < nWidthInByte; j++)
        {
            /* clear destination buffer */
            memset(aDest, 0, CONVERT_UNIT);

            /* get source data by 8 byte */
            for(k = 0; k < 8; k++)
            {
                aSrc[k] = pScreenBuffer[k*nWidthInByte + j + SKY_SUB_SCREEN_BUFFER_WIDTH*i];
            }
            /* set destination data by 8 byte from source data */
            for(k = 0; k < 8; k++)      /* vert */
            {
                if((j*CONVERT_UNIT + k) == SKY_SUB_SCREEN_WIDTH)
                    break;
                for(l = 0; l < 8; l++)  /* horiz */
                {
                    if(aSrc[l] & (MSB_BIT_MASK >> k))
                    {
                        aDest[k] |= (MSB_BIT_MASK >> l);
                    }
                }
                *(pBuffer + j*CONVERT_UNIT + k + SKY_SUB_SCREEN_WIDTH*i) = aDest[k];
            }
//            memcpy(pBuffer + j*CONVERT_UNIT + SKY_SUB_SCREEN_WIDTH*i, aDest, CONVERT_UNIT);
        }
    }
    
    return pBuffer;
}

void SKY_API SKY_UpdateSubScreen(void)
{
    BYTE aBuffer[SKY_SUB_SCREEN_WIDTH*MAX_BANK]; /* sub lcd buffer */
    INT8 i;
    
    /* sub LCD is not active, just do nothing */
    //if(l_fActiveSubLCD == FALSE)
    //    return;

    sConvertBuffer(aBuffer, NULL);
    for(i = 0; i < MAX_BANK; i++)
    {
        sLCDCommand(0xB0 + (MAX_BANK-1) - i);
        sLCDCommand(0x11);
        sLCDCommand(0x08);
        sLCDData(SKY_SUB_SCREEN_WIDTH, aBuffer + (SKY_SUB_SCREEN_WIDTH*i));
    }
}
