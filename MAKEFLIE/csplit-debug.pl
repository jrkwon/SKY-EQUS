# ========================================================================
#                            C Source Separator
#
# DESCRIPTION:  This script is used to separate a C source into its ROM
#    and internal RAM components.  Internal RAM code sections are
#    demarcated by preprocessor conditional #ifndef <user-defined keyword>
#    ... #endif.  csplit.pl puts the ROM component and all internal RAM
#    sections associated with the same #ifndef identifier into separate
#    files.  External references within the files should have been
#    resolved.
#
# USAGE:  perl csplit.pl -f [makefile] [list of C sources] [list of #ifndef identifiers]
#
# Copyright (c) 2000 QUALCOMM Inc.  All Rights Reserved.
# QUALCOMM Proprietary.  Export of this technology or software is
# regulated by the U.S. Government.  Diversion contrary to U.S. law
# prohibited.
#
# ------------------------------------------------------------------------
#
#                               Edit History
#
# $Header:   L:/src/asw/MSM5000/VCS/csplit.plv   1.14   20 Feb 2001 12:09:30   marcust  $
#
# when       who     what, where, why
# --------   ---     -----------------------------------------------------
# 02/20/01    mt     Remove 'system "del $targetdir$file";'.
# 01/31/01    mt     Fix infinite recompilation due to an insertion of a 
#                    blank line in a source file that does not compile
#                    successfullly.
# 10/13/00    mt     Fix parsing initialized string variables.
# 09/28/00    mt     Add Win98 stderr to stdout redirection.  2>&1 is not
#                    supported in Win98.  Remove csplit.pl's dependence on
#                    touch.
# 09/18/00    mt     Do not output function prototypes to internal RAM
#                    sources. 
# 09/06/00    mt     Refix check for C function prototypes.  Fix parsing 
#                    function pointer variable declarations.
# 09/05/00    mt     Change how the internal RAM source files are created.
#                    In doing so, csplit.pl no longer needs diff.  Remove
#                    csplit.pl's dependence on grep also.  Fix check for
#                    C function prototypes. 
# 07/26/00    mt     Get TARGETDIR as a command line option not from the
#                    input makefile.
# 07/14/00    mt     Do not cap the number of recompilations when trying
#                    to resolve the external reference errors.  As long as 
#                    the compiler makes progress from the previous
#                    compilation continue to recompile until the
#                    compilation is not abandoned.
# 07/14/00    mt     Terminate script if the files it will generate
#                    already exist and are read only.  Print error message
#                    when C source or #ifndef identifier is not specified.
# 07/06/00    mt     Generate files in TARGETDIR directory instead of in
#                    the current directory.
# 06/27/00    mt     Get the TARGETDIR value from the input makefile and
#                    get the compile command for $(TARGETDIR)\*.o not *.o.
# 06/23/00    mt     csplit.pl requires an input makefile and uses it to
#                    get the compile command for the current .c file.  Add
#                    a requirement to run this script with Perl version
#                    5.005 or greater.
# 06/14/00    mt     Initial revision.
#
# ========================================================================

require 5.005;
use Getopt::Long;
my %source_files;    # User's .c files that need to be processed.

my %identifiers;     # User's #ifndef identifiers.  Each entry is in the
                     # form of {Identifier, CurrentFile_Identifier_.c}.
                     # Items in this hash must be reinitialized before
                     # processing the next file in %source_files.

my %functions;       # Functions in the current .c file.  Each entry is in
                     # the form of {function, declaration}.  This hash must
                     # be undef before processing the next file in
                     # %source_files.

my %variables;       # Global or global to file variables in the current
                     # .c file.  Each entry is in the form of
                     # {variable, declaration}.  This hash must be undef
                     # before processing the next file in %source_files.

my %static_funcs;    # Static functions which need to be made global.  This
                     # hash must be undef before processing the next file in
                     # %source_files.

my %static_vars;     # Static variables which need to be made global.  This
                     # hash must be undef before processing the next file in
                     # %source_files.

my %undef_symbols;   # Undefined symbols of the outputed .c files.  Each
                     # entry is in the form of {file, hash} where each
                     # entry of the hash is in the form of
                     # {identifier, 'func'/'var'}.  This hash must be undef
                     # before processing the next file in %source_files.

my @code_lines;      # Scratch array for store lines of code.

my $target_command;  # Compile command to generate the .o file for the
                     # current *_.c file.  When DMSS code is split into its
                     # absolute and internal RAM components, we need to
                     # compile each component and parse through the compiler
                     # errors to resolve external references caused by the
                     # code split.  We cannot issue a general compile command
                     # because compiling DMSS code requires DMSS-related
                     # flags to be defined.  Otherwise, the compilation fails
                     # early without useful information.

my $targetdir;       # Value of TARGETDIR macro in the input makefile
                     # catenated with a \.

############################################
#youandi
my $sourcedir = 'Source\QUALCOMM30';
############################################

my $dir = 'temp';


# ------------------------------------------------------------------------
# SUBROUTINE:  print_hash
#
# DESCRIPTION:  Prints a hash table to stdout.
# ------------------------------------------------------------------------
sub print_hash
{
   my $key;

   foreach $key (keys %{$_[0]}) {
      print "$key:  ", $_[0]{$key}, "\n";
   }
   print "\n";
}


# ------------------------------------------------------------------------
# SUBROUTINE:  undef_hash_entries
#
# DESCRIPTION:  Sets the value of all keys of a hash table to undef.
# ------------------------------------------------------------------------
sub undef_hash_entries
{
   my $key;

   foreach $key (keys %{$_[0]}) {
      $_[0]{$key} = undef;
   }
}


# ------------------------------------------------------------------------
# SUBROUTINE:  get_dmss_compile_command
#
# DESCRIPTION:  Executes nmake to get the compile command for the current
#    *_.c file.  This command (with some changes) is used to preprocess
#    the original C source in the algorithm that splits the source into
#    its ROM and internal RAM components.  Additionally, the modified
#    compile command is used to get the unresolved externals that exists
#    between the ROM and internal RAM components.
# ------------------------------------------------------------------------
sub get_dmss_compile_command
{
   (my $stem = $_[0]) =~ s/\.c//;
   my $touch = 0;
   $target_command = '';

   if (!-e "$targetdir$stem\_.c") {
      open IRAM_SOURCE, ">$targetdir$stem\_.c";
      close IRAM_SOURCE;
      $touch = 1;
   }
   open NMAKE_OUTPUT, "nmake -anc -f $opt_file $targetdir$stem\_.o |";
   while (<NMAKE_OUTPUT>) {
      #print $_;
      s/^\s*//;
      if (/^tcc/ || /^armcc/) {
         s/-\s*o\s+.*?\s+//;   # Remove -o option and its parameter
         s/\Q$targetdir//;
         chomp;
         $target_command = $_;
      }
   }
   close NMAKE_OUTPUT;
#   system "del $targetdir$stem\_.c" if ($touch);
   if (!$target_command) {
      die "ERROR:  Can't get compile command for $targetdir$stem\_.o\n";
   }
}


