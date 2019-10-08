#include <stdio.h>
#include "utils.h"

int main(void) {
    int choice = 0;
    data client_data = STRUCT_DEFAULT, transfer = STRUCT_DEFAULT;
    printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case 1:
                write_master(&client_data);
                break;
            case 2:
                write_transaction(&transfer);
                break;
            case 3:
                write_black_record(&client_data, &transfer);
                break;
            default:
                puts("error");
                break;
        }
        printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    }
    return 0;
}
