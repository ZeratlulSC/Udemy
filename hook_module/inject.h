#pragma once

/* For each probe you need to allocate a kprobe structure */
#define INJECTED_FUNC "copy_process"

#ifdef CONFIG_HOOK_INJECT
int inject_init(void);
void inject_exit(void);
#else /* CONFIG_HOOK_FILTER */
static int inject_init(void)
{
	return 0;
}

static void inject_exit(void)
{
	return;
}
#endif
