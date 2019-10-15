#include "message.h"
#include <string.h>
#include <stdio.h>

#define SRV_HOST "127.0.0.1"
#define SRV_PORT 12345

int main() {
	struct conn_t *cn = new_connection(SRV_HOST, SRV_PORT);
	if (!cn) {
		return -1;
	}

	for (;;) {
		char *msg = read_message(cn);
		if (!msg) {
			return -1;
		}

		printf("Got message: %s\n", msg);
		if (strstr(msg, "\"type\":\"search_query\"")) {
			break;
		}
	}

	printf("Answer is correct: %d\n",
			check_answer(cn, "7b10d858-25e4-48d2-b245-ee04f86f98b5"));

	close_connection(cn);
	return 0;
}
