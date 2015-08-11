Install gcc
===========

Make sure you fulfill all the gcc dependences and set an installation path

    $ sudo apt-get install libgmp-dev libmpc-dev libmpfr-dev
    $ export INSTALLDIR=$HOME/gcc/gcc-install

Download gcc

    $ wget http://ftp.gnu.org/gnu/gcc/gcc-5.2.0/gcc-5.2.0.tar.bz2

Extract

    $ tar xfj gcc-5.2.0.tar.bz2

Create a build directory

    $ mkdir gcc-build
    $ cd gcc-build

Configure the source to get a C/C++ compiler

    $ ../gcc-5.2.0/configure --prefix=$INSTALLDIR --enable-languages=c,c++

Build (will take a while, like 10 min or so in a fast computer)

    $ make -j$(getconf _NPROCESSORS_ONLN) 

Install

    $ make install

Make sure we have plugins available

    $ ${INSTALLDIR}/bin/gcc -print-file-name=plugin
    <<INSTALLDIR>>/lib/gcc/x86_64-unknown-linux-gnu/5.2.0/plugin

If it just appears 'plugin' you are using the wrong compiler

Our first plugin
================

Create a boilerplate Makefile.common

    # Common makefile to be included from all other makefiles
    
    # Where we installed gcc and its headers
    INSTALLDIR=<<INSTALLDIR>>
    
    CC=$(INSTALLDIR)/bin/gcc
    CXX=$(INSTALLDIR)/bin/g++
    PLUGINDIR=$(shell $(CC) -print-file-name=plugin)
    
    CFLAGS=-fPIC -Wall -g -fno-rtti -I$(PLUGINDIR)/include
    CXXFLAGS=-fPIC -Wall -g -fno-rtti -I$(PLUGINDIR)/include
    LDFLAGS=
    LDADD=
    
    END=
    OBJECTS=$(patsubst %.cc,%.o,$(SOURCES))
    
    all: $(PLUGIN)
    
    $(PLUGIN): $(OBJECTS)
    	$(CXX) $(LDFLAGS) -o $@ -shared $+ $(LDADD)
    
    %.o: %.cc
    	$(CXX) -c -o $@ $(CFLAGS) $<
    
    .PHONY: all clean test
    clean:
    	rm -f $(OBJECTS) $(PLUGIN)
    
    test: $(PLUGIN)
    	$(CC) -fplugin=./$(PLUGIN) -c test.c
    
Create a first/Makefile

    PLUGIN=my-first-gcc-plugin.so
    SOURCES=\
            my-first-gcc-plugin.c \
    		$(END)
    
    include ../Makefile.common
  

Create a phase that just prints what it is passed by gcc

    // This is the first gcc header to be included
    #include "gcc-plugin.h"
    #include "plugin-version.h"
    
    #include <iostream>
    
    // We must assert that this plugin is GPL compatible
    int plugin_is_GPL_compatible;
    
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
    
        // Let's print all the information given to this plugin!
    
        std::cerr << "Plugin info\n";
        std::cerr << "===========\n\n";
        std::cerr << "Base name: " << plugin_info->base_name << "\n";
        std::cerr << "Full name: " << plugin_info->full_name << "\n";
        std::cerr << "Number of arguments of this plugin:" << plugin_info->argc << "\n";
    
        for (int i = 0; i < plugin_info->argc; i++)
        {
            std::cerr << "Argument " << i << ": Key: " << plugin_info->argv[i].key << ". Value: " << plugin_info->argv[i].value<< "\n";
    
        }
    
        std::cerr << "\n";
        std::cerr << "Version info\n";
        std::cerr << "============\n\n";
        std::cerr << "Base version: " << version->basever << "\n";
        std::cerr << "Date stamp: " << version->datestamp << "\n";
        std::cerr << "Dev phase: " << version->devphase << "\n";
        std::cerr << "Revision: " << version->devphase << "\n";
        std::cerr << "Configuration arguments: " << version->configuration_arguments << "\n";
        std::cerr << "\n";
    
        std::cerr << "Plugin successfully initialized\n";
    
        return 0;
    }

