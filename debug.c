/*
 * Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * Licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include "wcn36xx.h"
#include "debug.h"
#include "pmc.h"

static int wcn36xx_debugfs_open(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return 0;
}
static ssize_t read_file_bool_bmps(struct file *file, char __user *user_buf,
				   size_t count, loff_t *ppos)
{
	struct wcn36xx *wcn = file->private_data;
	char buf[3];

	if (wcn->pw_state == WCN36XX_BMPS)
		buf[0] = '1';
	else
		buf[0] = '0';
	buf[1] = '\n';
	buf[2] = 0x00;
	return simple_read_from_buffer(user_buf, count, ppos, buf, 2);
}

static ssize_t write_file_bool_bmps(struct file *file,
				    const char __user *user_buf,
				    size_t count, loff_t *ppos)
{
	struct wcn36xx *wcn = file->private_data;
	struct ieee80211_vif *vif = container_of((void *)wcn->current_vif,
						 struct ieee80211_vif,
						 drv_priv);
	char buf[32];
	int buf_size;

	buf_size = min(count, (sizeof(buf)-1));
	if (copy_from_user(buf, user_buf, buf_size))
		return -EFAULT;

	switch (buf[0]) {
	case 'y':
	case 'Y':
	case '1':
		wcn36xx_pmc_enter_bmps_state(wcn, vif->bss_conf.sync_tsf);
		break;
	case 'n':
	case 'N':
	case '0':
		wcn36xx_pmc_exit_bmps_state(wcn);
		break;
	}

	return count;
}

static const struct file_operations fops_wcn36xx_bmps = {
	.open  =       wcn36xx_debugfs_open,
	.read  =       read_file_bool_bmps,
	.write =       write_file_bool_bmps,
};

#define ADD_FILE_BOOL(name, mode, fop, priv_data)		\
	do {							\
		struct dentry *d;				\
		d = debugfs_create_file(__stringify(name),	\
					mode, dfs->rootdir,	\
					priv_data, fop);	\
		dfs->file_##name.dentry = d;			\
		if (IS_ERR(d)) {				\
			wcn36xx_warn("Create the debugfs entry failed");\
			dfs->file_##name.dentry = NULL;		\
		}						\
	} while (0)


void wcn36xx_debugfs_init(struct wcn36xx *wcn)
{
	struct wcn36xx_dfs_entry *dfs = &wcn->dfs;

	dfs->rootdir = debugfs_create_dir(KBUILD_MODNAME,
					  wcn->hw->wiphy->debugfsdir);
	if (IS_ERR(dfs->rootdir)) {
		wcn36xx_warn("Create the debugfs failed");
		dfs->rootdir = NULL;
	}
	ADD_FILE_BOOL(bmps_switcher, S_IRUSR | S_IWUSR,
		      &fops_wcn36xx_bmps, wcn);
}

void wcn36xx_debugfs_exit(struct wcn36xx *wcn)
{
	struct wcn36xx_dfs_entry *dfs = &wcn->dfs;
	debugfs_remove_recursive(dfs->rootdir);
}
