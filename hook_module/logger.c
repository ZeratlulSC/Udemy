#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sizes.h>
#include <linux/string.h>
#include <linux/time.h>
#include <linux/sched/clock.h>
#include <linux/uaccess.h>

#include "logger.h"


#define PROC_FILE_NAME "hook_logger"
#define BUF_SIZE	SZ_256K
#define MAX_STR_LEN	128

MODULE_LICENSE("GPL");

static char log_buf[BUF_SIZE];
static struct proc_dir_entry *entry;
static int is_buf_full;
static unsigned int curpos;

void logger_print(const char *fmt, ...)
{
	int len;
	va_list args;
	char buf[MAX_STR_LEN + 16];
	u64 time;
	unsigned long nsec;

	time = local_clock();
	nsec = do_div(time, 1000000000);
	len = snprintf(buf, sizeof(buf), "[%5lu.%06ld] ", (unsigned long)time, nsec / 1000);

	va_start(args, fmt);
	len += vsnprintf(buf + len, MAX_STR_LEN, fmt, args);
	va_end(args);

	if (curpos + len >= BUF_SIZE) {
			curpos = 0;
			is_buf_full = 1;
	}
	memcpy(log_buf + curpos, buf, len);
	curpos += len;
}

static ssize_t myread(struct file *file, char __user *ubuf, size_t len, loff_t *offset)
{
	loff_t pos = *offset;
	ssize_t count;
	size_t size;

	if (is_buf_full)
		size = BUF_SIZE;
	else
		// FIXME:
		// should be atomic otherwise race here when
		// !is_buf_full but before we have access to curpos
		// we get is_buf_full and now curpos is 0, so size will 0
		size = (size_t)curpos;

	if (pos >= size)
		return 0;

	count = min(len, size);

	if ((pos + count) > size)
		count = size - pos;

	if (copy_to_user(ubuf, log_buf + pos, count))
		return -EFAULT;

	*offset += count;

	return count;
}

static struct file_operations logger_ops = {
	.owner = THIS_MODULE,
	.read = myread,
};

void logger_init(void)
{
	entry = proc_create(PROC_FILE_NAME, 0444, NULL, &logger_ops);
	if (!entry) {
		pr_err("%s: failed to create proc entry\n", __func__);
		return;
	}

	logger_print("logger inited\n");
}

void logger_exit(void)
{
	if (entry)
		proc_remove(entry);
}
