#pragma once

#include <linux/compiler.h>

#ifdef CONFIG_HOOK_LOGGER
void logger_print(const char *fmt, ...);
void logger_init(void);
void logger_exit(void);
#else
void logger_init(void)
{
	return;
}
void logger_exit(void)
{
	return;
}
static void logger_print(const char *fmt, ...)
{
	(void)fmt;
	return;
};
#endif
