// This is the first gcc header to be included
#include "gcc-plugin.h"
#include "plugin-version.h"

#include <stdio.h>

// We must assert that this plugin is GPL compatible
int plugin_is_GPL_compatible;

static struct plugin_info my_gcc_plugin_info = { "1.0", "This is a very simple plugin" };

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

    fprintf(stderr, "Number of arguments of this plugin: %d\n", plugin_info->argc);
    int i;
    for (i = 0; i < plugin_info->argc; i++)
    {
        fprintf(stderr, "Argument %d: Key: \"%s\" Value: \"%s\"\n", i, plugin_info->argv[i].key, plugin_info->argv[i].value);
    }

    return 0;
}
