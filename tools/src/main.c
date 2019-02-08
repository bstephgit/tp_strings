#include <stdio.h>
#include <string.h>
#include "printmsg.h"
#include "lower.h"
#include "upper.h"
#include "rot13.h"


#define LOWER_OPT	"--lower"
#define UPPER_OPT	"--upper"
#define ROT13_OPT	"--rot13"
#define LOAD_OPT	"--load"

#include <dlfcn.h>
#define UPPER_LIB	"libupper.so"
#define LOWER_LIB	"liblower.so"
#define ROT13_LIB	"librot13.so"
#define DYN_FUNCTION_NAME	"module_execute"

#define 	COMMAND_INDEX		1
#define 	MODULE_INDEX		2


typedef 	void 	(*FUNC_PTR) (const char*, char*);

typedef void* HMODULE;

HMODULE load_module(const char* module_name)
{
	HMODULE handle = dlopen(module_name, RTLD_NOW);	
	if(!handle)
	{
		printmsg("cannot load module '%s'\n", module_name);
	}
	return handle;
}

HMODULE unload_module(HMODULE handle)
{
	if(handle)
	{
		dlclose(handle);
	}
}

FUNC_PTR get_dyn_function(HMODULE handle, const char* module_name, const char* function_name)
{
	FUNC_PTR fptr = NULL;
	if(handle)
	{
		fptr = dlsym(handle,function_name);
		if(!fptr)
		{
			printmsg("cannot load function '%s' from module '%s'\n",function_name,module_name);
		}
	}
	return fptr;
}

void process_args(FUNC_PTR fptr, int from, int to, char* args[])
{
	int i;
	for(i=from; i<to; i+=1)
	{
		(*fptr)(args[i],args[i]);
		printmsg("%s\n", args[i]);
	}
}

int main(int argc, char* argv[])
{

	if (argc > 2)
	{
		int arg_index = 2;
		const char* command = argv[COMMAND_INDEX];

		arg_index += 1;
		if (strcmp(command,LOWER_OPT)==0)
		{
			process_args(&make_lower,2,argc,argv);
		}
		else if (strcmp(command,UPPER_OPT)==0)
		{
			process_args(&make_upper,2,argc,argv);
		}
		else if (strcmp(command,ROT13_OPT)==0)
		{
			process_args(&rot13,2,argc,argv);
		}
		else if (strcmp(command,LOAD_OPT)==0)
		{
			
			const char* module_name = argv[MODULE_INDEX];
			HMODULE handle = load_module(module_name);
			FUNC_PTR fptr = get_dyn_function(handle,module_name,DYN_FUNCTION_NAME);
			if (fptr)
			{
				process_args(fptr,3,argc,argv);
			}
			
		}
		else
		{
			printmsg("Error: unknown paramter '%s'\n", command);
			return 1;
		}
	}
	else
	{
		printmsg("Error: unsufficient number of arguments\n");
		return 1;
	}
	return 0;
}
			
			
			
			

	
