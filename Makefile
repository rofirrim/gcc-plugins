.PHONY: all clean

all:
	for dir in $(wildcard [0123456789][0123456789]_*); \
    do \
		$(MAKE) -C $${dir}; \
    done

clean:
	for dir in $(wildcard [0123456789][0123456789]_*); \
    do \
		$(MAKE) -C $${dir} clean; \
    done