# ------------------------------------------------------------------------
# SUBROUTINE:  check_output_files_stat
#
# DESCRIPTION:  Checks that the files the script will generate are not
#    read only if they already exist.
# ------------------------------------------------------------------------
sub check_output_files_stat
{
   (my $stem = $_[0]) =~ s/\.c//;
   die "ERROR:  $targetdir$stem\_.c is read only\n"
      if (-e "$targetdir$stem\_.c" && !-W "$targetdir$stem\_.c");
   foreach (keys %identifiers) {
      die "ERROR:  $targetdir$stem\_$_\_.c is read only\n"
         if (defined($identifiers{$_}) && -e "$targetdir$stem\_$_\_.c" &&
             !-W "$targetdir$stem\_$_\_.c");
   }
}


# ------------------------------------------------------------------------
# SUBROUTINE:  process_arguments
#
# DESCRIPTION:  Processes the arguments of the script file.  The argument
#    list is expected to be a list of file names followed by a list of
#    preprocessor conditional #ifndef labels.  The file names are put in a
#    hash.  File names with wild cards are expanded and the individual
#    file names the wild card covers are put in the hash.  Likewise, the
#    #ifndef labels are put in a hash.
# ------------------------------------------------------------------------
sub process_arguments
{
   my $end_of_file_list = 0;
   my @filenames = ();
   my $file;
   my $i;

   for $i (0 .. $#ARGV) {
      if (!$end_of_file_list) {
         if ($ARGV[$i] =~ /(.*)\.c$/) {
             push (@filenames, $ARGV[$i]);
         }
         else {
            $end_of_file_list = 1;
            $identifiers{$ARGV[$i]} = undef if (!exists $identifiers{$ARGV[$i]});
         }
      }
      else {
         $identifiers{$ARGV[$i]} = undef if (!exists $identifiers{$ARGV[$i]});
      }
   }

   foreach $file (@filenames) {
      if ($file =~ /\*/) {
         # file name contains a wild card
         while (glob $file) {
            if (/_(\w*)\.c$/) {
               if ($1 ne "" && !exists $identifiers{$1} && !exists $source_files{$_}) {
                  $source_files{$_} = undef;
               }
            }
            else {
               if (!exists $source_files{$_}) {
                  $source_files{$_} = undef;
               }
            }
         }
      }
      elsif (!exists $source_files{$file}) {
         $source_files{$file} = undef;
      }
   }
}


# ------------------------------------------------------------------------
# SUBROUTINE:  keyword_in_file
#
# DESCRIPTION:  Greps a C file for preprocessor conditional #ifndef
#    statements on user supplied keyword(s).  The function returns 1 if
#    a #ifndef name statement is found where name is one of the keywords
#    supplied as an argument to the parser.  Otherwise, 0 is returned.
# ------------------------------------------------------------------------
sub keyword_in_file
{
   my $keyword_found = 0;
   my $search_str = '^#ifndef\s+(';

   foreach (keys %identifiers) {
      $search_str .= $_ . '|';
   }
   chop $search_str;
   $search_str .= ')';

   undef_hash_entries(\%identifiers);

   $_[0] =~ /(\w+)\.c/;
   my $stem = $1;
   
   ############################################
   #youandi
   my $temp_sourcedir;
   $temp_sourcedir = $sourcedir.$_[0];
   #print "-->1.0:$temp_sourcedir\n";
   open C_SOURCE, "$temp_sourcedir";
   ############################################  
   #open C_SOURCE, "$_[0]";

   while(<C_SOURCE>) {
      if (/$search_str/ && exists $identifiers{$1}) {
         $identifiers{$1} = "$stem\_$1_.c";
         $keyword_found = 1;
      }
   }

   close C_SOURCE;

   return $keyword_found;
}


# ------------------------------------------------------------------------
# SUBROUTINE:  parse_variable_declaration
#
# DESCRIPTION:  Takes a C variable declaration and separates the
#    variable(s) from the type.  Assumes the input line contains a single
#    declaration.  int a, b; is handled but int a; int b; is not. 
# ------------------------------------------------------------------------
sub parse_variable_declaration
{
   my ($line, $vars, $type) = ($_[0], $_[1], $_[2]);
   @$vars = ();

   # Remove array initializer, string initializer, variable initializer that
   # may involve casting
   $line =~ s/=\s*{.*}//g;
   $line =~ s/=\s*\".*\"//g;
   $line =~ s/=\s*(\(.*\))*\s*\w+\s*;\s*$/;/g;

   # Remove spaces between nonword characters so that function pointer
   # declarations are easier to parse.
   while ($line =~ s/(\W)\s+(\W)/$1$2/g) {};
   #print "LINE:  $line\n";

   my @byte_stream = split //, $line;
   my @char_collector = ();
   my $char;
   my $state = 0;
   my $var;
   my $paren_cnt = 0;
   my $i = $#{@byte_stream};
   
   WHILE:
   while ($i >= 0) {
      $char = $byte_stream[$i];

      SWITCH: {
         $state == 0 && $char == ';' && do {
            $state = 1;
            last SWITCH;
         };

         $state == 1 && do {
            if ($char =~ /\w/) {
               unshift @char_collector, $char;
            }
            elsif ($char =~ /\s/ &&
                  (($var = join '', @char_collector) =~ /^\w|^\(\*\s*\w+\s*\)/) &&
                  # Check that what we have collected so far is a word (identifier)
                  # or a function pointer
                  $paren_cnt == 0) {
               $state = 2;
               #print "VARIABLE 0:  $var\n";
               unshift @$vars, $var;
               @char_collector = ();
            }
            elsif ($char eq '*') {
               if ($paren_cnt != 0) {
                  unshift @char_collector, $char;
               }
               else {
                  $state = 2;
                  $var = join '', @char_collector;
                  #print "VARIABLE 1:  $var\n";
                  unshift @$vars, $var;
                  @char_collector = ();
                  $i++;
               }
            }
            elsif ($char eq ',') {
               $var = join '', @char_collector;
               #print "VARIABLE 2:  $var\n";
               unshift @$vars, $var;
               @char_collector = ();
            }
            elsif ($char eq ']') {
               unshift @char_collector, $char;
               $state = 3;
            }
            elsif ($char eq '}') {
               $var = join '', @char_collector;
               #print "VARIABLE 3:  $var\n";
               unshift @$vars, $var;
               @char_collector = ();
               splice @byte_stream, $i+1;
               last WHILE;
            }
            elsif ($char eq ')') {
               $paren_cnt++;
               unshift @char_collector, $char;
            }
            elsif ($char eq '(') {
               $paren_cnt--;
               unshift @char_collector, $char;
            }
            last SWITCH;
         };

         $state == 2 && do {
            if ($char eq ',') {
               $state = 1;
            }
            elsif ($char =~ /[\w}\*]/) {
               # done with variables
               splice @byte_stream, $i+1;
               last WHILE;
            }
            last SWITCH;
         };

         $state == 3 && do {
            unshift @char_collector, $char if ($char !~ /\s/);
            if ($char eq '[') {
               $state = 1;
            }
            last SWITCH;
         };
      }
      $i--;
   }

   $$type = join '', @byte_stream;
   #print "TYPE: $$type\n";
}


