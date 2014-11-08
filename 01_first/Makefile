
PLUGIN=my_first_gcc_plugin.so
SOURCES=\
        my_first_gcc_plugin.cc \
		$(END)

include ../Makefile.common

.PHONY: test
test: $(PLUGIN)
	$(CCPLUGIN) -c -x c /dev/null -o /dev/null
