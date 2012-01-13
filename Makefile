#############################################################################
# Makefile for building: Proximus-complete
# Generated by qmake (2.01a) (Qt 4.7.4) on: Tue Jan 3 11:40:03 2012
# Project:  Proximus-complete.pro
# Template: subdirs
# Command: i:/qtsdk/madde/targets/harmattan_10.2011.34-1/bin/qmake.exe -spec ../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/default -unix -after  OBJECTS_DIR=obj MOC_DIR=moc UI_DIR=ui RCC_DIR=rcc -o Makefile Proximus-complete.pro
#############################################################################

first: make_default
MAKEFILE      = Makefile
QMAKE         = i:/qtsdk/madde/targets/harmattan_10.2011.34-1/bin/qmake.exe
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
SUBTARGETS    =  \
		sub-Proximus \
		sub-proximus-daemon

Proximus/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) Proximus/ || $(MKDIR) Proximus/ 
	cd Proximus/ && $(QMAKE) i:/Qt/Proximus-complete/Proximus/Proximus.pro -spec ../../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/default -unix -after  OBJECTS_DIR=obj MOC_DIR=moc UI_DIR=ui RCC_DIR=rcc -o $(MAKEFILE)
sub-Proximus-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) Proximus/ || $(MKDIR) Proximus/ 
	cd Proximus/ && $(QMAKE) i:/Qt/Proximus-complete/Proximus/Proximus.pro -spec ../../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/default -unix -after  OBJECTS_DIR=obj MOC_DIR=moc UI_DIR=ui RCC_DIR=rcc -o $(MAKEFILE)
sub-Proximus: Proximus/$(MAKEFILE) FORCE
	cd Proximus/ && $(MAKE) -f $(MAKEFILE)
sub-Proximus-make_default: Proximus/$(MAKEFILE) FORCE
	cd Proximus/ && $(MAKE) -f $(MAKEFILE) 
sub-Proximus-make_first: Proximus/$(MAKEFILE) FORCE
	cd Proximus/ && $(MAKE) -f $(MAKEFILE) first
sub-Proximus-all: Proximus/$(MAKEFILE) FORCE
	cd Proximus/ && $(MAKE) -f $(MAKEFILE) all
sub-Proximus-clean: Proximus/$(MAKEFILE) FORCE
	cd Proximus/ && $(MAKE) -f $(MAKEFILE) clean
sub-Proximus-distclean: Proximus/$(MAKEFILE) FORCE
	cd Proximus/ && $(MAKE) -f $(MAKEFILE) distclean
sub-Proximus-install_subtargets: Proximus/$(MAKEFILE) FORCE
	cd Proximus/ && $(MAKE) -f $(MAKEFILE) install
sub-Proximus-uninstall_subtargets: Proximus/$(MAKEFILE) FORCE
	cd Proximus/ && $(MAKE) -f $(MAKEFILE) uninstall
proximus-daemon/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) proximus-daemon/ || $(MKDIR) proximus-daemon/ 
	cd proximus-daemon/ && $(QMAKE) i:/Qt/Proximus-complete/proximus-daemon/proximus-daemon.pro -spec ../../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/default -unix -after  OBJECTS_DIR=obj MOC_DIR=moc UI_DIR=ui RCC_DIR=rcc -o $(MAKEFILE)
sub-proximus-daemon-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) proximus-daemon/ || $(MKDIR) proximus-daemon/ 
	cd proximus-daemon/ && $(QMAKE) i:/Qt/Proximus-complete/proximus-daemon/proximus-daemon.pro -spec ../../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/default -unix -after  OBJECTS_DIR=obj MOC_DIR=moc UI_DIR=ui RCC_DIR=rcc -o $(MAKEFILE)
sub-proximus-daemon: proximus-daemon/$(MAKEFILE) FORCE
	cd proximus-daemon/ && $(MAKE) -f $(MAKEFILE)
sub-proximus-daemon-make_default: proximus-daemon/$(MAKEFILE) FORCE
	cd proximus-daemon/ && $(MAKE) -f $(MAKEFILE) 