# ------------------------------------------------------------------------
# SUBROUTINE:  put_variable_in_hash
#
# DESCRIPTION:  Takes a C variable declaration and puts the variable in a
#    hash as a key with its type as the value if the variable is not
#    in the hash.  The function returns 1 if a variable is put into the
#    hash, 0 otherwise.
# ------------------------------------------------------------------------
sub put_variable_in_hash
{
   my $line = $_[0];
   my $type = '';
   my $ret_val = 1;

   parse_variable_declaration($line, \@vars, \$type);
   print "TYPE: $type\n";

   # Array variables generate two entries in the %variables hash,
   # variable name followed by one or more [n] => type
   # variable name without [n] => one or more [n]
   # For external reference errors to arrays, the compiler merely provides
   # the variable name.  However, we need to resolve with the array
   # dimensions, e.g, extern int array[][][]; not just extern int array;
   # We store the number of brace pairs in the hash so we can resolve
   # arrays correctly later.

   my $open_brace;
   my $remaining_braces = '';
   for (@vars) {
         print "aaaaaaaaaaaaaaaaaaa: $_\n";   	
      if (exists $variables{$_}) {
         print "bbbbbbbbbbbbbbbbbbb: $variables{$_}\n";
         $ret_val = 0;
         last;
      }
      $variables{$_} = $type;
      ($var, $open_brace, $remaining_braces) = split /(\[)/, $_, 2;
      if ($remaining_braces ne '') {
         print "cccccccccccccccccccc : $open_brace.$remaining_braces\n";      	
         $variables{$var} = $open_brace.$remaining_braces;
      }
         print "dddd: $var, $open_brace, $remaining_braces\n";   	      
   }

   return $ret_val;
}


# ------------------------------------------------------------------------
# SUBROUTINE:  put_function_in_hash
#
# DESCRIPTION:  Takes a C function definition and extracts the function
#    name.  The function name is put in a hash as a key with its
#    declaration as the value if the function is in the hash.  This
#    function returns 1 if an entry is put into the hash, 0 otherwise. 
# ------------------------------------------------------------------------
sub put_function_in_hash
{
   my $ret_val = 0;

   $_[0] =~ /(\w+)\s*\(.*\)$/;

   if (!exists $functions{$1}) {
      $functions{$1} = $_[0];
      $ret_val = 1;
   }

   return $ret_val;
}


# ------------------------------------------------------------------------
# SUBROUTINE:  remove_statics
#
# DESCRIPTION:  Removes the static qualifiers on those functions and
#    variables found in the %static_funcs and %static_vars hashes,
#    respectively, from the ROM component and internal RAM component
#    sources.
# ------------------------------------------------------------------------
sub remove_statics
{
   my @files = ();
   my $file;
   my $i;
   my $prev_line = '';
   my $line = '';
   my $str;
   my $static_pos = -1;
   my $open_paren_cnt = 0;
   my $close_paren_cnt = 0;
   my $open_brace_cnt = 0;
   my $close_brace_cnt = 0;
   my @vars;
   my @code;
   my $type;

   ($_ = $_[0]) =~ s/\.c$//;
   push @files, "$targetdir$_\_.c";
   foreach (keys %identifiers) {
      if (defined $identifiers{$_}) {
         push @files, "$targetdir$identifiers{$_}";
      }
   }

   while ((%static_vars || %static_funcs) && defined($file = shift @files)) {
      next if (!-e $file);
      print "Remove some static qualifiers in $file\n";
      open $file, $file or die "ERROR:  Could not open $file\n";
      @code = <$file>;
      close $file;

      for ($i = 0; $i <= $#code; $i++) {
         $str = $code[$i];
         next if ($str =~ /^\s*$/);

         $static_pos = $i if ($str =~ /^\s*static\s+/ ||
                              $str =~ /^\s*static$/);
         next if ($static_pos == -1);

         chomp $str;
         $line .= $str;

         $line =~ s/\s*$//;          # delete trailing white space
         $line =~ s/^\s+//;          # delete leading white space

         if ($open_brace_cnt == 0) {
            $open_paren_cnt = $line =~ tr/(//;
            $close_paren_cnt = $line =~ tr/)//;
            next if ($open_paren_cnt != $close_paren_cnt);
         } 

         if ($open_brace_cnt == 0) {
            $open_brace_cnt = $line =~ tr/{//;
            $close_brace_cnt = $line =~ tr/}//;
         }
         else {
            $open_brace_cnt += $str =~ tr/{//;
            $close_brace_cnt += $str =~ tr/}//;
         }
         next if ($open_brace_cnt != $close_brace_cnt);
         $open_brace_cnt = $close_brace_cnt = 0;

         if ($line =~ /;$/) {
            $line = $prev_line." $line";
            $line =~ s/^\s+//;                  # delete leading white space
            if ($line =~ /^typedef/) {
               #print "typedef:  $line\n";
            }
            elsif ($line =~ /^extern/) {
               #print "extern var/func:  $line\n";
            }
            elsif ($line =~ /\w+\s*\(.*?\)\s*;$/) {
               # print "static prototype:  $line\n";
               $line =~ /(\w+)\s*\(.*?\);$/;
               if (exists $static_funcs{$1}) {
                  $static_funcs{$1}++;
                  if ($code[$static_pos] =~ /^\s*static\s*$/) {
                     $i -= ($i - $static_pos + 1);
                  }
                  $code[$static_pos] =~ s/static\s*//;
               }
               $static_pos = -1;
            }
            else {
               #print "variable declaration:  $line\n";
               parse_variable_declaration($line, \@vars, \$type); 
               foreach (@vars) {
                  if (exists $static_vars{$_} && $static_pos != -1) {
                     delete $static_vars{$_};
                     if ($code[$static_pos] =~ /^\s*static\s*$/) {
                        $i -= ($i - $static_pos + 1);
                     }
                     $code[$static_pos] =~ s/static\s*//;
                     last;
                  }
               }
               $static_pos = -1;
            }
            $prev_line = $line = '';
            next;
         }

         # line does not end with a ;

         if ($line =~ /}$/) {
            if ($prev_line ne '' && $prev_line =~ /\)$/) {
               # print "function 1:  $prev_line\n";
               $prev_line =~ /(\w+)\s*\(.*\)$/;
               if (exists $static_funcs{$1} && $static_pos != -1) {
                  $static_funcs{$1}++;
                  if ($code[$static_pos] =~ /^\s*static\s*$/) {
                     $i -= ($i - $static_pos + 1);
                  }
                  $code[$static_pos] =~ s/static\s*//;
               }
               $prev_line = $line = '';
               $static_pos = -1;
               next;
            }
            elsif ($line =~ /\w+\s*\(.*\)\s*\{/) {
               # print "function 2:  $line\n";
               $line =~ s/\s*{.*}//;
               $line =~ /(\w+)\s*\(.*\)$/;
               if (exists $static_funcs{$1} && $static_pos != -1) {
                  $static_funcs{$1}++;
                  if ($code[$static_pos] =~ /^\s*static\s*$/) {
                     $i -= ($i - $static_pos + 1);
                  }
                  $code[$static_pos] =~ s/static\s*//;
               }
               $prev_line = $line = '';
               $static_pos = -1;
               next;
            }
         }

         $prev_line .= " $line";
         $line = '';
      }

      open $file, ">$file";
      print $file @code;
      close $file;

      if (%static_funcs) {
         # We should be removing the static keyword on a function no more than
         # twice, once in the function definition and perhaps another in its
         # prototype.  Each time we the remove the static keyword, we increment
         # its hash value.  Now we check this value.  If it is > 1, then we can
         # delete it from the hash to prevent unnecessary searching.
         foreach (keys %static_funcs) {
            delete $static_funcs{$_} if ($static_funcs{$_} > 1);
         }
      }
   }
}


