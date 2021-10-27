#pragma once

#include "logger.h"

extern unsigned int filter_verbosity;
extern unsigned int hooker_verbosity;

enum {
	HOOKER_LOG_LEVEL_ERROR,
	HOOKER_LOG_LEVEL_INFO,
	HOOKER_LOG_LEVEL_DEBUG,
};

static inline const char *hooker_print_prefix(unsigned int lvl)
{
	switch (lvl) {
	case HOOKER_LOG_LEVEL_ERROR:
		return "ERR";
	case HOOKER_LOG_LEVEL_INFO:
		return "INF";
	case HOOKER_LOG_LEVEL_DEBUG:
		return "DBG";
	default:
		return "???";
	}
}

#define hooker_print(lvl, param, fmt, ...)				\
	do {												\
		if (lvl <= param)								\
			printk("hooker %s %s(%d): "fmt,				\
					hooker_print_prefix(lvl),			\
					__func__, __LINE__, ##__VA_ARGS__);	\
		logger_print(fmt, ##__VA_ARGS__);				\
	} while (0)

#define log_debug(module, fmt, ...)	hooker_print(HOOKER_LOG_LEVEL_DEBUG, module##_verbosity, fmt, ##__VA_ARGS__)
#define log_info(module, fmt, ...)	hooker_print(HOOKER_LOG_LEVEL_INFO, module##_verbosity, fmt, ##__VA_ARGS__)
#define log_error(module, fmt, ...)	hooker_print(HOOKER_LOG_LEVEL_ERROR, module##_verbosity, fmt, ##__VA_ARGS__)
