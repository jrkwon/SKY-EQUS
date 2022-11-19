#===================================================================
# FILE: asm.pl
#
# SERVICES: Strips #line directives from preprocessed files
#           Filters out blank lines.
#
# $Header:   L:/src/asw/MSM5000/VCS/asm.plv   1.1   10 Sep 1999 17:15:36   lchan  $
#
# (c) COPYRIGHT 1998 Qualcomm Incorporated.  All Rights Reserved.
# (c) COPYRIGHT 1999 Qualcomm Incorporated.  All Rights Reserved.
#                   QUALCOMM Proprietary/GTDR
#===================================================================
while(<>) {
   s/^\s*(#line )+(.)+//; 
   if (!/^\s*$/) {
      print $_ ;
   }
}
