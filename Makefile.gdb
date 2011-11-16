# These are all used for debugging testcases.  This is sufficiently hackish so
# it's going into it's own Makefile.

# We only need to regenerated the default .conf file if the Makefile has changed.
$(top_builddir)/debug-test.conf: Makefile
	@echo "Generating common debug configuration file $@."
	@echo 'CC="$(CC)"' > $@
	@echo 'DBG=$(dir $(firstword $(CC)))gdb$(cc_msize)' >> $@
	@echo 'OBJDUMP=$(dir $(firstword $(CC)))objdump' >> $@
	@echo 'GLIBC_BUILD=$(glibc_builddir)' >> $@
	@echo 'GLIBC_HEADERS=$(glibc_headers)' >> $@
	@echo 'LIBDFP_BUILD=$(top_builddir)/' >> $@
	@echo 'LIBDFP_HEADERS=$(top_srcdir)/dfp' >> $@
	@echo 'LIBDFP_SRC=$(top_srcdir)' >> $@
	cp $(top_srcdir)/tests/debug-test.sh $(top_builddir)/
	@echo

# We only need to regenerated the .conf files if the Makefile has changed.
$(addsuffix .conf,$(libdfp_tests)): Makefile
	@echo "Generating $@ configuration file."
	@echo 'GDB_SCRIPT="$(patsubst %.conf,%,$@).gdb"' > $@
	@echo 'APP="$(patsubst %.conf,%,$@)"' >> $@
	@echo 'APP_SRC="$(patsubst %.conf,%,$@).c"' >> $@
ifneq ($(glibc_builddir),)
	@echo 'APP_OR_LOADER="$(glibc_builddir)/elf/ld.so"' >> $@
else
	@echo 'APP_OR_LOADER="$(top_builddir)/$(patsubst %.conf,%,$@)"' >> $@
endif
	@echo

# These need to be regenerated whenever the test executables change since the
# address layouts may have changed.
$(addsuffix .gdb,$(libdfp_tests)): %.gdb:%
	@echo "Generating GDB script $@"
	@echo 'source $(top_srcdir)/tests/gdb_start_address.py' > $@
	@echo 'set environment C -E -x c-header' >> $@
	@echo 'set environment LD_LIBRARY_PATH=$(top_builddir)/:$$LD_LIBRARY_PATH' >> $@
ifneq ($(glibc_builddir),)
ifeq ($(machine),powerpc/powerpc64)
# powerpc64 doesn't have _dl_main_dispatch exported.
	@echo 'break _dl_start_user' >> $@
else
	@echo 'break _dl_main_dispatch' >> $@
endif
	@echo 'run --library-path $(LIBRARY_PATH):$(glibc_builddir)/nptl_db:$(top_builddir)/ $(top_builddir)/$(patsubst %.gdb,%,$@)' >> $@
else
	@echo 'break main' >> $@
	@echo 'run' >> $@
endif
	@echo '' >> $@
	@echo -n 'add-symbol-file $(top_builddir)/$(patsubst %.gdb,%,$@) 0x' >> $@
	@echo `$(dir $(firstword $(CC)))objdump -s --section=".text" $(top_builddir)/$(patsubst %.gdb,%,$@) | grep Contents -A 1 | tail -n 1 | awk -F' ' '{printf $$1}'` >> $@
	@echo '' >> $@
	@echo 'p/x $$start_address("libc")' >> $@
	@echo 'set $$libc_start = $$' >> $@
	@echo -n 'set $$libc_text = 0x' >> $@
ifneq ($(glibc_builddir),)
	@echo `$(dir $(firstword $(CC)))objdump -s --section=".text" $(glibc_builddir)/libc.so | grep Contents -A 1 | tail -n 1 | awk -F' ' '{printf $$1}'` >> $@
else
	@$(dir $(firstword $(CC)))objdump -s --section=".text" `LD_LIBRARY_PATH=./:$$LD_LIBRARY_PATH ldd $(patsubst %.gdb,%,$@) | grep libc.so | awk -F' ' '{print $$3}'` | grep Contents -A 1 | tail -n 1 | awk -F' ' '{printf $$1}' >> $@
	@echo >> $@
endif
	@echo 'set $$libc_addr = $$libc_start + $$libc_text' >> $@
	@echo -n 'add-symbol-file ' >> $@
ifneq ($(glibc_builddir),)
	@echo -n '$(glibc_builddir)/libc.so' >> $@
else
	@echo -n `LD_LIBRARY_PATH=./:$$LD_LIBRARY_PATH ldd $(patsubst %.gdb,%,$@) | grep libc.so | awk -F' ' '{print $$3}'` >> $@
endif
	@echo ' $$libc_addr' >> $@
	@echo '' >> $@
	@echo 'p/x $$start_address("libdfp")' >> $@
	@echo 'set $$libdfp_start = $$' >> $@
	@echo -n 'set $$libdfp_text = 0x' >> $@
	@echo `$(dir $(firstword $(CC)))objdump -s --section=".text" $(top_builddir)/libdfp.so.1 | grep Contents -A 1 | tail -n 1 | awk -F' ' '{printf $$1}'` >> $@
	@echo 'set $$libdfp_addr = $$libdfp_start + $$libdfp_text' >> $@
	@echo 'add-symbol-file $(top_builddir)/libdfp.so.1 $$libdfp_addr' >> $@
	@echo
