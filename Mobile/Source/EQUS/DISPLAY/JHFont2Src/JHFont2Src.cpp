// JHFont2Src.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>

/* JHFont2Src <filename> <Width> <Height> */
/* The extension of the output file is ".EJF" which means EQUS hangule Johap Font */
int main(int argc, char* argv[])
{
    FILE *fIn, *fOut;
    char OutFileName[80];
    int nData;
    char* temp;
    //int nWidht, nHeight;
    int i = 0;

    //nWidht = 16, nHeight = 16; // default value

    if(argc != 2)//arc != 4 && argc != 2)
    {
//        printf("usage : \n\tJHFont2Src <filename> <font width> <font height>\n");
        printf("usage : \n\tJHFont2Src <filename>\n");
        return 0;
    }
    fIn = fopen(argv[1], "rb");
    if(!fIn)
    {
        printf("Error : file open (%s)\n", argv[1]);
        return 0;
    }
    sprintf(OutFileName, argv[1]);
    temp = strchr(OutFileName, '.');
    *temp = '\0';
    strcat(OutFileName, ".EJF");
    fOut = fopen(OutFileName, "wt");
    if(!fOut)
    {
        printf("Error : file open (%s)\n", OutFileName);
        return 0;
    }

    i = 0;
    /* Cycle until end of file reached: */
    while( !feof( fIn ) )
    {
        if(i != 0 && !(i % 8))
            fprintf(fOut, "\n");
        nData = fgetc(fIn);
        if(nData == EOF)
            break;
        fprintf(fOut, "0x%02X, ", nData);
        i++;
    } 
    printf("Success!!\n");
	return 0;
}


