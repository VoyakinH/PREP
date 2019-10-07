#include <stdio.h>
#include "utils.h"

void write_master(FILE *ofPTR_1, data client) {
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
           "1 Number account: ",
           "2 Client name: ",
           "3 Surname: ",
           "4 Addres client: ",
           "5 Client Telnum: ",
           "6 Client indebtedness: ",
           "7 Client credit limit: ",
           "8 Client cash payments: ");
    while (scanf("%d%20s%20s%30s%15s%lf%lf%lf",
                 &client.number,
                 client.name,
                 client.surname,
                 client.address,
                 client.tel_number,
                 &client.indebtedness,
                 &client.credit_limit,
                 &client.cash_payments) != -1) {
        fprintf(ofPTR_1, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n",
                client.number,
                client.name,
                client.surname,
                client.address,
                client.tel_number,
                client.indebtedness,
                client.credit_limit,
                client.cash_payments);
        printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
               "1 Number account: ",
               "2 Client name: ",
               "3 Surname: ",
               "4 Addres client: ",
               "5 Client Telnum: ",
               "6 Client indebtedness: ",
               "7 Client credit limit: ",
               "9 Client cash payments:");
    }
}

void write_transaction(FILE *ofPtr_1, data transfer) {
    printf("%s\n%s\n",
           "1 Number account: ",
           "2 Client cash payments: ");
    while (scanf("%d %lf", &transfer.number, &transfer.cash_payments) != -1) {
        fprintf(ofPtr_1, "%-3d%-6.2f\n", transfer.number, transfer.cash_payments);
        printf("%s\n%s\n",
               "1 Number account:",
               "2 Client cash payments: ");
    }
}

void write_black_record(FILE *ofPTR_1, FILE *ofPTR_2, FILE *black_record, data client_data, data transfer) {
    while (fscanf(ofPTR_1, "%d%20s%20s%30s%15s%lf%lf%lf",
                  &client_data.number,
                  client_data.name,
                  client_data.surname,
                  client_data.address,
                  client_data.tel_number,
                  &client_data.indebtedness,
                  &client_data.credit_limit,
                  &client_data.cash_payments) != -1) {
        while (fscanf(ofPTR_2, "%d %lf", &transfer.number, &transfer.cash_payments) != -1) {
            if (client_data.number == transfer.number && transfer.cash_payments != 0) {
                client_data.credit_limit += transfer.cash_payments;
            }
        }
        fprintf(black_record, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n",
                client_data.number,
                client_data.name,
                client_data.surname,
                client_data.address,
                client_data.tel_number,
                client_data.indebtedness,
                client_data.credit_limit,
                client_data.cash_payments);
        rewind(ofPTR_2);
    }
}
