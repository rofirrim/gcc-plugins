// This is the first gcc header to be included
#include "gcc-plugin.h"
#include "plugin-version.h"

// Needed for gimple_opt_pass
#include "tree-pass.h"
#include "function.h"
#include "tree.h"
#include "gimple.h"

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
  PROP_gimple_any,        /* properties_required */
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
    pass_info.reference_pass_name = "omplower";
    pass_info.ref_pass_instance_number = 1;
    pass_info.pos_op = PASS_POS_INSERT_AFTER;

    register_callback (plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_info);

    return 0;
}

static tree callback_stmt(gimple_stmt_iterator * gsi, bool *handled_all_ops, struct walk_stmt_info *wi)
{
    gimple g = gsi_stmt(*gsi);

    location_t l = gimple_location(g);
    enum gimple_code code = gimple_code(g);

    fprintf(stderr, "Statement of type: %s at %s:%d\n",
            gimple_code_name[code],
            LOCATION_FILE(l),
            LOCATION_LINE(l));

    return NULL;
}

static tree callback_op(tree *t, int *, void *data)
{
    enum tree_code code = TREE_CODE(*t);

    fprintf(stderr, "   Operand: %s\n", tree_code_name[code]);
    return NULL;
}

static unsigned int execute_my_first_pass(void)
{
    // cfun is the current function being called
    gimple_seq gimple_body = cfun->gimple_body;

    struct walk_stmt_info walk_stmt_info;
    memset(&walk_stmt_info, 0, sizeof(walk_stmt_info));

    walk_gimple_seq(gimple_body, callback_stmt, callback_op, &walk_stmt_info);

    // Nothing special todo
    return 0;
}
