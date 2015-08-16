
PLUGIN=warn_unused_result_cxx.so
SOURCES=\
        warn_unused_result_cxx.cc \
		$(END)

include ../Makefile.common

.PHONY: test
test: $(PLUGIN) test.cc
	$(CXXPLUGIN) -std=c++11 -c -o /dev/null test.cc
