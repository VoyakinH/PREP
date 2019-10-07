#ifndef PROJECT_INCLUDE_UTILS_H_
#define PROJECT_INCLUDE_UTILS_H_

#define TRANSACTION_FILE "transaction.dat"
#define RECORD_FILE "record.dat"
#define BLACK_RECORD_FILE "blackrecord.dat"
#define STRUCT_DEFAULT { 0, "", "", "", "", 0, 0, 0}

struct master_record {
    int number;
    char name[20];
    char surname[20];
    char address[30];
    char tel_number[15];
    double indebtedness;
    double credit_limit;
    double cash_payments;
};
typedef struct master_record data;

void write_master(FILE *ofPTR_1, data client);
void write_transaction(FILE *ofPTR_1, data transfer);
void write_black_record(FILE *ofPTR_1, FILE *ofPTR_2, FILE *black_record, data client_data, data transfer);

#endif  // PROJECT_INCLUDE_UTILS_H_
