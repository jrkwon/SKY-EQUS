#ifndef _TESTGRPH_H_
#define _TESTGRPH_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SCREEN_WIDTH    120
#define SCREEN_HEIGHT   127

extern void OpenNUIGraphics(int nColor);
extern void CloseNUIGraphics(void);
extern unsigned char* GetLCDBuffer(void);
extern VOID ClearNUIScreen(VOID);
extern VOID NUIGraphicsDemo(VOID);
extern VOID NUIGraphicsDemo256Image();
extern VOID NUIGraphicsDemo4GImage();
extern VOID NUIGraphicsDemoPrimitive();
extern VOID NUIGraphicsDemoTextOut(VOID);
extern VOID NUIGraphicsDemoDC();
extern VOID NUIGraphicsDemoGetPutImage();

extern void TestFormatter();

#ifdef __cplusplus
}
#endif

#endif