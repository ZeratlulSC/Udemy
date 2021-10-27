#include <linux/kernel.h>
#include <linux/kprobes.h>

#include "inject.h"
#include "log.h"
#include "logger.h"

static struct kprobe kp = {0};


/* kprobe pre_handler: called just before the probed instruction is executed */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
#ifdef CONFIG_X86
	log_debug(hooker, "pre_handler: p->addr = 0x%p, ip = %lx,"
			" flags = 0x%lx\n",
		p->addr, regs->ip, regs->flags);
#endif
#ifdef CONFIG_PPC
	log_debug(hooker, "pre_handler: p->addr = 0x%p, nip = 0x%lx,"
			" msr = 0x%lx\n",
		p->addr, regs->nip, regs->msr);
#endif

	/* A dump_stack() here will give a stack backtrace */
	return 0;
}

/* kprobe post_handler: called after the probed instruction is executed */
static void handler_post(struct kprobe *p, struct pt_regs *regs,
				unsigned long flags)
{
#ifdef CONFIG_X86
	log_debug(hooker, "post_handler: p->addr = 0x%p, ip = %lx,"
			" flags = 0x%lx\n",
		p->addr, regs->ip, regs->flags);
#endif
#ifdef CONFIG_PPC
	log_debug(hooker, "post_handler: p->addr = 0x%p, msr = 0x%lx\n",
		p->addr, regs->msr);
#endif
}

/*
 * fault_handler: this is called if an exception is generated for any
 * instruction within the pre- or post-handler, or when Kprobes
 * single-steps the probed instruction.
 */
static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
	log_debug(hooker, "fault_handler: p->addr = 0x%p, trap #%dn",
		p->addr, trapnr);
	/* Return 0 because we don't handle the fault. */
	return 0;
}

int inject_init()
{
	int ret = 0;

	kp.pre_handler = handler_pre;
	kp.post_handler = handler_post;
	kp.fault_handler = handler_fault;
	kp.addr = (kprobe_opcode_t *)kallsyms_lookup_name(INJECTED_FUNC);

	if (!kp.addr) {
		log_debug(hooker, "failed to find address for %s\n", INJECTED_FUNC);
		return -1;
	}

	ret = register_kprobe(&kp);
	if (ret < 0) {
		log_debug(hooker, "register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	log_debug(hooker, "Planted kprobe at %p\n", kp.addr);
}

void inject_exit()
{
	unregister_kprobe(&kp);
	log_debug(hooker, "kprobe at %p unregistered\n", kp.addr);
}