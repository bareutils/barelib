#START include config file
-include .config.mk
#END
lxdialog:
	$(MAKE) -C $(BARELIB_PREFIX)/maketools/lxdialog

DISABLED := @true
