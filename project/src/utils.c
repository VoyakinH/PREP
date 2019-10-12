#include <stdio.h>
#include "utils.h"

int write_master(char *client_info_file_name) {
    FILE *client_info = fopen(client_info_file_name, "r+");
    data client = STRUCT_DEFAULT;

    if (client_info == NULL) {
        return FILE_OPEN_ERR;
    }
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
    return OK;
}

int write_transaction(char *client_transaction_file_name) {
    FILE *client_transaction = fopen(client_transaction_file_name, "r+");
    data transfer = STRUCT_DEFAULT;

    if (client_transaction == NULL) {
        return FILE_OPEN_ERR;
    }
    printf("1 Number account: \n"
           "2 Client cash payments: \n");

    while (scanf("%d %lf", &transfer.number, &transfer.cash_payments) == 2) {
        fprintf(client_transaction, "%-3d%-6.2f\n", transfer.number, transfer.cash_payments);
        printf("1 Number account:\n"
               "2 Client cash payments: \n");
    }

    fclose(client_transaction);
    return OK;
}

int write_black_record(char *client_info_file_name, char *client_transaction_file_name,
                       char *new_info_file_name) {
    FILE *client_info = fopen(client_info_file_name, "r");
    FILE *client_transaction = fopen(client_transaction_file_name, "r");
    FILE *new_info = fopen(new_info_file_name, "w");
    data client_data = STRUCT_DEFAULT;
    data transfer = STRUCT_DEFAULT;

    int rc = check_opening_files(client_info, client_transaction, new_info);
    if (rc == 0) {
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
        return OK;
    }
    return rc;
}

int return_err(char *str) {
    fprintf(stdout, "Can't open file %s: File not found or error while opening occured", str);
    return 1;
}

int check_opening_files(FILE *client_info, FILE *client_transaction, FILE *new_info) {
    if (client_info == NULL || client_transaction == NULL || new_info == NULL) {
        int what_file = 0;

        if (client_info) {
            what_file = 1;
            fclose(client_info);
        }
        if (client_transaction) {
            what_file = 2;
            fclose(client_transaction);
        }
        if (new_info) {
            what_file = 3;
            fclose(new_info);
        }
        return what_file;
    } else {
        return 0;
    }
}
