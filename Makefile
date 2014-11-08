.PHONY: all clean

END=
PLUGIN_DIRS = \
  01_first \
  02_help_version \
  03_parameters \
  04_callbacks \
  05_my_first_pass \
  06_dump_gimple \
  07_walk_gimple \
  08_cfg_test \
  09_cfg_graphviz \
  $(END)

all:
	for dir in $(PLUGIN_DIRS); \
    do \
		$(MAKE) -C $${dir}; \
    done

clean:
	for dir in $(PLUGIN_DIRS); \
    do \
		$(MAKE) -C $${dir} clean; \
    done

test:
	for dir in $(PLUGIN_DIRS); \
    do \
		$(MAKE) -C $${dir} test; \
    done
