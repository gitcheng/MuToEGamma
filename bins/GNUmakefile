# Please see SoftRelTools/HOWTO-GNUmakefile for documentation
# $Id: GNUmakefile 1878 2010-01-25 09:42:17Z stroili $
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

BINS := MuToEGammaApp
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

#################################################################
#++ include standard makefile from SoftRelTools.
include SoftRelTools/standard.mk