Compile

    $ make
    <<INSTALLDIR>>/bin/gcc -fPIC -Wall -I<<INSTALLDIR>>/lib/gcc/x86_64-unknown-linux-gnu/5.2.0/plugin/include   -c -o my-first-gcc-plugin.o my-first-gcc-plugin.c
    <<INSTALLDIR>>/bin/gcc  -o my-first-gcc-plugin.so -shared my-first-gcc-plugin.o 

Run gcc enabling the plugin (you can call 'make test')

    $ <<INSTALLDIR>>/bin/gcc -fplugin=./my-first-gcc-plugin.so -c test.c
    Plugin info
    ===========
    
    Base name: my_first_gcc_plugin
    Full name: ./my_first_gcc_plugin.so
    Number of arguments of this plugin:0
    
    Version info
    ============
    
    Base version: 5.2.0
    Date stamp: 20150716
    Dev phase: 
    Revision: 
    Configuration arguments: ../gcc-5.2.0/configure --prefix=<<INSTALLDIR>> --enable-languages=c,c++,fortran
    
    Plugin successfully initialized

The phase does nothing and still lacks info but it is a step in the good direction.

Introducing ourselves to the compiler
=====================================
  
    // This is the first gcc header to be included
    #include "gcc-plugin.h"
    #include "plugin-version.h"
    
    #include <iostream>
    
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
            std::cerr << "This GCC plugin is for version " << GCCPLUGIN_VERSION_MAJOR << "." << GCCPLUGIN_VERSION_MINOR << "\n";
    		return 1;
        }
    
        // Register the plugin itself
        register_callback(plugin_info->base_name,
                /* event */ PLUGIN_INFO,
                /* callback */ NULL, /* user_data */ &my_gcc_plugin_info);
    
        return 0;
    }

Call the C compiler (the gcc driver is not going to be enough) to see the plugins loaded

    $ <<INSTALLDIR>>/libexec/gcc/x86_64-unknown-linux-gnu/5.2.0/cc1 -fplugin=./help-version.so --help
    
    ...
      -p                                [disabled]
      -pedantic-errors                  [disabled]
      -quiet                            [disabled]
    
    Help for the loaded plugins:
     help-version:
        This is a very simple plugin

Being able to pass parameters
=============================

Once we have introduced ourselves to the compiler we can pass parameters using -fplugin-arg-<<basename>>-key=value

    // This is the first gcc header to be included
    #include "gcc-plugin.h"
    #include "plugin-version.h"
    
    #include <iostream>
    
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
            std::cerr << "This GCC plugin is for version " << GCCPLUGIN_VERSION_MAJOR << "." << GCCPLUGIN_VERSION_MINOR << "\n";
    		return 1;
        }
    
        register_callback(plugin_info->base_name,
                /* event */ PLUGIN_INFO,
                /* callback */ NULL, /* user_data */ &my_gcc_plugin_info);
    
        std::cerr << "Number of arguments of this plugin:" << plugin_info->argc << "\n";
    
        for (int i = 0; i < plugin_info->argc; i++)
        {
            std::cerr << "Argument " << i << ": Key: " << plugin_info->argv[i].key << ". Value: " << plugin_info->argv[i].value<< "\n";
    
        }
    
        return 0;
    }

For instance

    <<INSTALLDIR>>/bin/gcc -fplugin=./my_parameters.so -c -x c /dev/null \
    	-fplugin-arg-my_parameters-this=is-a-parameter \
    	-fplugin-arg-my_parameters-and-this=is-another-parameter
    Number of arguments of this plugin:2
    Argument 0: Key: this. Value: is-a-parameter
    Argument 1: Key: and-this. Value: is-another-parameter

