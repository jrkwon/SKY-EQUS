/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
/*
 * ConTypes.h
 *
 * Generated by CreatePrefixTree.
 * DO NOT EDIT!!!
 * To add or modify the list of supported content types,
 * edit the file ContentTypes.txt and then run CreatePrefixTree.
 */

#ifndef _ConTypes_h
#define _ConTypes_h

#define NUM_NODES  57

typedef struct node_s {
  char *s;
  unsigned char len;
  signed char sibling;
  signed char child;
  signed char parent;
  signed char idx;
} node_t;

const node_t treeNodes[NUM_NODES] = {
  {"*/*", 3, 1, -1, -1, 0},
  {"text/", 5, 13, 2, -1, -1},
  {"*", 1, 3, -1, 1, 1},
  {"html", 4, 4, -1, 1, 2},
  {"plain", 5, 5, -1, 1, 3},
  {"x", 1, 11, 54, 1, -1},
  {"hdml", 4, 7, -1, 54, 4},
  {"ttml", 4, 8, -1, 54, 5},
  {"vca", 3, -1, 9, 54, -1},
  {"lendar", 6, 10, -1, 8, 6},
  {"rd", 2, -1, -1, 8, 7},
  {"vnd.wap.wml", 11, -1, 12, 1, 8},
  {"script", 6, -1, -1, 11, 9},
  {"application/", 12, 14, 20, -1, -1},
  {"multipart/", 10, 39, 15, -1, -1},
  {"*", 1, 16, -1, 14, 11},
  {"mixed", 5, 17, -1, 14, 12},
  {"form-data", 9, 18, -1, 14, 13},
  {"byteranges", 10, 19, -1, 14, 14},
  {"alternative", 11, -1, -1, 14, 15},
  {"vnd.wap.", 8, 21, 26, 13, -1},
  {"*", 1, 22, -1, 13, 16},
  {"java-vm", 7, 23, -1, 13, 17},
  {"x", 1, -1, 52, 13, -1},
  {"www-form-urlencoded", 19, 25, -1, 52, 18},
  {"hdmlc", 5, 36, -1, 52, 19},
  {"catc", 4, 27, -1, 20, 10},
  {"w", 1, 32, 30, 20, -1},
  {"c", 1, 29, -1, 30, 20},
  {"scriptc", 7, -1, -1, 30, 21},
  {"ml", 2, 31, 28, 27, -1},
  {"sic", 3, 33, -1, 27, 22},
  {"uaprof", 6, 46, -1, 20, 23},
  {"tls-", 4, 56, 34, 27, -1},
  {"ca-certificate", 14, 35, -1, 33, 24},
  {"user-certificate", 16, -1, -1, 33, 25},
  {"x509-", 5, -1, 37, 52, -1},
  {"ca-cert", 7, 38, -1, 36, 26},
  {"user-cert", 9, -1, -1, 36, 27},
  {"image/", 6, -1, 40, -1, -1},
  {"*", 1, 41, -1, 39, 28},
  {"gif", 3, 42, -1, 39, 29},
  {"jpeg", 4, 43, -1, 39, 30},
  {"tiff", 4, 44, -1, 39, 31},
  {"png", 3, 45, -1, 39, 32},
  {"vnd.wap.wbmp", 12, -1, -1, 39, 33},
  {"multipart.", 10, -1, 47, 20, -1},
  {"*", 1, 48, -1, 46, 34},
  {"mixed", 5, 49, -1, 46, 35},
  {"form-data", 9, 50, -1, 46, 36},
  {"byteranges", 10, 51, -1, 46, 37},
  {"alternative", 11, -1, -1, 46, 38},
  {"-", 1, 53, 24, 23, -1},
  {"ml", 2, -1, -1, 23, 39},
  {"-", 1, 55, 6, 5, -1},
  {"ml", 2, -1, -1, 5, 40},
  {"bxml", 4, -1, -1, 27, 41}
};

#endif
