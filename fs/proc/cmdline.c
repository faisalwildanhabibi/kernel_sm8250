// SPDX-License-Identifier: GPL-2.0
/*
 * alioth: kernel-identity hardening for /proc/cmdline
 *
 * The bootloader state exposed here (verifiedbootstate=orange, flash.locked=0)
 * is one of the trivially readable signals that mark a device as rooted /
 * custom. We rewrite the reported tokens to their stock (locked, green)
 * values while leaving saved_command_line itself untouched, so in-kernel
 * consumers of the raw cmdline keep seeing the truth.
 */
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/kernel.h>

#ifdef CONFIG_ALIOTH_IDENTITY_SPOOF
static void cmdline_proc_show_sanitized(struct seq_file *m)
{
	const char *p = saved_command_line;
	const char *next;

	while (*p) {
		next = strchrnul(p, ' ');

		if (!strncmp(p, "androidboot.verifiedbootstate=", 30))
			seq_puts(m, "androidboot.verifiedbootstate=green");
		else if (!strncmp(p, "androidboot.flash.locked=", 25))
			seq_puts(m, "androidboot.flash.locked=1");
		else
			seq_write(m, p, next - p);

		if (!*next)
			break;

		seq_putc(m, ' ');
		p = next + 1;
	}

	seq_putc(m, '\n');
}
#endif

static int cmdline_proc_show(struct seq_file *m, void *v)
{
#ifdef CONFIG_ALIOTH_IDENTITY_SPOOF
	cmdline_proc_show_sanitized(m);
#else
	seq_puts(m, saved_command_line);
	seq_putc(m, '\n');
#endif
	return 0;
}

static int __init proc_cmdline_init(void)
{
	proc_create_single("cmdline", 0, NULL, cmdline_proc_show);
	return 0;
}
fs_initcall(proc_cmdline_init);
