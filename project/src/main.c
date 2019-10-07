#include <stdio.h>
#include "utils.h"

int main(void) {
    int choice = 0;
    FILE *ptr_1, *ptr_2, *black_record;
    data client_data = STRUCT_DEFAULT, transfer = STRUCT_DEFAULT;
    printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case 1:
                ptr_1 = fopen(RECORD_FILE, "r+");
                if (ptr_1 == NULL) {
                    puts("Not access");
                } else {
                    write_master(ptr_1, client_data);
                    fclose(ptr_1);
                }
                break;
            case 2:
                ptr_1 = fopen(TRANSACTION_FILE, "r+");
                if (ptr_1 == NULL) {
                    puts("Not access");
                } else {
                    write_transaction(ptr_1, transfer);
                    fclose(ptr_1);
                }
                break;
            case 3:
                ptr_1 = fopen(RECORD_FILE, "r");
                ptr_2 = fopen(TRANSACTION_FILE, "r");
                black_record = fopen(BLACK_RECORD_FILE, "w");
                if (ptr_1 == NULL || ptr_2 == NULL || black_record == NULL) {
                    puts("exit");
                } else {
                    write_black_record(ptr_1, ptr_2, black_record, client_data, transfer);
                    fclose(ptr_1);
                    fclose(ptr_2);
                    fclose(black_record);
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
