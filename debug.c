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
}

void wcn36xx_debugfs_exit(struct wcn36xx *wcn)
{
	struct wcn36xx_dfs_entry *dfs = &wcn->dfs;
	debugfs_remove_recursive(dfs->rootdir);
}
