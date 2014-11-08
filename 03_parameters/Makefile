
PLUGIN=my_parameters.so
SOURCES=\
        my_parameters.cc \
		$(END)

include ../Makefile.common

.PHONY:
test: $(PLUGIN)
	$(CCPLUGIN) -c -x c /dev/null -o /dev/null \
		-fplugin-arg-my_parameters-this=is-a-parameter \
		-fplugin-arg-my_parameters-and-this=is-another-parameter
