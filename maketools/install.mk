INSTDIR := $(DESTDIR)/$(PREFIX)
INCDIR  := $(INSTDIR)/include
INSTLIB := $(INSTDIR)/lib
INSTBIN := $(INSTDIR)/bin
install:
	@echo -e INSTALL $(ECHO)
	@install -d $(INCDIR)/$(PROJECT_NAMESPACE) $(INSTLIB)
	@install -m 644 include/$(PROJECT).h $(INCDIR)/$(PROJECT).h
	@if [ -d include/$(PROJECT_NAMESPACE) ]; then \
		install -m 644 include/$(PROJECT_NAMESPACE)/*.h $(INCDIR)/$(PROJECT_NAMESPACE)/; \
	fi
	@if [ -f $(LIBDIR)/lib$(PROJECT).a ]; then \
		install -m 644 $(LIBDIR)/lib$(PROJECT).a $(INSTLIB)/lib$(PROJECT).a; \
	fi
	@if [ -f $(LIBDIR)/lib$(PROJECT).so ]; then \
		install -m 755 $(LIBDIR)/lib$(PROJECT).so $(INSTLIB)/lib$(PROJECT).so; \
	fi
	@install -d $(INSTBIN)
	@for bin in $(BINARIES); do \
		install -m 755 $$bin $(INSTBIN)/; \
	done
	@cp -r $(OUTDIR)/etc $(DESTDIR)

uninstall:
	rm -f  $(INCDIR)/$(PROJECT).h
	rm -rf $(INCDIR)/$(PROJECT_NAMESPACE)
	rm -f  $(INSTLIB)/lib$(PROJECT).a
	rm -f  $(INSTLIB)/lib$(PROJECT).so
	@for bin in $(BINARIES); do \
		rm -f $(INSTBIN)/$$(basename $$bin); \
	done
