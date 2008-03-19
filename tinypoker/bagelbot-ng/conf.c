/*
 * bagelbot-ng - Trivial client for tinypokerd
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "conf.h"

#include <libdaemon/dlog.h>
#include <confuse.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Reads 'configfile' and allocates memory for their values
 */
void configure()
{
	cfg_opt_t opts[] = {
		CFG_SIMPLE_STR("user", &user),
		CFG_SIMPLE_STR("host", &host),
		CFG_SIMPLE_INT("port", &port),
		CFG_END()
	};

	cfg_t *cfg;

	user = host = NULL;
	port = 0;

	cfg = cfg_init(opts, 0);	/* call libconfuse */
	cfg_parse(cfg, configfile);
	cfg_free(cfg);
}
