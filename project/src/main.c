#include <stdio.h>
#include "utils.h"

int main(void) {
    int choice = 0, indent;
    printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case 1:
                if (write_master(RECORD_FILE) != OK)
                    return_err(RECORD_FILE);
                break;
            case 2:
                if (write_transaction(TRANSACTION_FILE) != OK)
                    return_err(TRANSACTION_FILE);
                break;
            case 3:
                indent = write_black_record(RECORD_FILE, TRANSACTION_FILE, BLACK_RECORD_FILE);
                if (indent == 1) {
                    return_err(RECORD_FILE);
                } else if (indent == 2) {
                    return_err(TRANSACTION_FILE);
                } else if (indent == 3) {
                    return_err(BLACK_RECORD_FILE);
                }
                break;
            default:
                puts("error");
                break;
        }
        printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    }
    return 0;
}