# ------------------------------------------------------------------------
# SUBROUTINE:  insert_undef_symbols
#
# DESCRIPTION:  Inserts function declarations and variable extern
#    declarations in the 'separated-C-source-file' output files just
#    before the first function.  This should allow the output files to
#    compile without an unresolved reference error.
# ------------------------------------------------------------------------
sub insert_undef_symbols
{
   my ($code, $prototypes, $extern_vars) = ($_[0], $_[1], $_[2]);
   my $i;
   my $prev_line = '';
   my $line = '';
   my $first_func_pos = 0;
   my $open_brace_cnt = 0;
   my $close_brace_cnt = 0;
   my $open_paren_cnt = 0;
   my $close_paren_cnt = 0;

   my @code_lines = ();

   for $i (0 .. $#$code) {
      push @code_lines, @$code[$i];
      next if (@$code[$i] =~ /^\s*$/);

      $line .= @$code[$i];
      chomp $line;

      $line =~ s/\s*$//;          # delete trailing white space
      $line =~ s/^\s+//;          # delete leading white space
 
      # C preprocessor directive
      if ($line =~ /^#include/) {
         @code_lines = ();
         $prev_line = $line = '';
         next;
      }

      if ($open_brace_cnt == 0) {
         $open_paren_cnt = $line =~ tr/(//;
         $close_paren_cnt = $line =~ tr/)//;
         next if ($open_paren_cnt != $close_paren_cnt);
      }

      if ($open_brace_cnt == 0) {
         $open_brace_cnt = $line =~ tr/{//;
         $close_brace_cnt = $line =~ tr/}//;
      }
      else {
         $open_brace_cnt += $code_lines[$#code_lines] =~ tr/{//;
         $close_brace_cnt += $code_lines[$#code_lines] =~ tr/}//;
      }
      next if ($open_brace_cnt != $close_brace_cnt);
      $open_brace_cnt = $close_brace_cnt = 0;

      if ($line =~ /;$/) {
         @code_lines = ();
         $prev_line = $line = '';
         next;
      }

      # line does not end with a ;

      if ($line =~ /}$/) {
         if ($prev_line ne '' && $prev_line =~ /\)$/) {
            #print "function 1:  $prev_line\n";
            $first_func_pos = $i - $#code_lines;
            $prev_line = $line = '';
            @code_lines = ();
            last;
         }
         elsif ($line =~ /\w+\s*\(.*\)\s*\{/) {
            #print "function 2:  $line\n";
            $first_func_pos = $i - $#code_lines;
            $prev_line = $line = '';
            @code_lines = ();
            last;
         }
      }

      $prev_line .= " $line";
      $line = '';
   }

   #print "FIRST FUNCTION POS: $first_func_pos\n";

   if (@$code[$first_func_pos] !~ /^\s*$/) {
      splice @$code, $first_func_pos, 0, "\n";
   }
   if (@$code[$first_func_pos - 1] !~ /^\s*$/) {
      unshift @$prototypes, "\n";
   }
   splice @$code, $first_func_pos, 0, @$extern_vars;
   splice @$code, $first_func_pos, 0, @$prototypes;
}


# ------------------------------------------------------------------------
# SUBROUTINE:  insert_external_refs
#
# DESCRIPTION:  Inserts function prototype definitions and extern variable
#    declarations in those files that reference functions and variables
#    that the files themselves do not define.
# ------------------------------------------------------------------------
sub insert_external_refs
{
   my $file = $_[0];
   my $symbol;
   my @code;
   my @prototypes;
   my @extern_vars;
   my $type;

   if (%{$undef_symbols{$file}}) {
      print "Insert external references in $file\n";
      @code = ();

      open $file, $file or die "ERROR:  Could not open $file\n";
      @code = <$file>;
      close $file;

      @extern_vars = ();
      @prototypes = ();
      foreach $symbol (keys %{$undef_symbols{$file}}) {
         print "SYMBOL:  $symbol, $undef_symbols{$file}{$symbol}\n";
         if ($undef_symbols{$file}{$symbol} eq 'var') {
            $type = $variables{$symbol};
            $type =~ s/static\s*//;
            push @extern_vars, "extern $type $symbol;\n";
         }
         else {
            $type = $functions{$symbol};
            $type =~ s/static\s*//;
            push @prototypes, "$type;\n";
         }
      }

      #print "FILE:  $file\n";
      #print "PROTO:  @prototypes\n";
      #print "EXTERN VARS:  @extern_vars\n";

      insert_undef_symbols(\@code, \@prototypes, \@extern_vars);

      if ($#code != -1) {
         open $file, ">$file";
         print $file @code;
         close $file;
      }
      delete $undef_symbols{$file};
   }
}


# ------------------------------------------------------------------------
# SUBROUTINE:  process_compiler_output
#
# DESCRIPTION:  Compiles a C source file and extracts function and
#    variable reference errors caused by functions being relocated to
#    multiple files.  The undefined functions and varibles are put in the
#    %undef_symbols hash.
# ------------------------------------------------------------------------
my $prev_line_in_error = 0;
sub process_compiler_output
{
   my ($file, $id_str) = ($_[0], $_[1]);
   my $symbol;
   my $filename;
   my $cc;
   my $ret_val = 0;
   my $line_in_error = 0;
   my $compilation_abandoned = 0;

#			print "-->7.00:$_\n";

   ($_ = $file) =~ s/.*?\\//g;

#   			print "-->7.01:$_\n";
   			
   s/($id_str)$//;
   ($cc = $target_command) =~ s/$_\_\.c$/$file/g;
#   print "CMD: $cc\n";
   print "Compile $file\n";

   if ($ENV{OS} eq "Windows_NT") {
      open TCC_OUTPUT, "$cc -o $dir\\object.o 2>&1|";
#   			print "-->7.02:$_\n";      	      
   }
   else {
#   			print "-->7.03:$_\n";      	   	
      # Win98 workaround for redirecting stderr to stdout
      open SAVEERR, ">&STDERR";        # Save stderr 
      open SAVEOUT, ">&STDOUT";        # Save stdout

      # Redirect stdout to temp\tcc.out and stderr to stdout
      open STDOUT, ">$dir\\tcc.out" or die "ERROR:  Cannot dup stdout";
      open STDERR, ">&STDOUT" or die "ERROR:  Cannot dup stdout";

      select STDERR; $| = 1;           # Make stderr and stdout unbuffered to ensure 
      select STDOUT; $| = 1;           # correct output order between the two.

      system "$cc -o $dir\\object.o";
      open TCC_OUTPUT, "$dir\\tcc.out";
   }
   			print "-->7.04:$_\n";      	
   while (<TCC_OUTPUT>) {
   			print "-->7.05:$_\n";   	
#      print "$_\n";
      if (/"(.*?)".*line (\d+).*C2456E.*'extern int (\w+)'/) {
         print "$1 $2 $3\n";
         # Append as many [] pairs as needed on array variables.
         $filename = $1;
         $line_in_error = $2;
         $symbol = $3;
         if (exists $variables{$symbol} && $variables{$symbol} =~ /^\[/) {
            $symbol .= $variables{$symbol};
         print "11111111111111111111111111\n";
         }

         if (!exists $undef_symbols{$filename}{$symbol}) {
         print "2222222222222222222222222\n";
            if (exists $variables{$symbol}) {
         print "3333333333333333333333333\n";
               $undef_symbols{$filename}{$symbol} = 'var';
               if ($variables{$symbol} =~ /static/ &&
                   !exists $static_vars{$symbol}) {
         print "4444444444444444444444444\n";
                  $static_vars{$symbol} = undef;
               }
            }
            elsif (exists $functions{$symbol}) {
         print "5555555555555555555555555\n";			                     	            	
               $undef_symbols{$filename}{$symbol} = 'func';
               if ($functions{$symbol} =~ /static/
                   && !exists $static_funcs{$symbol}) {
         print "6666666666666666666666666\n";
                  $static_funcs{$symbol} = 0;
               }
            }
         }
            			print "-->7.06:$_\n";
      }
      elsif (/"(.*?)".*line (\d+).*C2207W.*'extern int (\w+)\(.*\);/) {
         print "$1 $2 $3\n";
         $filename = $1;
         $line_in_error = $2;

         if (!exists $undef_symbols{$1}{$3} && exists $functions{$3}) {

            # Do an exist check on the function $3 just in case there is a 
            # reference to function $3 that does not exist in the file and
            # is not 'prototyped.'  The original source itself should not
            # have compiled and the the parser will ignore this undefined
            # function.  If we do not do the exist check on function $3,
            # function $3 gets added to %undef_symbols as well as %functions
            # when we do the if ($functions{$symbol} =~ /static/ ...)
            # check below.  Later when the parser goes through
            # %undef_symbols, it will want to insert 'extern prototype
            # function $3;' in file $1 to fix the compile error.  However,
            # it won't not have the prototype for function $3 because during
            # process_file the parser never encountered a
            # definition/declaration for function $3.

            $undef_symbols{$1}{$3} = 'func';
            $symbol = $3;
            if ($functions{$symbol} =~ /static/
                && !exists $static_funcs{$symbol}) {
               $static_funcs{$symbol} = 0;
            }
         }
   			print "-->7.07:$_\n";         
      }
      elsif (/"(.*?)".*line (\d+).*C2937E: static function '(\w+)'/) {
         print "$1 $2 $3\n";
         $filename = $1;
         $line_in_error = $2;

         # This case occurs when a static function and its static prototype
         # has been moved to a separate file.  However, the static function
	 # is referenced from another file.  In such cases the function and
         # its prototype need to have the static qualifier removed.

         if (!exists $undef_symbols{$1}{$3} && exists $functions{$3}) {
            $undef_symbols{$1}{$3} = 'func';
            $symbol = $3;
            if ($functions{$symbol} =~ /static/ &&
                !exists $static_funcs{$symbol}) {
               $static_funcs{$symbol} = 0;
            }
         }
   			print "-->7.08:$_\n";         
      }
      elsif (/Compilation abandoned/) {
         $compilation_abandoned = 1;
   			print "-->7.09:$_\n";         
      }
   }
   close TCC_OUTPUT;
   	print "-->7.10:\n";            
      print_hash(\%{$undef_symbols{$filename}});   
   	print "-->7.10:\n";            
   foreach $symbol (keys %{$undef_symbols{$filename}}  ){
   	print "-->7.11:$symbol\n";         
   }

   if ($ENV{OS} ne "Windows_NT") {
      # Win98 workaround for redirecting stderr to stdout
      # Restore stdout and stderr to their original state 
      close STDERR;                    # Close the redirected stderr and stdout
      close STDOUT;
      open STDERR, ">&SAVEERR";        # Restore stderr and stdout
      open STDOUT, ">&SAVEOUT";
      close SAVEERR;                   # Close copy of stderr and stdout;
      close SAVEOUT;
   }

   if ($compilation_abandoned) {
      if ($filename =~ /\.c$/ && $prev_line_in_error < $line_in_error) {
         $prev_line_in_error = $line_in_error;
         $ret_val = 1;
      }
      else {
         $ret_val = 2;
      }
   }
   else {
      $prev_line_in_error = 0;
      $ret_val = 0;
   }

   unlink "$dir\\object.o" if (-e "$dir\\object.o");
   return $ret_val;
}


# ------------------------------------------------------------------------
# SUBROUTINE:  resolve_external_references
#
# DESCRIPTION:  Resolves external references that results from splitting a
#    C source into its ROM and internal RAM components and putting them in
#    separate files.  This subroutine calls for each of the C source files
#    to get compiled to determine the unresolved references and tweaked
#    with added declarations to fix the unresolved reference errors.  This
#    compile/tweak cycle may be done multiple times because earlier
#    compilations may yield so many errors that the compiler quits early.
#    Based on the return value of process_compiler_output, we either
#    1) fix what we can and recompile the same source to get further in
#       the compilation,
#    2) compile the next C file or
#    3) terminate the script because of a compile error the script cannot
#       fix.
# ------------------------------------------------------------------------
sub resolve_external_references
{
   $_[0] =~ s/\.c//;
   my $id;
   my $id_str = '_.c|';
   my $file;
   my @filenames = ("$targetdir$_[0]_.c");

   $undef_symbols{"$targetdir$_[0]_.c"} = undef;

   foreach $id (keys %identifiers) {
      if (defined $identifiers{$id}) {
         $undef_symbols{"$targetdir$identifiers{$id}"} = undef;
         push @filenames, "$targetdir$identifiers{$id}";
         $id_str .= "_$id\_.c\|";
      }
   }

   chop $id_str;

   foreach $file (@filenames) {
      while (1) {
         $_ = process_compiler_output($file, $id_str);
#					print "-->7.0: $file, $id_str, $_:\n";               
         if ($_ == 0) {
            # Compiler did not abandon compilation.  We should have gotten
            # all the unresolved external errors for this C file.
            insert_external_refs($file);
            last;
         }
         elsif ($_ == 1) {
            # Compiler abandoned compilation on a C file but the last line
            # in error is greater than the last line in error from a
            # previous compile of the same C file.  This means that we are
            # progressing.  Recompile the same C source to get more
            # unresolved external errors.
            insert_external_refs($file);
         }
         else {
            # Compiler abandoned compilation on a header file or on C file
            # but the compiler did not make progress.
            die "ERROR:  Cannot fix $file\n";
         }
      }
   }

   foreach (keys %undef_symbols) {
      print "FILE:  $_\n";
      print_hash(\%{$undef_symbols{$_}});
   }

   #print "STATIC VARS:\n";
   #print_hash(\%static_vars);
   #print "STATIC FUNCS:\n";
   #print_hash(\%static_funcs);
}


# ------------------------------------------------------------------------
# SUBROUTINE:  collect_rom_c_objects
#
# DESCRIPTION:  Scans a ROM C preprocessed file and looks for C objects,
#    variable declarations, function definitions.  These C objects are
#    stored in hashes for later reference.
# ------------------------------------------------------------------------
sub collect_rom_c_objects
{
   $_[0] =~ /(\w+)\.c/;
   my $stem = $1.'_';

   my $prev_line = '';
   my $line = '';
   my $open_brace_cnt = 0;
   my $close_brace_cnt = 0;
   my $open_paren_cnt = 0;
   my $close_paren_cnt = 0;

   my @code_lines = ();

   open ROM_SOURCE, "$dir\\trim_$stem.c" or die "ERROR:  Could not open $dir\\trim_$stem.c\n";

   while (<ROM_SOURCE>) {
      push @code_lines, $_;
      next if (/^\s*$/);
      
      chomp;
      $line .= $_;

      $line =~ s/\s*$//;          # delete trailing white space
      $line =~ s/^\s+//;          # delete leading white space

      if ($line =~ /^#/) {
         $prev_line = $line = '';
         @code_lines = ();
         next;
      }
 
      if ($open_brace_cnt == 0) {
         $open_paren_cnt = $line =~ tr/(//;
         $close_paren_cnt = $line =~ tr/)//;
         next if ($open_paren_cnt != $close_paren_cnt);
      }

      if ($open_brace_cnt == 0) {
         $open_brace_cnt = $line =~ tr/{//;
         $close_brace_cnt = $line =~ tr/}//;
      }
      else {
         $open_brace_cnt += $code_lines[$#code_lines] =~ tr/{//;
         $close_brace_cnt += $code_lines[$#code_lines] =~ tr/}//;
      }
      next if ($open_brace_cnt != $close_brace_cnt);
      $open_brace_cnt = $close_brace_cnt = 0;

      if ($line =~ /;$/) {
         $line = $prev_line." $line";
         $line =~ s/^\s+//;                  # delete leading white space
         if ($line =~ /^typedef/) {
            print "typedef:  $line\n";
         }
         elsif ($line =~ /^extern/) {
            print "extern var/func:  $line\n";
         }
         elsif ($line =~ /\(\s*\*\s*\w+\s*\)\s*\(.*?\)\s*;$/) {
            print "function pointer:  $line\n";
            put_variable_in_hash($line);
         }
         elsif ($line =~ /\w+\s*\(.*?\)\s*;$/) {
            print "prototype:  $line\n";
         }
         elsif ($line =~ /^enum\s*\{/ && $line =~ /\}\s*;$/) {
            # Found a non-variable enum that servers like a list of #defines,
            # e.g., enum {A=0, B=0};
         }
         else {
            print "variable: $line\n";
            put_variable_in_hash($line);
         }
         $prev_line = $line = '';
         @code_lines = ();
         next;
      }

      # line does not end with a ;

      if ($line =~ /}$/) {
         if ($prev_line ne '' && $prev_line =~ /\)$/) {
            $prev_line =~ s/^\s+//;          # delete leading white space
            print "function 1:  $prev_line\n";
            put_function_in_hash($prev_line);
            $prev_line = $line = '';
            @code_lines = ();
            next;
         }
         elsif ($line =~ /\w+\s*\(.*\)\s*\{/) {
            $line =~ s/\s*{.*}//;
            print "function 2:  $line\n";
            put_function_in_hash($line);
            @code_lines = ();
            $prev_line = $line = '';
            next;
         }
      }

      $prev_line .= " $line";
      $line = '';
   }

   close ROM_SOURCE;
   rename "$dir\\trim_$stem.c", "$targetdir$stem.c";
}


# ------------------------------------------------------------------------
# SUBROUTINE:  create_iram_sources
#
# DESCRIPTION:  Extracts out the ROM component from the C preprocessed
#    files that contained the ROM and internal RAM components, leaving
#    behind #includes, typedefs, etc. and of course the internal RAM
#    component.  Any new C objects (functions and variables) that we find
#    in the internal RAM components are added to their respective hashes. 
#
# ------------------------------------------------------------------------
sub create_iram_sources
{
   $_[0] =~ /(\w+)\.c/;
   my $stem = $1.'_';
   # $_ contains a #ifndef identifier

   my $prev_line = '';
   my $line = '';
   my $open_brace_cnt = 0;
   my $close_brace_cnt = 0;
   my $open_paren_cnt = 0;
   my $close_paren_cnt = 0;

   my @code_lines = ();

   open C_PREPROC_SOURCE, "$dir\\trim_$stem$_\_.c" or die "ERROR:  Could not open $dir\\trim_$stem$_\_.c\n";
   open IRAM_SOURCE, ">$targetdir$stem$_\_.c";

   while (<C_PREPROC_SOURCE>) {
      push @code_lines, $_;
      next if (/^\s*$/);

      chomp;
      $line .= $_;

      $line =~ s/\s*$//;          # delete trailing white space
      $line =~ s/^\s+//;          # delete leading white space

      if ($line =~ /^#/) {
         print IRAM_SOURCE @code_lines;
         $prev_line = $line = '';
         @code_lines = ();
         next;
      }
 
      if ($open_brace_cnt == 0) {
         $open_paren_cnt = $line =~ tr/(//;
         $close_paren_cnt = $line =~ tr/)//;
         next if ($open_paren_cnt != $close_paren_cnt);
      }

      if ($open_brace_cnt == 0) {
         $open_brace_cnt = $line =~ tr/{//;
         $close_brace_cnt = $line =~ tr/}//;
      }
      else {
         $open_brace_cnt += $code_lines[$#code_lines] =~ tr/{//;
         $close_brace_cnt += $code_lines[$#code_lines] =~ tr/}//;
      }
      next if ($open_brace_cnt != $close_brace_cnt);
      $open_brace_cnt = $close_brace_cnt = 0;

      if ($line =~ /;$/) {
         $line = $prev_line." $line";
         $line =~ s/^\s+//;                  # delete leading white space
         if ($line =~ /^typedef/) {
            print "typedef:  $line\n";
            print IRAM_SOURCE @code_lines;
         }
         elsif ($line =~ /^extern/) {
            print "extern var/func:  $line\n";
            print IRAM_SOURCE @code_lines;
         }
         elsif ($line =~ /\(\s*\*\s*\w+\s*\)\s*\(.*?\)\s*;$/) {
            print "function pointer:  $line\n";
            if (put_variable_in_hash($line)) {
               print IRAM_SOURCE @code_lines;
            }
         }
         elsif ($line =~ /\w+\s*\(.*?\)\s*;$/) {
            print "prototype:  $line\n";
         }
         elsif ($line =~ /^enum\s*\{/ && $line =~ /\}\s*;$/) {
            # Found a non-variable enum that servers like a list of #defines,
            # e.g., enum {A=0, B=0};
         }
         else {
            print "variable: $line\n";
            if (put_variable_in_hash($line)) {
               print IRAM_SOURCE @code_lines;
            }
         }
         $prev_line = $line = '';
         @code_lines = ();
         next;
      }

      # line does not end with a ;

      if ($line =~ /}$/) {
         if ($prev_line ne '' && $prev_line =~ /\)$/) {
            $prev_line =~ s/^\s+//;          # delete leading white space
            print "function 1:  $prev_line\n";
            if (put_function_in_hash($prev_line)) {
               print IRAM_SOURCE @code_lines;
            }
            $prev_line = $line = '';
            @code_lines = ();
            next;
         }
         elsif ($line =~ /\w+\s*\(.*\)\s*\{/) {
            $line =~ s/\s*{.*}//;
            print "function 2:  $line\n";
            if (put_function_in_hash($line)) {
               print IRAM_SOURCE @code_lines;
            }
            @code_lines = ();
            $prev_line = $line = '';
            next;
         }
      }

      $prev_line .= " $line";
      $line = '';
   }

   close IRAM_SOURCE;
   close C_PREPROC_SOURCE;
}


# ------------------------------------------------------------------------
# SUBROUTINE:  trim_c_preprocessed_source
#
# DESCRIPTION:  Trims the C preprocessed file of blank lines and inline
#    expansion of header files.
# ------------------------------------------------------------------------
sub trim_c_preprocessed_source
{
   (my $file = $_[0]) =~ s/\\/\\trim_/;
   my $stem = $_[1];
   my $c_source_line = 0;           # Indicates whether the next line belongs
                                    # to the .c source or to the .h file that 
                                    # has been included and expanded.
   my $prev_line_is_blank = 0;
			
#			print "-->7.00:$_, $file, $_[0], $stem\n";
			
			
   open TRIMMED_SOURCE, ">$file";
   open SOURCE, "$_[0]" or die "ERROR:  Could not open $_[0]";
   while (<SOURCE>) {
#			print "-->7.11:$_\n";   	
      if (/^\s*#line\s+[0-9]+\s+(.*)$/) {
   ############################################  
   # youandi 				
         if ($1 eq "\"$sourcedir$stem.c\"") {
   ############################################         	
            $c_source_line = 1;
         }
         elsif ($c_source_line) {
   ############################################         	         	
   # youandi 					   
      	    my $includefilepath = $1;
#			print "-->7.22:$includefilepath,$sourcedir\n";      	          	    
      	    $includefilepath =~ s/Source\\QUALCOMM30\\//;
      	    $includefilepath =~ s/Source\\SKY\\B2\\INCLUDE\\//;
      	    $includefilepath =~ s/Source\\DS\\//;
      	    $includefilepath =~ s/Source\\SockApi\\//;
      	    $includefilepath =~ s/SOURCE\\IRDA\\LIB\\dcinc\\//;
      	    $includefilepath =~ s/SOURCE\\IRDA\\//;
      	    $includefilepath =~ s/SOURCE\\WAP_B2\\//;      	    
#			print "-->7.22:$includefilepath\n";      	    
            print TRIMMED_SOURCE "#include $includefilepath\n";
   ############################################
   #        print TRIMMED_SOURCE "#include $1\n";    
            $c_source_line = 0;
            $prev_line_is_blank = 0;
         }
         next;
      }
      
      if ($c_source_line) {
         if (/^\s*$/) {
            next if ($prev_line_is_blank);
            $prev_line_is_blank = 1;
         }
         else {
            $prev_line_is_blank = 0;
         }
#			print "-->7.33:$_\n";      	             
         print TRIMMED_SOURCE $_;
      }
   }
   close TRIMMED_SOURCE;
   close SOURCE;
}


# ------------------------------------------------------------------------
# SUBROUTINE:  create_c_preprocessed_files 
#
# DESCRIPTION:  Generates C preprocessed files for the ROM component and
#    each of the internal RAM components.  For a file foo.c, the ROM
#    component will be put in temp\foo_.c while each internal RAM
#    components will be put in temp\foo_*.c, where * is the #ifndef
#    identifier that demarcates the internal RAM component.  Note that the
#    C preprocessed files containing the internal RAM components also
#    include the ROM component.  The ROM component will have to be
#    stripped out later.  So if foo.c has a ROM component and two internal
#    RAM components demarcated by #ifndef IRAM1 ... #endif and #ifndef
#    IRAM2 ... #endif, three files will be produced, temp\foo_.c,
#    temp\foo_IRAM1_.c, and temp\foo_IRAM2_.c.
# ------------------------------------------------------------------------
sub create_c_preprocessed_files
{
   (my $stem = $_[0]) =~ s/\.c$//;
   my $cc;
   my $dash_D_symbols = "";
   my $dash_D_syms;

   foreach (keys %identifiers) {
      $dash_D_symbols .= " -D$_";
   }

   # Generate the C preprocessed source without any internal RAM code
   ($cc = $target_command) =~ s/_\.c/\.c/g;
   $cc .= " -E $dash_D_symbols > $dir\\$stem\_.c";

   ############################################  
   # youandi 
   my $temp_sourcedir;
   $temp_sourcedir = $sourcedir.$_[0];
   $cc =~ s/\s$stem\.c/ $temp_sourcedir/g;
   ############################################
   #print "CC: $cc\n";
   system $cc;

   trim_c_preprocessed_source("$dir\\$stem\_.c", $stem);

   foreach (keys %identifiers) {
      # Skip if current source file does not have an internal RAM component
      # demarcated by the current #ifndef identifier
      next if (!defined($identifiers{$_}));

      # Generate the C preprocessed source with one of the internal RAM
      # #ifndef identifiers not defined
      ($dash_D_syms = $dash_D_symbols) =~ s/\s-D$_//;
      ($cc = $target_command) =~ s/_\.c/\.c/g;
      $cc .= " -E $dash_D_syms > $dir\\$stem\_$_\_.c";
   ############################################      
   #youandi
      $cc =~ s/\s$stem\.c/ $temp_sourcedir/g;
   ############################################
   #print "CC: $cc\n";

      system $cc;

      trim_c_preprocessed_source("$dir\\$stem\_$_\_.c", $stem);
   }
}


# ------------------------------------------------------------------------
# MAIN
# ------------------------------------------------------------------------
GetOptions("file=s", "targetdir=s");
die "ERROR:  Makefile not specified\n" if (!defined($opt_file));
die "ERROR:  $opt_file does not exists\n" if (!-e $opt_file);
die "ERROR:  Target directory not specified\n" if (!defined($opt_targetdir));
die "ERROR:  $opt_targetdir directory does not exist\n" if(!-d $opt_targetdir);

process_arguments();

die "ERROR:  No C source specified\n" if ((keys %source_files) == 0);
die "ERROR:  No #ifndef identifier specified\n" if ((keys %identifiers) == 0);

mkdir $dir, 0x777 if (!-e $dir);

$targetdir = $opt_targetdir."\\";
$sourcedir = $sourcedir."\\";
###########################
#youandi
#print "\%source_files  = ", %source_files, "\n";
#print "\%identifiers   = ", %identifiers, "\n";
#print "\$targetdir     = ", $targetdir, "\n";
#print "\$dir           = ", $dir, "\n";
#print "\$opt_targetdir = ", $opt_targetdir, "\n";
#print "\$sourcedir     = ", $sourcedir, "\n";
###########################


foreach $file (keys %source_files) {
   next if (!keyword_in_file($file));
   print "Process $file\n";
   check_output_files_stat($file);
   get_dmss_compile_command($file);
   create_c_preprocessed_files($file);
   collect_rom_c_objects($file);
   foreach (keys %identifiers) {
      create_iram_sources($file) if defined($identifiers{$_});
   }
   resolve_external_references($file);
   if (%static_vars || %static_funcs) {
#      remove_statics($file);
   }
   undef %functions;
   undef %variables;
   undef %static_funcs;
   undef %static_vars;
   undef %undef_symbols;
}

# Deleting the files using unlink does not work
if ($ENV{OS} eq "Windows_NT") {
#   system "del $dir\\*.c";
}
else {
#   system "del $dir\\*.c";
#   system "del $dir\\tcc.out";
}
