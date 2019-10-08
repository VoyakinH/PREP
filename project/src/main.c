#include <stdio.h>
#include "utils.h"

int main(void) {
    int choice = 0;
    printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case 1:
                write_master();
                break;
            case 2:
                write_transaction();
                break;
            case 3:
                write_black_record();
                break;
            default:
                puts("error");
                break;
        }
        printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    }
    return 0;
}
