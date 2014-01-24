// This is the first gcc header to be included
#include "gcc-plugin.h"
#include "plugin-version.h"

#include <stdio.h>

// We must assert that this plugin is GPL compatible
int plugin_is_GPL_compatible;

	      
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

    // Let's print all the information given to this plugin!

    fprintf(stderr, "Plugin info\n");
    fprintf(stderr, "===========\n\n");
    fprintf(stderr, "Base name: %s\n", plugin_info->base_name);
    fprintf(stderr, "Full name: %s\n", plugin_info->full_name);
    fprintf(stderr, "Number of arguments of this plugin: %d\n", plugin_info->argc);
    int i;
    for (i = 0; i < plugin_info->argc; i++)
    {
        fprintf(stderr, "Argument %d: Key: %s. Value: %s\n", i, plugin_info->argv[i].key, plugin_info->argv[i].value);

    }
    fprintf(stderr, "Version string of the plugin: %s\n", plugin_info->version);
    fprintf(stderr, "Help string of the plugin: %s\n", plugin_info->help);

    fprintf(stderr, "\n");
    fprintf(stderr, "Version info\n");
    fprintf(stderr, "============\n\n");
    fprintf(stderr, "Base version: %s\n", version->basever);
    fprintf(stderr, "Date stamp: %s\n", version->datestamp);
    fprintf(stderr, "Dev phase: %s\n", version->devphase);
    fprintf(stderr, "Revision: %s\n", version->devphase);
    fprintf(stderr, "Configuration arguments: %s\n", version->configuration_arguments);
    fprintf(stderr, "\n");

    fprintf(stderr, "Plugin successfully initialized\n");

    return 0;
}
