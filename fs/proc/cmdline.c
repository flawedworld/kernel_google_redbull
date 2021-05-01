// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/setup.h>

static char proc_cmdline[COMMAND_LINE_SIZE];

static int cmdline_proc_show(struct seq_file *m, void *v)
{
	seq_puts(m, proc_cmdline);
	seq_putc(m, '\n');
	return 0;
}

static int __init proc_cmdline_init(void)
{
	char *offset_addr;
	char *a1, *a2; // anupritaisno1: edit original patch to fix warning

	offset_addr = strstr(saved_command_line, "androidboot.mode=reboot");
	if (offset_addr != NULL)
		strncpy(offset_addr + 17, "normal", 6);

	/* SafetyNet bypass: show androidboot.verifiedbootstate=green */

	a1 = strstr(saved_command_line, "androidboot.verifiedbootstate=");
	if (a1) {
		a1 = strchr(a1, '=');
		a2 = strchr(a1, ' ');
		if (!a2) /* last argument on the cmdline */
			a2 = "";

		scnprintf(proc_cmdline, COMMAND_LINE_SIZE, "%.*sgreen%s",
			  (int)(a1 - saved_command_line + 1),
			  saved_command_line, a2);
	} else {
		strncpy(proc_cmdline, saved_command_line, COMMAND_LINE_SIZE);
	}

	proc_create_single("cmdline", 0, NULL, cmdline_proc_show);
	return 0;
}
fs_initcall(proc_cmdline_init);
