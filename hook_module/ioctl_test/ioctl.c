// base on http://tldp.org/LDP/lkmpg/2.6/html/lkmpg.html#AEN687
/*
 *  ioctl.c - the process to use ioctl's to control the kernel module
 *
 *  Until now we could have used cat for input and output.  But now
 *  we need to do ioctl's, which require writing our own process.
 */

/* 
 * device specifics, such as ioctl numbers and the
 * major device file. 
 */
#include "filter.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <sys/ioctl.h>		/* ioctl */
#include <errno.h>
#include <string.h>

/* 
 * Functions for the ioctl calls 
 */

void ioctl_add_filter(int file_desc, struct filter_ioctl_data *filter_ioctl)
{
	int ret_val;

	ret_val = ioctl(file_desc, IOCTL_ADD_FILTER, filter_ioctl);

	if (ret_val < 0) {
		printf("ioctl_add_filter failed:%d\n", ret_val);
		return;
	}

	printf("set_filter \n");
}

void ioctl_read_filters(int file_desc, struct filter_ioctl_data *filter_ioctl)
{
	int ret_val;
	char buf[100] = {0};

	ret_val = ioctl(file_desc, IOCTL_READ_FILTERS, filter_ioctl);
	if (ret_val < 0) {
		printf("ioctl_read_filters failed:%d\n", ret_val);
		return;
	}

	snprintf(buf, sizeof(buf), "%s\n", filter_ioctl->data);

	printf("get_filter message: %s\n", buf);
}

void ioctl_cleanup_filter(int file_desc, int filter_type)
{
	int ret_val = 0;
	struct filter_ioctl_data filter_ioctl =
	{
		.filter_type = filter_type,
	};

	ret_val = ioctl(file_desc, IOCTL_CLEANUP_FILTERS, &filter_ioctl);
	if (ret_val < 0) {
		printf("ioctl_cleanup_filters failed:%d\n", ret_val);
		return;
	}
}

/* 
 * Main - Call the ioctl functions 
 */
void main()
{
	int file_desc, ret_val;
	struct filter_ioctl_data filter_ioctl[] =
	{
		{
			.data = "1000",
			.filter_type = UID,
			.len = sizeof("1000"),
		},
		{
			.filter_type = PROC_NAME,
			.data = "bash\0",
			.len = sizeof("bash\0")
		},
		{
			.data = (void *)0xffffffffff,
			.filter_type = FLT_CNT + 1,
		},
	};


	char buf[100];
	struct filter_ioctl_data filter_ioctl_get[] =
	{
		{
			.data = buf,
			.filter_type = UID,
			.len = sizeof("1000"),
		},
		{
			.filter_type = PROC_NAME,
			.data = buf,
			.len = sizeof("bash"),
		},
		{
			.filter_type = UID,
			.data = buf,
			.len = sizeof(buf),
		},
	};

	file_desc = open(DEVICE_FILE_NAME, 0);
	if (file_desc < 0) {
		printf("Can't open device file: %s ret %d\n", DEVICE_FILE_NAME, errno);
		exit(-1);
	}
	ioctl_cleanup_filter(file_desc, UID);
	ioctl_cleanup_filter(file_desc, PROC_NAME);

	ioctl_add_filter(file_desc, &filter_ioctl[0]);
	// return;
	ioctl_read_filters(file_desc, &filter_ioctl_get[0]);
	ioctl_read_filters(file_desc, &filter_ioctl_get[2]);
	// return;

	ioctl_add_filter(file_desc, &filter_ioctl[1]);
	ioctl_read_filters(file_desc, &filter_ioctl_get[1]);
	// return;

	ioctl_add_filter(file_desc, &filter_ioctl[2]);
	// return;

	ioctl_cleanup_filter(file_desc, UID);
	memset(filter_ioctl_get[0].data, 0, filter_ioctl_get[0].len);
	ioctl_read_filters(file_desc, &filter_ioctl_get[0]);
	// return;

	ioctl_cleanup_filter(file_desc, PROC_NAME);
	ioctl_read_filters(file_desc, &filter_ioctl_get[1]);

	close(file_desc);
}
