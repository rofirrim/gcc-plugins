// This is the first gcc header to be included
#include "gcc-plugin.h"
#include "plugin-version.h"

#include <iostream>

// We must assert that this plugin is GPL compatible
int plugin_is_GPL_compatible;

static struct plugin_info my_gcc_plugin_info = { "1.0", "This is a very simple plugin" };

static void callback_finish_type(void *gcc_data, void *user_data)
{
    std::cerr << " *** A type has been finished\n";
}

static void callback_finish_declaration(void *gcc_data, void *user_data)
{
    std::cerr << " *** A declaration has been finished\n";
}

static void callback_finish_unit(void *gcc_data, void *user_data)
{
    std::cerr << " *** A translation unit has been finished\n";
}

static void callback_pre_genericize(void *gcc_data, void *user_data)
{
    std::cerr << " *** We are about to emit GENERIC after parsing C/C++\n";
}

static void callback_finish(void *gcc_data, void *user_data)
{
    std::cerr << " *** GCC is exiting\n";
}

static void callback_register_attribute(void *gcc_data, void *user_data)
{
    std::cerr << " *** Registering attributes\n";
}

static void callback_start_unit(void *gcc_data, void *user_data)
{
    std::cerr << " *** We are about to process a translation unit\n";
}

static void callback_registering_pragmas(void *gcc_data, void *user_data)
{
    std::cerr << " *** Registering pragmas\n";
}

static void callback_all_passes_start(void *gcc_data, void *user_data)
{
    std::cerr << " *** About to start all passes\n";
}

static void callback_all_passes_end(void *gcc_data, void *user_data)
{
    std::cerr << " *** All passes ended\n";
}

static void callback_pass_execution(void *gcc_data, void *user_data)
{
    std::cerr << " *** About to execute a pass\n";
}

int plugin_init (struct plugin_name_args *plugin_info,
		struct plugin_gcc_version *version)
{
	// We check the current gcc loading this plugin against the gcc we used to
	// created this plugin
	if (!plugin_default_version_check (version, &gcc_version))
    {
        std::cerr << "This GCC plugin is for version " << GCCPLUGIN_VERSION_MAJOR << "." << GCCPLUGIN_VERSION_MINOR << "\n";
		return 1;
    }

    register_callback(plugin_info->base_name,
            /* event */ PLUGIN_INFO,
            /* callback */ NULL, /* user_data */ &my_gcc_plugin_info);

    register_callback(plugin_info->base_name,
            PLUGIN_FINISH_TYPE,
            callback_finish_type, /* user_data */ NULL);

    register_callback(plugin_info->base_name,
            PLUGIN_FINISH_DECL,
            callback_finish_declaration, /* user_data */ NULL);

    register_callback(plugin_info->base_name,
            PLUGIN_FINISH_UNIT,
            callback_finish_unit, /* user_data */ NULL);

    register_callback(plugin_info->base_name,
            PLUGIN_PRE_GENERICIZE,
            callback_pre_genericize, /* user_data */ NULL);

    register_callback(plugin_info->base_name,
            PLUGIN_FINISH,
            callback_finish, /* user_data */ NULL);

    register_callback(plugin_info->base_name,
            PLUGIN_ATTRIBUTES,
            callback_register_attribute, /* user_data */ NULL);

    register_callback(plugin_info->base_name,
            PLUGIN_START_UNIT,
            callback_start_unit, /* user_data */ NULL);

    register_callback(plugin_info->base_name,
            PLUGIN_PRAGMAS,
            callback_registering_pragmas, /* user_data */ NULL);

    register_callback(plugin_info->base_name,
            PLUGIN_ALL_PASSES_START,
            callback_all_passes_start, /* user_data */ NULL);

    register_callback(plugin_info->base_name,
            PLUGIN_ALL_PASSES_END,
            callback_all_passes_end, /* user_data */ NULL);

    register_callback(plugin_info->base_name,
            PLUGIN_PASS_EXECUTION,
            callback_pass_execution, /* user_data */ NULL);

    return 0;
}
