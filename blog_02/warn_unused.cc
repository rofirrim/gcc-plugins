#include <iostream>

// This is the first gcc header to be included
#include "gcc-plugin.h"
#include "plugin-version.h"

#include "tree-pass.h"
#include "context.h"
#include "basic-block.h"
#include "gimple-pretty-print.h"

// We must assert that this plugin is GPL compatible
int plugin_is_GPL_compatible;

static struct plugin_info my_gcc_plugin_info =
{ "1.0", "This is a very simple plugin" };

namespace {
const pass_data my_first_pass_data =
{
  GIMPLE_PASS,
  "my_first_pass",		 /* name */
  OPTGROUP_NONE,			 /* optinfo_flags */
  TV_NONE,				 /* tv_id */
  PROP_gimple_any,		 /* properties_required */
  0,						 /* properties_provided */
  0,						 /* properties_destroyed */
  0,						 /* todo_flags_start */
  0						 /* todo_flags_finish */
};

struct my_first_pass : gimple_opt_pass
{
  my_first_pass(gcc::context * ctx) :
    gimple_opt_pass(my_first_pass_data, ctx)
  {
  }

  virtual unsigned int execute(function * fun) override
  {
    basic_block bb;

    std::cerr << "subgraph fun_" << fun << " {\n";

    FOR_ALL_BB_FN(bb, fun)
    {
      gimple_bb_info *bb_info = &bb->il.gimple;

      std::cerr << "bb_" << fun << "_" << bb->index << "[label=\"";
      if(bb->index == 0)
      {
        std::cerr << "ENTRY: "
                  << function_name(fun) << "\n"
                  << (LOCATION_FILE(fun->function_start_locus) ? : "<unknown>")
                  << ":" << LOCATION_LINE(fun->function_start_locus);
      }
      else if(bb->index == 1)
      {
        std::cerr << "EXIT: "
                  << function_name(fun) << "\n"
                  << (LOCATION_FILE(fun->function_end_locus) ? : "<unknown>") <<
                  ":" << LOCATION_LINE(fun->function_end_locus);
      }
      else
      {
        print_gimple_seq(stderr, bb_info->seq, 0, 0);
      }
      std::cerr << "\"];\n";

      edge e;
      edge_iterator ei;

      FOR_EACH_EDGE(e, ei, bb->succs)
      {
        basic_block dest = e->dest;
        std::
        cerr << "bb_" << fun << "_" << bb->index << " -> bb_" << fun <<
             "_" << dest->index << ";\n";
      }
    }

    std::cerr << "}\n";

    // Nothing special todo
    return 0;
  }

  virtual my_first_pass *clone() override
  {
    // We do not clone ourselves
    return this;
  }
};
}


namespace {

void start_graph()
{
  std::cerr << "digraph cfg {\n";
}

void end_graph()
{
  std::cerr << "}\n";
}

void finish_gcc(void *gcc_data, void *user_data)
{
  end_graph();
}

}


int plugin_init(struct plugin_name_args *plugin_info,
                struct plugin_gcc_version *version)
{
  if(!plugin_default_version_check(version, &gcc_version))
  {
    std::cerr << "This GCC plugin is for version " << GCCPLUGIN_VERSION_MAJOR
              << "." << GCCPLUGIN_VERSION_MINOR << "\n";
    return 1;
  }

  register_callback(plugin_info->base_name,
                    /* event */ PLUGIN_INFO,
                    /* callback */ NULL,
                    /* user_data */
                    &my_gcc_plugin_info);

  // Register the phase right after cfg
  struct register_pass_info pass_info;

  pass_info.pass = new my_first_pass(g);
  pass_info.reference_pass_name = "cfg";
  pass_info.ref_pass_instance_number = 1;
  pass_info.pos_op = PASS_POS_INSERT_AFTER;

  register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL,
                    &pass_info);
  register_callback(plugin_info->base_name, PLUGIN_FINISH, finish_gcc, NULL);

  start_graph();

  return 0;
}
