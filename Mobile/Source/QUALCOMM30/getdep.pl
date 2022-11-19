#! /usr/bin/env perl

#############################################################################
#
#                             G E T D E P
#
# GENERAL DESCRIPTION
#   Process the output of the C preprocessor into a nice list of dependencies.
#   The -M option on tcc works poorly, and can't handle an assembly file.
#   The first argument is the name of our object file.
#   The second object is the name of our source (needed for <stdin>
#   replacement).
#
# INVOCATION
#   perl getdep.pl file.o file.c
#
# Copyright (c) 1998, 1999 by QUALCOMM Incorporated.  All Rights Reserved.
#############################################################################
#
#                        EDIT HISTORY FOR FILE
#
# $Header:   L:/src/asw/MSM5000/VCS/getdep.plv   1.1   10 Sep 1999 17:20:46   lchan  $
#
# when       who     what, where, why
# --------   ---     --------------------------------------------------------
# 10/13/98   dlb     Initial version.
#
#############################################################################

die "Usage: perl getdep.pl file.o file.c\n"
    unless $#ARGV == 1;

$object = $ARGV[0];
$source = $ARGV[1];

# The object is probably of the form 'XXnnnn\name.o'.  Fix this to be
# '$(TARGET)\name.o'.
$object =~ s/^[A-Z0-9a-z]+\\/\$\(TARGET\)\\/;

%deps = ();

while (<STDIN>) {
  next unless (/^\#line\s+\d+\s+\"(.*)\"/);

  # Fix up a few names.
  $name = $1;

  $name =~ s/\<stdin\>/$source/;

  # print $name, "\n";
  $deps{$name} = 1;
}

# Tricky stuff.  Need to remove any dependencies on files that are not
# present in this directory.  This indicates that it probably came
# from a -I.
#
# This is needed due to stupidity in the ARM compiler to not output
# paths on the #line directives.

# Print out the dependencies.

for $name (sort keys %deps) {
  next unless -f $name;
  print "$object: $name\n";
}

###########################################################################
# End of Perl script.
###########################################################################
