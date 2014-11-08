
PLUGIN=my_first_pass.so
SOURCES=\
        my_first_pass.cc \
		$(END)

include ../Makefile.common

.PHONY: test
test: $(PLUGIN)
	$(CCPLUGIN) -c -o /dev/null test.c
