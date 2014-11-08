
PLUGIN=help_version.so
SOURCES=\
        help_version.cc \
		$(END)

include ../Makefile.common

.PHONY: test
test: $(PLUGIN)
	$$($(CC) -print-prog-name=cc1) $(PLUGINFLAG) --help
