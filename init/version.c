/*
 *  linux/init/version.c
 *
 *  Copyright (C) 1992  Theodore Ts'o
 *
 *  May be freely distributed as part of Linux.
 */

#include <generated/compile.h>
#include <linux/build-salt.h>
#include <linux/export.h>
#include <linux/init.h>
#include <linux/uts.h>
#include <linux/utsname.h>
#include <linux/string.h>
#include <generated/utsrelease.h>
#include <linux/version.h>
#include <linux/proc_ns.h>

#ifndef CONFIG_KALLSYMS
#define version(a) Version_ ## a
#define version_string(a) version(a)

extern int version_string(LINUX_VERSION_CODE);
int version_string(LINUX_VERSION_CODE);
#endif

struct uts_namespace init_uts_ns = {
	.kref = KREF_INIT(2),
	.name = {
		.sysname	= UTS_SYSNAME,
		.nodename	= UTS_NODENAME,
		.release	= UTS_RELEASE,
		.version	= UTS_VERSION,
		.machine	= UTS_MACHINE,
		.domainname	= UTS_DOMAINNAME,
	},
	.user_ns = &init_user_ns,
	.ns.inum = PROC_UTS_INIT_INO,
#ifdef CONFIG_UTS_NS
	.ns.ops = &utsns_operations,
#endif
};
EXPORT_SYMBOL_GPL(init_uts_ns);

/* FIXED STRINGS! Don't touch! */
const char linux_banner[] =
	"Linux version " UTS_RELEASE " (" LINUX_COMPILE_BY "@"
	LINUX_COMPILE_HOST ") (" LINUX_COMPILER ") " UTS_VERSION "\n";

const char *linux_banner_ptr = linux_banner;
EXPORT_SYMBOL_GPL(linux_banner_ptr);

const char linux_proc_banner[] =
	"%s version %s"
	" (" LINUX_COMPILE_BY "@" LINUX_COMPILE_HOST ")"
	" (" LINUX_COMPILER ") %s\n";

BUILD_SALT;

#ifdef CONFIG_ALIOTH_IDENTITY_SPOOF
/*
 * alioth: re-assert the stock HyperOS kernel identity on the live UTS
 * namespace. UTS_RELEASE and UTS_VERSION already resolve to the stock
 * strings through the build metadata (Makefile SUBLEVEL, CONFIG_LOCALVERSION
 * and the pinned KBUILD_BUILD_TIMESTAMP in scripts/mkcompile_h); this makes
 * /proc/version, uname() and sysinfo() immune to any drift of that metadata.
 */
static int __init alioth_uts_identity_init(void)
{
	struct new_utsname *u = &init_uts_ns.name;

	strscpy(u->release, UTS_RELEASE, sizeof(u->release));
	strscpy(u->version, UTS_VERSION, sizeof(u->version));

	pr_info("alioth: kernel identity set to %s %s\n", u->release, u->version);

	return 0;
}
late_initcall(alioth_uts_identity_init);
#endif

