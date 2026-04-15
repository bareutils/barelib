$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	@echo -e CC $(ECHO) $<
	@$(CC) $(ACTIVE_CFLAGS) $(FPIC) -c $< -o $@

$(LIBDIR)/lib$(PROJECT).a: $(LIB_OBJS) | $(LIBDIR)
	@$(AR) $(ARFLAGS) $@ $^

$(LIBDIR)/lib$(PROJECT).so: $(LIB_OBJS) | $(LIBDIR)
	@echo -e CC $(ECHO) $@
	@$(CC) $(ACTIVE_CFLAGS) -shared -o $@ $^ $(ACTIVE_LDFLAGS)

ifneq ($(EXDIR),)
$(BINDIR)/%: $(EXDIR)/%.c | $(BINDIR)
	@echo -e CC $(ECHO) $<
	@$(CC) $(ACTIVE_CFLAGS) $< -o $@ -L$(LIBDIR) $(ACTIVE_LDFLAGS)
endif

$(OBJDIR) $(LIBDIR) $(BINDIR):
	@mkdir -p $@

clean:
	@echo -e CLEAN $(ECHO)
	@rm -rf $(OUTDIR)
