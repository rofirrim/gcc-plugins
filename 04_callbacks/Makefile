
PLUGIN=callbacks.so
SOURCES=\
        callbacks.cc \
		$(END)

include ../Makefile.common

.PHONY: test
test: $(PLUGIN)
	$(CCPLUGIN) -c -x c /dev/null -o /dev/null
