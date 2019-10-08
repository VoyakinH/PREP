#include <stdio.h>
#include "utils.h"

void write_master() {
    FILE *client_info = fopen(RECORD_FILE, "r+");
    data client = STRUCT_DEFAULT;
    if (client_info == NULL) {
        return_err(RECORD_FILE);
    } else {
        printf("1 Number account:\n"
               "2 Client name:\n"
               "3 Surname:\n"
               "4 Addres client:\n"
               "5 Client Telnum:\n"
               "6 Client indebtedness:\n"
               "7 Client credit limit:\n"
               "8 Client cash payments: \n");
        while (scanf("%d%20s%20s%30s%15s%lf%lf%lf",
                     &client.number,
                     client.name,
                     client.surname,
                     client.address,
                     client.tel_number,
                     &client.indebtedness,
                     &client.credit_limit,
                     &client.cash_payments) == 8) {
            fprintf(client_info, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n",
                    client.number,
                    client.name,
                    client.surname,
                    client.address,
                    client.tel_number,
                    client.indebtedness,
                    client.credit_limit,
                    client.cash_payments);
            printf("1 Number account:\n"
                   "2 Client name:\n"
                   "3 Surname:\n"
                   "4 Addres client:\n"
                   "5 Client Telnum:\n"
                   "6 Client indebtedness:\n"
                   "7 Client credit limit:\n"
                   "8 Client cash payments: \n");
        }
        fclose(client_info);
    }
}

void write_transaction() {
    FILE *client_transaction = fopen(TRANSACTION_FILE, "r+");
    data transfer = STRUCT_DEFAULT;
    if (client_transaction == NULL) {
        return_err(TRANSACTION_FILE);
    } else {
        printf("1 Number account: \n"
               "2 Client cash payments: \n");
        while (scanf("%d %lf", &transfer.number, &transfer.cash_payments) == 2) {
            fprintf(client_transaction, "%-3d%-6.2f\n", transfer.number, transfer.cash_payments);
            printf("1 Number account:\n"
                   "2 Client cash payments: \n");
        }
        fclose(client_transaction);
    }
}

void write_black_record() {
    FILE *client_info = fopen(RECORD_FILE, "r");
    FILE *client_transaction = fopen(TRANSACTION_FILE, "r");
    FILE *new_info = fopen(BLACK_RECORD_FILE, "w");
    data client_data = STRUCT_DEFAULT, transfer = STRUCT_DEFAULT;
    if (check_and_write(client_info, client_transaction, new_info) == 0) {
        while (fscanf(client_info, "%d%20s%20s%30s%15s%lf%lf%lf",
                      &client_data.number,
                      client_data.name,
                      client_data.surname,
                      client_data.address,
                      client_data.tel_number,
                      &client_data.indebtedness,
                      &client_data.credit_limit,
                      &client_data.cash_payments) == 8) {
            while (fscanf(client_transaction, "%d %lf", &transfer.number, &transfer.cash_payments) == 2) {
                if (client_data.number == transfer.number && transfer.cash_payments != 0) {
                client_data.credit_limit += transfer.cash_payments;
                }
            }
            fprintf(new_info, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n",
                    client_data.number,
                    client_data.name,
                    client_data.surname,
                    client_data.address,
                    client_data.tel_number,
                    client_data.indebtedness,
                    client_data.credit_limit,
                    client_data.cash_payments);
            rewind(client_transaction);
        }
        fclose(client_info);
        fclose(client_transaction);
        fclose(new_info);
    }
    return;
}

int return_err(char *str) {
    fprintf(stdout, "Can't open file %s: File not found or error while opening occured", str);
    return 1;
}

int check_and_write(FILE *client_info, FILE *client_transaction, FILE *new_info) {
    if (client_info == NULL || client_transaction == NULL || new_info == NULL) {
        return_err("all files");
        if (client_info)
            fclose(client_info);
        if (client_transaction)
            fclose(client_transaction);
        if (new_info)
            fclose(new_info);
        return 1;
    } else {
        return 0;
    }
}