sub-proximus-daemon-make_first: proximus-daemon/$(MAKEFILE) FORCE
	cd proximus-daemon/ && $(MAKE) -f $(MAKEFILE) first
sub-proximus-daemon-all: proximus-daemon/$(MAKEFILE) FORCE
	cd proximus-daemon/ && $(MAKE) -f $(MAKEFILE) all
sub-proximus-daemon-clean: proximus-daemon/$(MAKEFILE) FORCE
	cd proximus-daemon/ && $(MAKE) -f $(MAKEFILE) clean
sub-proximus-daemon-distclean: proximus-daemon/$(MAKEFILE) FORCE
	cd proximus-daemon/ && $(MAKE) -f $(MAKEFILE) distclean
sub-proximus-daemon-install_subtargets: proximus-daemon/$(MAKEFILE) FORCE
	cd proximus-daemon/ && $(MAKE) -f $(MAKEFILE) install
sub-proximus-daemon-uninstall_subtargets: proximus-daemon/$(MAKEFILE) FORCE
	cd proximus-daemon/ && $(MAKE) -f $(MAKEFILE) uninstall

Makefile: Proximus-complete.pro  ../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/default/qmake.conf ../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/common/g++.conf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/common/unix.conf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/common/linux.conf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/qconfig.pri \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/qt_functions.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/qt_config.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/default_pre.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/release.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/default_post.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/warn_on.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/qt.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/unix/thread.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/moc.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/resources.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/uic.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/yacc.prf \
		../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/lex.prf
	$(QMAKE) -spec ../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/default -unix -after  OBJECTS_DIR=obj MOC_DIR=moc UI_DIR=ui RCC_DIR=rcc -o Makefile Proximus-complete.pro
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/common/g++.conf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/common/unix.conf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/common/linux.conf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/qconfig.pri:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/qt_functions.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/qt_config.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/default_pre.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/release.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/default_post.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/warn_on.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/qt.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/unix/thread.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/moc.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/resources.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/uic.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/yacc.prf:
../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/features/lex.prf:
qmake: qmake_all FORCE
	@$(QMAKE) -spec ../../QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/share/qt4/mkspecs/default -unix -after  OBJECTS_DIR=obj MOC_DIR=moc UI_DIR=ui RCC_DIR=rcc -o Makefile Proximus-complete.pro

qmake_all: sub-Proximus-qmake_all sub-proximus-daemon-qmake_all FORCE

make_default: sub-Proximus-make_default sub-proximus-daemon-make_default FORCE
make_first: sub-Proximus-make_first sub-proximus-daemon-make_first FORCE
all: sub-Proximus-all sub-proximus-daemon-all FORCE
clean: sub-Proximus-clean sub-proximus-daemon-clean FORCE
distclean: sub-Proximus-distclean sub-proximus-daemon-distclean FORCE
	-$(DEL_FILE) Makefile
install_subtargets: sub-Proximus-install_subtargets sub-proximus-daemon-install_subtargets FORCE
uninstall_subtargets: sub-Proximus-uninstall_subtargets sub-proximus-daemon-uninstall_subtargets FORCE

sub-Proximus-check: Proximus/$(MAKEFILE)
	cd Proximus/ && $(MAKE) check
sub-proximus-daemon-check: proximus-daemon/$(MAKEFILE)
	cd proximus-daemon/ && $(MAKE) check
check: sub-Proximus-check sub-proximus-daemon-check

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all
install_target2: FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/etc/init/apps/ || $(MKDIR) $(INSTALL_ROOT)/etc/init/apps/ 
	-$(INSTALL_FILE) i:/Qt/Proximus-complete/proximus-daemon.conf $(INSTALL_ROOT)/etc/init/apps/


uninstall_target2:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/etc/init/apps/proximus-daemon.conf
	-$(DEL_DIR) $(INSTALL_ROOT)/etc/init/apps/ 


install: install_subtargets install_target2  FORCE

uninstall: uninstall_target2  uninstall_subtargets FORCE

FORCE:

