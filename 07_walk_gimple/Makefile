
PLUGIN=walk_gimple.so
SOURCES=\
        walk_gimple.cc \
		$(END)

include ../Makefile.common

.PHONY: test
test: $(PLUGIN)
	$(CCPLUGIN) -c test.c -o /dev/null
