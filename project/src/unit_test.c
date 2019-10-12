#include <stdio.h>
#include <string.h>
#include "utils.h"

void write_to_file(FILE *test_file, data expected_data) {
    fprintf(test_file, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n",
            expected_data.number,
            expected_data.name,
            expected_data.surname,
            expected_data.address,
            expected_data.tel_number,
            expected_data.indebtedness,
            expected_data.credit_limit,
            expected_data.cash_payments);
    return;
}

void read_from_file(FILE *test_file, data *got_data) {
    fscanf(test_file, "%d%20s%20s%30s%15s%lf%lf%lf",
           &got_data->number,
           got_data->name,
           got_data->surname,
           got_data->address,
           got_data->tel_number,
           &got_data->indebtedness,
           &got_data->credit_limit,
           &got_data->cash_payments);
    return;
}

int compare_structures(data expected_data, data got_data) {
    if (expected_data.number == got_data.number &&
        strcmp(expected_data.name, got_data.name) == 0 &&
        strcmp(expected_data.surname, got_data.surname) == 0 &&
        strcmp(expected_data.address, got_data.address) == 0 &&
        strcmp(expected_data.tel_number, got_data.tel_number) == 0 &&
        expected_data.indebtedness == got_data.indebtedness &&
        expected_data.credit_limit == got_data.credit_limit &&
        expected_data.cash_payments == got_data.cash_payments) {
        return 0;
    }
    return 1;
}

int test_write_to_file() {
    const char *filename = "test_file.dat";
    data expected_data = { 1, "Alexei", "Voyakin", "Russia", "89161190130", 10, 100, 5 };
    data got_data = STRUCT_DEFAULT;

    FILE *test_file = fopen(filename, "w+");
    
    if (test_file == NULL) {
        return 1;
    }

    write_to_file(test_file, expected_data);

    rewind(test_file);

    read_from_file(test_file, &got_data);

    fclose(test_file);
    if (compare_structures(expected_data, got_data) == 0) {
        return 0;
    }
    return 1;
}

int main() {
    if (test_write_to_file() == 0) {
        printf("Чтение, запись, перезапись выполняются верно");
        return 0;
    }
    printf("Чтение, запись, перезапись выполняются неверно");
    return 1;
}
