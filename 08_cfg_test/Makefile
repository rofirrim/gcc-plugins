
PLUGIN=cfg_test.so
SOURCES=\
        cfg_test.cc \
		$(END)

include ../Makefile.common

.PHONY: test
test: $(PLUGIN)
	$(CCPLUGIN) -c -o /dev/null test.c
