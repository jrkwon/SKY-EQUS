#ifndef __MMF_DECODER_H
#define __MMF_DECODER_H

#define OCTET unsigned char

int getMMFInfo(OCTET* mmfdata, int* titleSize, int* imageSize, int* musicSize, int* textSize);
int mmfDecode(OCTET* mmfdata, OCTET* title, OCTET* image, int* imageType, OCTET* music, int* musicType, OCTET* text);

#endif