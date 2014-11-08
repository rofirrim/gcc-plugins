
PLUGIN=graphviz.so
SOURCES=\
        graphviz.cc \
		$(END)

include ../Makefile.common

.PHONY: test
test: $(PLUGIN)
	$(CCPLUGIN) -c -o /dev/null test.c
