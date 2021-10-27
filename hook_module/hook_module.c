#include <linux/cred.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/module.h>
#include <linux/ptrace.h>
#include <linux/uidgid.h>

#include "log.h"
#include "logger.h"
#include "filter.h"
#include "inject.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bulat A.");
MODULE_DESCRIPTION("hooker");


static void hooker_copy_process(struct pt_regs *regs)
{
	long retval;
	struct task_struct *child;
	struct pid *child_pid_t;
	struct pid *parent_pid_t;
	struct filter_data filter_data;

	retval = regs_return_value(regs);
	if (!retval)
		return;

	child = (struct task_struct *)retval;
	if (IS_ERR(child)) {
		log_debug(hooker, "%s: child is %lx\n", __func__, retval);
		return;
	}

	child_pid_t = get_task_pid(child, PIDTYPE_PID);
	parent_pid_t = get_task_pid(current, PIDTYPE_PID);

	filter_data.child_pid = pid_nr(parent_pid_t);
	filter_data.parent_pid = pid_nr(child_pid_t);
	filter_data.gid = __kgid_val(current_gid());
	filter_data.uid = __kuid_val(current_uid());
	get_task_comm(filter_data.proc_name, current);

	put_pid(child_pid_t);
	put_pid(parent_pid_t);

	hooker_filter(&filter_data);
}

/* KRETPROBE */
/* per-instance private data */
struct my_data {
	ktime_t entry_stamp;
};

/* Here we use the entry_hanlder to timestamp function entry */
static int entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct my_data *data;

	if (!current->mm)
		return 1;	/* Skip kernel threads */

	data = (struct my_data *)ri->data;
	data->entry_stamp = ktime_get();
	return 0;
}

/*
 * Return-probe handler: Log the return value and duration. Duration may turn
 * out to be zero consistently, depending upon the granularity of time
 * accounting on the platform.
 */
static int ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	long retval = regs_return_value(regs);
	struct my_data *data;
	s64 delta;
	ktime_t now;

	log_debug(hooker, "ret_handler: ip = %lx,"
			" flags = 0x%lx\n", regs->ip, regs->flags);

	hooker_copy_process(regs);

	data = (struct my_data *)ri->data;

	now = ktime_get();
	delta = ktime_to_ns(ktime_sub(now, data->entry_stamp));
	log_debug(hooker, "ret_handler: returned %lx and took %lld ns to execute\n",
			retval, (long long)delta);
	return 0;
}

static struct kretprobe my_kretprobe = {
	.handler		= ret_handler,
	.entry_handler	= entry_handler,
	.data_size		= sizeof(struct my_data),
	/* Probe up to 20 instances concurrently. */
	.maxactive		= 20,
};

static int __init kprobe_init(void)
{
	int ret = 0;

	logger_init();

	ret = inject_init();
	if (ret)
		return ret;

	my_kretprobe.kp.symbol_name = INJECTED_FUNC;
	ret = register_kretprobe(&my_kretprobe);
	if (ret < 0) {
		inject_exit();
		log_debug(hooker, "register_kretprobe failed, returned %d\n",
				ret);
		return ret;
	}

	log_debug(hooker, "Planted return probe at %s: %p\n",
			my_kretprobe.kp.symbol_name, my_kretprobe.kp.addr);

	filter_init();

	return 0;
}

static void __exit kprobe_exit(void)
{
	filter_exit();

	inject_exit();

	unregister_kretprobe(&my_kretprobe);
	log_debug(hooker, "kretprobe at %p unregistered\n",
			my_kretprobe.kp.addr);

	logger_exit();
}

module_init(kprobe_init)
module_exit(kprobe_exit)
