#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "message.h"

#ifndef MESSAGE_MAX_LEN
#	define MESSAGE_MAX_LEN 4096
#endif

struct conn_t {
	int sock;
	char msg[MESSAGE_MAX_LEN];
};

struct conn_t *new_connection(const char *host, int port) {
	struct sockaddr_in srv_addr;
	memset(&srv_addr, 0, sizeof(srv_addr));

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, host, &srv_addr.sin_addr) <= 0) {
		fprintf(stderr, "can't parse host address %s\n", host);
		return NULL;
	}

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		fprintf(stderr, "can't create a socket: %s\n", strerror(errno));
		return NULL;
	}

	if (connect(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
		fprintf(stderr, "can't connect to %s:%d: %s\n", host, port, strerror(errno));
		return NULL;
	}

	struct conn_t *cn = calloc(sizeof(*cn), 1);
	cn->sock = sock;

	return cn;
}

void close_connection(struct conn_t *cn) {
	close(cn->sock);
	free(cn);
}

static int read_line(int fd, char *buf, int max_size) {
	int num_read = 0; // # of bytes fetched by last read()
	int tot_read = 0; // Total bytes read so far
	char ch = '\0';

	assert(max_size > 0 && buf != NULL);

	for (;;) {
		num_read = read(fd, &ch, 1);

		if (num_read == -1) {
			if (errno == EINTR) {
				// Interrupted --> restart read()
				continue;
			} else {
				return -1;
			}
		} else if (num_read == 0) {
			// EOF
			if (tot_read == 0) {
				// No bytes read; return 0
				return 0;
			} else {
				// Some bytes read; add '\0'
				break;
			}
		} else {
			// 'num_read' must be 1 if we get here
			assert(num_read == 1);
			if (tot_read < max_size - 1) {
				// Discard > (n - 1) bytes
				tot_read++;
				*buf++ = ch;
			}

			if (ch == '\n') {
				break;
			}
		}
	}

	*buf = '\0';
	return tot_read;
}

char *read_message(struct conn_t *cn) {
	// send byte to the server and wait for response
	// response ends with \n
	if (write(cn->sock, "", 1) != 1) {
		fprintf(stderr, "can't request message from the server: %s", strerror(errno));
		return NULL;
	}

	int line_len = read_line(cn->sock, cn->msg, sizeof(cn->msg));
	if (line_len < 0) {
		fprintf(stderr, "can't read message from the server: %s", strerror(errno));
		return NULL;
	}

	return cn->msg;
}

int check_answer(struct conn_t *cn, const char *answer) {
	int len = strlen(answer);
	if (write(cn->sock, answer, len) != len) {
		fprintf(stderr, "can't write message to server: %s", strerror(errno));
		return -1;
	}

	if (write(cn->sock, "\n", 1) != 1) {
		fprintf(stderr, "can't write \\n to server: %s", strerror(errno));
		return -1;
	}

	int line_len = read_line(cn->sock, cn->msg, sizeof(cn->msg));
	if (line_len < 0) {
		fprintf(stderr, "can't read response from the server: %s", strerror(errno));
		return -1;
	}

	return strcmp(cn->msg, "{\"status\":\"success\"}") ? 0 : 1;
}
