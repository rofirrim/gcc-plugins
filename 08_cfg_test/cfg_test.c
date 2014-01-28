// This is the first gcc header to be included
#include "gcc-plugin.h"
#include "plugin-version.h"

// Needed for gimple_opt_pass
#include "tree-pass.h"
#include "function.h"
#include "tree.h"
#include "gimple.h"
#include "gimple-pretty-print.h"

#include <stdio.h>

// We must assert that this plugin is GPL compatible
int plugin_is_GPL_compatible;

static struct plugin_info my_gcc_plugin_info = { "1.0", "This is a very simple plugin" };


static unsigned int execute_my_first_pass(void);

static struct gimple_opt_pass my_first_pass =
{
 {
  GIMPLE_PASS,
  "my_first_pass",        /* name */
  OPTGROUP_NONE,          /* optinfo_flags */
  NULL,                   /* gate */
  execute_my_first_pass,  /* execute */
  NULL,                   /* sub */
  NULL,                   /* next */
  0,                      /* static_pass_number */
  TV_NONE,                /* tv_id */
  PROP_cfg,               /* properties_required */
  0,                      /* properties_provided */
  0,                      /* properties_destroyed */
  0,                      /* todo_flags_start */
  0                       /* todo_flags_finish */
 }
};

int plugin_init (struct plugin_name_args *plugin_info,
		struct plugin_gcc_version *version)
{
	// We check the current gcc loading this plugin against the gcc we used to
	// created this plugin
	if (!plugin_default_version_check (version, &gcc_version))
    {
        fprintf(stderr, "This GCC plugin is for version %d.%d\n", GCCPLUGIN_VERSION_MAJOR, GCCPLUGIN_VERSION_MINOR);
		return 1;
    }

    register_callback(plugin_info->base_name,
            /* event */ PLUGIN_INFO,
            /* callback */ NULL, /* user_data */ &my_gcc_plugin_info);

    // Register the phase right after omplower
    struct register_pass_info pass_info;

    pass_info.pass = &my_first_pass.pass;
    pass_info.reference_pass_name = "cfg";
    pass_info.ref_pass_instance_number = 0;
    pass_info.pos_op = PASS_POS_INSERT_AFTER;

    register_callback (plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_info);

    return 0;
}


static unsigned int execute_my_first_pass(void)
{
    // cfun is the current function being called

    basic_block bb;
    FOR_EACH_BB_FN(bb, cfun)
    {
        fprintf(stderr, "Basic Block %d\n", bb->index);
        gimple_bb_info *bb_info = &bb->il.gimple;
        print_gimple_seq(stderr, bb_info->seq, 0, 0);
        fprintf(stderr, "\n");
    }

    // Nothing special todo
    return 0;
}
