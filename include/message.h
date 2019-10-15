#ifndef __MESSAGE_H_
#define __MESSAGE_H_

#define MESSAGE_MAX_LEN 2048

struct conn_t;

// `setup` function should be called in the beginning of the program
struct conn_t *new_connection(const char *host, int port);

// `close_connection` closes the connection with the remote server. Should be called at end.
void close_connection(struct conn_t *cn);

// `read_message` function sends a request to the server and waits for the response.
// Function returns a string read from the server.
char *read_message(struct conn_t *cn);

// `send_answer` function sends an answer to the server and waits for response.
// Function returns 1 if the answer was correct, 0 if not and -1 in the case of error.
int check_answer(struct conn_t *cn, const char *answer);

#endif // __MESSAGE_H_
