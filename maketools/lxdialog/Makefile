check-lxdialog  := ./check-lxdialog.sh
HOST_EXTRACFLAGS = $(shell $(CONFIG_SHELL) $(check-lxdialog) -ccflags)
HOST_LOADLIBES   = $(shell $(CONFIG_SHELL) $(check-lxdialog) -ldflags $(HOSTCC))
HOST_EXTRACFLAGS += -DLOCALE
all: build
$(obj)/dochecklxdialog:
	$(Q)$(CONFIG_SHELL) $(check-lxdialog) -check $(CC) $(HOST_EXTRACFLAGS) $(HOST_LOADLIBES)

hostprogs-y	:= lxdialog
always		:= $(hostprogs-y) dochecklxdialog
lxdialog-objs := checklist.c menubox.c textbox.c yesno.c inputbox.c util.c lxdialog.c msgbox.c
build: $(obj)/dochecklxdialog
	$(CC) $(HOST_EXTRACFLAGS) $(HOST_LOADLIBES) $(lxdialog-objs) -o lxdialog
