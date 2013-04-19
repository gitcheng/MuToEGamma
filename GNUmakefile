# Please see SoftRelTools/HOWTO-GNUmakefile for documentation
# $Id: GNUmakefile 458 2008-09-08 22:27:29Z simi $
#################################################################
#++ library products				[build it with 'lib']

LIBREMOVEFILES := 
LIBTMPLFILES := 
LIBDDLORDERED := 

#################################################################
#++ extra binary products	[not in production, build it with extrabin]

#EXTRABINS := xxxApp

#++ Extra binary rules
#$(addprefix $(bindir),$(EXTRABINS)): $(bindir)% : %.o

#################################################################
#++ binary products				[build it with 'bin']

BINS := 
BINCCFILES := $(BINS:=.cc) $(EXTRABINS:=.cc)

#++ Binary rules		 [in production, build it with 'bin']
$(addprefix $(bindir),$(BINS)): $(bindir)% : %.o

#++ shell script products.. 			[build it with 'bin']
#BINSCRIPTS := 

#################################################################
#++ regression test scripts			[build it with 'test']

#$(testdir)mytest.T : 

#########################################
#++ make sure -fno-automatic flag is used

###ifneq (,$(findstring Linux2,$(BFARCH)))
noautomatic_pkg += $(PACKAGE)
##endif ## Linux2

SUBDIRS = src bins
#################################################################
#++ include standard makefile from SoftRelTools.
include SoftRelTools/standard.mk

