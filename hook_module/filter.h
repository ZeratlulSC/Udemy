#pragma once

#define DEVICE_FILE_NAME "hooker_filter"

#ifndef TASK_COMM_LEN
#define TASK_COMM_LEN 16
#endif

#define MAJOR_NUM 100

struct filter_ioctl_data {
	unsigned int filter_type;
	char *data;
	unsigned int len;
	// unsigned long value;
	// char data[TASK_COMM_LEN];
};

#include <linux/ioctl.h>
#define IOCTL_ADD_FILTER		_IOWR(MAJOR_NUM, 0, struct filter_ioctl_data *)
#define IOCTL_READ_FILTERS		_IOW(MAJOR_NUM, 1, struct filter_ioctl_data *)
#define IOCTL_CLEANUP_FILTERS	_IOR(MAJOR_NUM, 2, struct filter_ioctl_data *)
#define IOCTL_SET_FILTER		_IOWR(MAJOR_NUM, 3, struct filter_ioctl_data *)
#define IOCTL_GET_FILTER		_IOWR(MAJOR_NUM, 4, struct filter_ioctl_data *)
#define IOCTL_REMOVE_FILTER		_IOWR(MAJOR_NUM, 5, struct filter_ioctl_data *)

#ifdef __KERNEL__
#include <linux/compiler.h>

struct filter_data {
	char proc_name[TASK_COMM_LEN];
	pid_t parent_pid;
	pid_t child_pid;
	gid_t gid;
	uid_t uid;
};

#ifdef CONFIG_HOOK_FILTER
void hooker_filter(struct filter_data *data);
int filter_init(void);
void filter_exit(void);
#else /* CONFIG_HOOK_FILTER */
static void hooker_filter(struct filter_data *data)
{
	(void)data;
	return;
};
static int filter_init(void)
{
	return 0;
}

static void filter_exit(void)
{
	return;
}
#endif

#else /* __KERNEL__ */
enum {
	PID,
	GID,
	UID,
	PROC_NAME,
	FLT_CNT
} filter_types;
// part for drivers` users
#endif 