#include <linux/module.h>
#include "log.h"

MODULE_LICENSE("GPL");

#define DEBUG_PARAM_DECLARE(param, value)					\
	unsigned int param##_verbosity = value;					\
	module_param(param##_verbosity, int, 0644);				\
	MODULE_PARM_DESC(param_##verbosity, "0: error, 1: info, 2: debug")

DEBUG_PARAM_DECLARE(hooker, 0);

#if defined(CONFIG_HOOK_FILTER)
DEBUG_PARAM_DECLARE(filter, 0);
#endif
