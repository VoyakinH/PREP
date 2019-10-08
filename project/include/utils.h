#ifndef PROJECT_INCLUDE_UTILS_H_
#define PROJECT_INCLUDE_UTILS_H_

#define TRANSACTION_FILE "transaction.dat"
#define RECORD_FILE "record.dat"
#define BLACK_RECORD_FILE "blackrecord.dat"
#define FILE_OPEN_ERR 1
#define OK 0
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

int write_master(char *client_info_file_name);
int write_transaction(char *client_transaction_file_name);
int write_black_record(char *client_info_file_name, char *client_transaction_file_name,
                       char *new_info_file_name);
int return_err(char *str);
int check_and_write(FILE *client_info, FILE *client_transaction, FILE *new_info);

#endif  // PROJECT_INCLUDE_UTILS_H_
