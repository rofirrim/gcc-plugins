
PLUGIN=dump_body.so
SOURCES=\
        dump_body.cc \
		$(END)

include ../Makefile.common

.PHONY: test
test: $(PLUGIN)
	$(CCPLUGIN) -c test.c
