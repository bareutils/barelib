$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(ACTIVE_CFLAGS) $(FPIC) -c $< -o $@

$(LIBDIR)/lib$(PROJECT).a: $(LIB_OBJS) | $(LIBDIR)
	$(AR) $(ARFLAGS) $@ $^

$(LIBDIR)/lib$(PROJECT).so: $(LIB_OBJS) | $(LIBDIR)
	$(CC) $(ACTIVE_CFLAGS) -shared -o $@ $^ $(ACTIVE_LDFLAGS)

ifneq ($(EXAMPLE_SRCS),)
$(BINDIR)/%: examples/%.c $(TARGETS) | $(BINDIR)
	$(CC) $(ACTIVE_CFLAGS) $< -o $@ -L$(LIBDIR) -l$(PROJECT) $(ACTIVE_LDFLAGS)
endif

$(OBJDIR) $(LIBDIR) $(BINDIR):
	mkdir -p $@

clean:
	rm -rf out
