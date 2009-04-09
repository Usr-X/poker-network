/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
 * 
 * This file is part of libtinypoker.
 * 
 * libtinypoker is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * libtinypoker is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * libtinypoker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../main/tinypoker.h"
#include "test.h"

void logger(char *msg)
{
	printf("logger()\n");
        if (msg) {
                printf("%s\n", msg);
        }
}

int auth(char *s) {
	printf("auth()\n");
	return 1;
}

void client_connect_callback(ipp_socket * sock) {
	ipp_player *player;

	printf("client_connect_callback()\n");

	player = ipp_server_handshake(sock, "testd/1.0", auth, logger);
	if (player == NULL) {
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		assertNotNull("ipp_server_handshake() returned NULL", player);
		ipp_servloop_shutdown();
		return;
	}

	assertStringEqual("Player name fail", "JSMITH", player->name);
	ipp_free_player(player);
	ipp_servloop_shutdown();
	return;
}

gpointer serverside(gpointer data) {
	printf("serverside()\n");
	ipp_servloop(TINYPOKER_PORT, client_connect_callback);
	return NULL;
}

int clientside() {
	ipp_socket *sock;

	printf("clientside()\n");

	sleep(3); // give the server some time to startup

	printf("about to attempt handshake\n");

	sock = ipp_client_handshake("localhost", TINYPOKER_PORT, "JSMITH", "500", logger);
        if (sock) {
                printf("- HANDSHAKE OK\n");
                ipp_disconnect(sock);
                ipp_free_socket(sock);
                sock = NULL;
		return PASS;
        } else {
                printf("- HANDSHAKE FAILED\n");
		return FAIL;
        }
}

int main()
{
	GThread *server_thread;
	int client_rc, server_rc;

	ipp_init();

	server_thread = g_thread_create(serverside, NULL, TRUE, NULL);
	g_thread_yield();
	client_rc = clientside();

	g_thread_join(server_thread);

	ipp_exit();

	if (client_rc == FAIL || server_rc == FAIL) {
		return FAIL;
	} else {
		return PASS;
	}
}