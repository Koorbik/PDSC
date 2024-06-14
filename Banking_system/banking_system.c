#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>


#define MAX_INPUT 128
#define MAX_DEPOSIT 1000000
#define MAX_MONEY (INT32_MAX - MAX_DEPOSIT)
#define MAX_LOAN 1000000
#define INTEREST_RATE 0.1
#define PESEL_LENGTH 11
#define BANK_CODE 945
#define FIXED_CHECK_DIGITS 00 
#define IBAN_LENGTH 28

typedef struct Account {
    int id;
    char account_number[IBAN_LENGTH + 1];  
    char name[MAX_INPUT];
    char surname[MAX_INPUT];
    char address[MAX_INPUT];
    char pesel[MAX_INPUT];
    double current_balance;
    double loan_balance;
    double interest_rate;
} Account;

void welcome_screen();
char *strlwr(char *str);
bool does_file_exist(char *path);
void count_accounts_in_db(FILE *file_path, int *acc_in_db);
void initialize_database(FILE **file_path, int *id);
void generate_account_number(Account *account);
void get_string(char *prompt, char *input, int maxLength);
double get_double(char *prompt);
void get_names(char *prompt, char *input);
bool validate_pesel(const char input[]);
void get_pesel(Account *account);
bool validate_address(const char input[]);
void get_address(Account *account);
bool get_confirmation(char *prompt);
void create_account(int *id, FILE *file_path);
void get_user_input(int *choice);
void print_account(Account *account);
void list_all_accounts(FILE *file_path, int id);
void search_by_acc_num(FILE *file_path, char *acc_num);
void search_by_name(FILE *file_path, char *name);
void search_by_surname(FILE *file_path, char *surname);
void search_by_adress(FILE *file_path, char *address);
void search_by_pesel(FILE *file_path, char *pesel);
void print_search_menu(FILE *file_path);
void deposit(FILE *file_path, char *acc_num, double money, bool *success);
void withdraw(FILE *file_path, char* acc_num, double money, bool *success);
void transfer(FILE *file_path, char *sender_acc_num, char *receiver_acc_num, double money);
void take_a_loan(FILE *file_path, char *acc_num);
void pay_debt(FILE *file_path, char *acc_num, double money);
void bank_loop(FILE *file_path, int id);


void welcome_screen() 
{
    printf("Welcome to the banking system!\n");
    printf("1. Create an account\n");
    printf("2. List all accounts\n");
    printf("3. Search for accounts\n");
    printf("4. Deposit\n");
    printf("5. Withdraw\n");
    printf("6. Take a loan\n");
    printf("7. Pay Debt\n");
    printf("8. Transfer\n");
    printf("9. Exit\n");
}

char *strlwr(char *str)
{
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = tolower((unsigned char)*p);
      p++;
  }

  return str;
}

bool does_file_exist(char *path) 
{
    if(access(path, F_OK) != -1) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}

void count_accounts_in_db(FILE *file_path, int *acc_in_db)
{
    Account temp;
    *acc_in_db = 0;

    rewind(file_path);
    while(fread(&temp, sizeof(Account), 1, file_path) == 1) 
    {
        (*acc_in_db)++;
    }
}

void initialize_database(FILE **file_path, int *id) 
{    
    if(!does_file_exist("database.dat"))
    {
        if((*file_path = fopen("database.dat", "wb")) == NULL) 
        {
            perror("Error opening database\n");
            exit(1);
        }
    }
    else 
    {
        if((*file_path = fopen("database.dat", "r+b")) == NULL) 
        {
            perror("Error opening database\n");
            exit(1);
        }
        count_accounts_in_db(*file_path, id);
    }

    fclose(*file_path);
}

void generate_account_number(Account *account)
{
    char account_number[21]; 
    for (int i = 0; i < 20; i++) {
        account_number[i] = '0' + rand() % 10;
    }
    account_number[20] = '\0';
    
    snprintf(account->account_number, IBAN_LENGTH + 1, "PL%03d%02d%s", BANK_CODE, FIXED_CHECK_DIGITS, account_number);
}

void get_string(char *prompt, char *input, int maxLength)
{
    printf("%s", prompt);
    fgets(input, maxLength, stdin);
    if (input[strlen(input) - 1] == '\n') 
    {
        input[strlen(input) - 1] = '\0';
    }
}

double get_double(char *prompt)
{
    double input;
    printf("%s", prompt);
    while(scanf("%lf", &input) != 1) 
    {
        while(getchar() != '\n');
        printf("Invalid input. Please try again.\n");
    }
    while(getchar() != '\n');
    return input;
}

void get_names(char *prompt, char *input)
{
    bool is_valid;
    do
    {
        is_valid = true;
        get_string(prompt, input, MAX_INPUT);
        for (int i = 0; i < strlen(input); i++) 
        {
            if (!isalpha(input[i])) 
            {
                is_valid = false;
                printf("Invalid input. Please try again.\n");
                break;
            }
        }
    } while(!is_valid);
}

bool validate_pesel(const char input[])
{
    if(strlen(input) != PESEL_LENGTH) 
    {
        return false;
    }
    for (int i = 0; i < PESEL_LENGTH; i++) 
    {
        if(!isdigit(input[i])) 
        {
            return false;
        }
    }
    return true;
}

void get_pesel(Account *account)
{
    char input[MAX_INPUT];

    do {
        get_string("Enter your PESEL: ", input, MAX_INPUT);
        if(!validate_pesel(input)) 
        {
            printf("Invalid PESEL. Please try again.\n");
        }
    } while(!validate_pesel(input));
    strcpy(account->pesel, input);
}

bool validate_address(const char input[])
{
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isalnum(input[i]) && input[i] != '/' && input[i] != '-' && input[i] != ' ' && input[i] != '.' && input[i] != ',') {
            return false;
        }
    }
    return true;
}

void get_address(Account *account)
{
    char input[MAX_INPUT];

    do {
        get_string("Enter your address: ", input, MAX_INPUT);
        if(!validate_address(input)) 
        {
            printf("Invalid address. Please try again.\n");
        }
    } while(!validate_address(input));
    strcpy(account->address, input);
}

bool get_confirmation(char *prompt)
{
    char input;
    printf("%s", prompt);

    while(scanf("%c", &input) != 1 || (tolower(input) != 'y' && tolower(input) != 'n')) 
    {
        while(getchar() != '\n');
        printf("Invalid input. Please try again.\n");
    }

    if (tolower(input) == 'y') 
    {
        return true;
    } 
    return false;
}

void create_account(int *id, FILE *file_path)
{
    Account new_account;
    memset(&new_account, 0, sizeof(Account));
    new_account.id = ++(*id);

    get_names("Enter your name: ", new_account.name);
    get_names("Enter your surname: ", new_account.surname);
    get_address(&new_account);
    get_pesel(&new_account);
    generate_account_number(&new_account);
    new_account.current_balance = 0.0;
    new_account.loan_balance = 0.0;
    new_account.interest_rate = 0.0;

    if (!get_confirmation("Do you want to create an account with the following details? (y/n): ")) 
    {
        printf("Account creation cancelled.\n");
        (*id)--;
        return;
    }

    if((file_path = fopen("database.dat", "r+b")) == NULL) 
    {
        perror("Error opening database\n");
        exit(1);
    }

    fseek(file_path, 0, SEEK_END);

    if(fwrite(&new_account, sizeof(Account), 1, file_path) != 1) 
    {
        perror("Error writing to database\n");
        exit(1);
    }

    printf("Account created successfully!\n");

    fclose(file_path);
}

void get_user_input(int *choice)
{
    do {
        *choice = (int)get_double("Enter your choice: ");
        printf("\n\n");
    } while(*choice < 1 || *choice > 9);
}

void print_account(Account *account)
{
    printf("Account details:\n");
    printf("%-20s %-30d\n", "ID:", account->id);
    printf("%-20s %-30s\n", "Account number:", account->account_number);
    printf("%-20s %-30s\n", "Name:", account->name);
    printf("%-20s %-30s\n", "Surname:", account->surname);
    printf("%-20s %-30s\n", "Address:", account->address);
    printf("%-20s %-30s\n", "PESEL:", account->pesel);
    printf("%-20s %-30.2f\n", "Current balance:", account->current_balance);
    printf("%-20s %-30.2f\n", "Loan balance:", account->loan_balance);
    printf("%-20s %-30.2f\n", "Interest rate:", account->interest_rate);
}

void list_all_accounts(FILE *file_path, int id)
{
    if(id < 1)
    {
        printf("No accounts in the database.\n");
        return;
    }
    else 
    {
        Account temp;
        if((file_path = fopen("database.dat", "r+b")) == NULL) 
        {
            perror("Error opening database\n");
            exit(1);
        }
        for (int i = 0; i < id; i++) 
        {
           if (fread(&temp, sizeof(Account), 1, file_path) != 1) 
           {
               perror("Error reading from database\n");
               exit(1);
           }
           print_account(&temp);
           printf("\n");
        }

        fclose(file_path);
    }
}

void search_by_acc_num(FILE *file_path, char *acc_num)
{
    Account temp;
    bool found = false;

    if((file_path = fopen("database.dat", "r+b")) == NULL) 
    {
        perror("Error opening database\n");
        exit(1);
    }

    while(fread(&temp, sizeof(Account), 1, file_path) == 1) 
    {
        if(strcmp(temp.account_number, acc_num) == 0) 
        {
            print_account(&temp);
            found = true;
            break;
        }
    }

    if(!found) 
    {
        printf("Account not found.\n");
    }

    fclose(file_path);
}

void search_by_name(FILE *file_path, char *name)
{
    Account temp;
    bool found = false;
    char temp_name[MAX_INPUT];

    if((file_path = fopen("database.dat", "r+b")) == NULL) 
    {
        perror("Error opening database\n");
        exit(1);
    }

    while(fread(&temp, sizeof(Account), 1, file_path) == 1) 
    {
        strcpy(temp_name, temp.name);
        if(strcmp(strlwr(temp_name), strlwr(name)) == 0) 
        {
            print_account(&temp);
            found = true;
        }
    }

    if(!found) 
    {
        printf("Account not found.\n");
    }

    fclose(file_path);
}

void search_by_surname(FILE *file_path, char *surname)
{
    Account temp;
    bool found = false;
    char temp_surname[MAX_INPUT];

    if((file_path = fopen("database.dat", "r+b")) == NULL) 
    {
        perror("Error opening database\n");
        exit(1);
    }

    while(fread(&temp, sizeof(Account), 1, file_path) == 1) 
    {
        strcpy(temp_surname, temp.surname);
        if(strcmp(strlwr(temp_surname), strlwr(surname)) == 0) 
        {
            print_account(&temp);
            found = true;
        }
    }

    if(!found) 
    {
        printf("Account not found.\n");
    }

    fclose(file_path);
}

void search_by_adress(FILE *file_path, char *address)
{
    Account temp;
    bool found = false;

    if((file_path = fopen("database.dat", "r+b")) == NULL) 
    {
        perror("Error opening database\n");
        exit(1);
    }

    while(fread(&temp, sizeof(Account), 1, file_path) == 1) 
    {
        if(strcmp(temp.address, address) == 0) 
        {
            print_account(&temp);
            found = true;
        }
    }

    if(!found) 
    {
        printf("Account not found.\n");
    }

    fclose(file_path);
}

void search_by_pesel(FILE *file_path, char *pesel)
{
    Account temp;
    bool found = false;

    if((file_path = fopen("database.dat", "r+b")) == NULL) 
    {
        perror("Error opening database\n");
        exit(1);
    }

    while(fread(&temp, sizeof(Account), 1, file_path) == 1) 
    {
        if(strcmp(temp.pesel, pesel) == 0) 
        {
            print_account(&temp);
            found = true;
        }
    }

    if(!found) 
    {
        printf("Account not found.\n");
    }

    fclose(file_path);
}

void print_search_menu(FILE *file_path)
{
    int choice;
    char input[MAX_INPUT];

    printf("1. Search by account number\n");
    printf("2. Search by name\n");
    printf("3. Search by surname\n");
    printf("4. Search by address\n");
    printf("5. Search by PESEL\n");
    printf("6. Return to main menu\n");

    choice = (int)get_double("Enter your choice: ");

    switch (choice)
    {
    case 1:
        get_string("Enter account number: ", input, IBAN_LENGTH);
        search_by_acc_num(file_path, input);
        printf("\n");
        break;
    case 2:
        get_string("Enter name: ", input, MAX_INPUT);
        search_by_name(file_path, input);
        printf("\n");
        break;
    case 3:
        get_string("Enter surname: ", input, MAX_INPUT);
        search_by_surname(file_path, input);
        printf("\n");
        break;
    case 4:
        get_string("Enter address: ", input, MAX_INPUT);
        search_by_adress(file_path, input);
        printf("\n");
        break;
    case 5:
        get_string("Enter PESEL: ", input, PESEL_LENGTH+1);
        search_by_pesel(file_path, input);
        printf("\n");
        break;
    case 6:
        break;
    default:
        printf("Invalid choice. Please try again.\n");
        break;
    }
}

void deposit(FILE *file_path, char *acc_num, double money, bool *success)
{
    Account temp;
    bool found = false;

    if((file_path = fopen("database.dat", "r+b")) == NULL) 
    {
        perror("Error opening database\n");
        exit(1);
    }

    if (money > MAX_DEPOSIT) 
    {
        printf("Deposit amount exceeds the limit.\n");
        fclose(file_path);
        return;
    }
    else if (money <= 0) 
    {
        printf("Invalid deposit amount.\n");
        fclose(file_path);
        return;
    }

    while(fread(&temp, sizeof(Account), 1, file_path)) 
    {
        if(strcmp(temp.account_number, acc_num) == 0) 
        {
            if(temp.current_balance + money > MAX_MONEY) 
            {
                printf("Deposit amount exceeds the limit.\n");
                fclose(file_path);
                return;
            }
            temp.current_balance += money;
            *success = true;
            fseek(file_path, -sizeof(Account), SEEK_CUR);
            fwrite(&temp, sizeof(Account), 1, file_path);
            found = true;
            break;
        }
    }
    
    if(!found) 
    {
        printf("Account not found.\n");
    }


    fclose(file_path);
}

void withdraw(FILE *file_path, char* acc_num, double money, bool *success)
{
    Account temp;
    bool found = false;

    if((file_path = fopen("database.dat", "r+b")) == NULL) 
    {
        perror("Error opening database\n");
        exit(1);
    }

    if (money > MAX_DEPOSIT) 
    {
        printf("Withdrawal amount exceeds the limit.\n");
        fclose(file_path);
        return;
    }
    else if (money <= 0) 
    {
        printf("The money amount must be positive.\n");
        fclose(file_path);
        return;
    }

    while(fread(&temp, sizeof(Account), 1, file_path)) 
    {
        if(strcmp(temp.account_number, acc_num) == 0) 
        {
            if(temp.current_balance - money < 0) 
            {
                printf("Insufficient funds.\n");
                fclose(file_path);
                return;
            }
        temp.current_balance -= money;
        *success = true;
        fseek(file_path, -sizeof(Account), SEEK_CUR);
        fwrite(&temp, sizeof(Account), 1, file_path);
        found = true;
        break;
        }
    }

    if(!found) 
    {
        printf("Account not found.\n");
    }

    fclose(file_path);
}

void transfer(FILE *file_path, char *sender_acc_num, char *receiver_acc_num, double money)
{
    bool successful_deposit = false;
    bool successful_withdrawal = false;
    withdraw(file_path, sender_acc_num, money, &successful_withdrawal);

    if(successful_withdrawal) 
    {
        deposit(file_path, receiver_acc_num, money, &successful_deposit);
        if(successful_deposit) 
        {
            printf("Transfer successful.\n");
        }
    }
    else 
    {
        printf("Transfer failed.\n");
    }
}

void take_a_loan(FILE *file_path, char *acc_num)
{
    Account temp;
    double loan_amount = get_double("Enter the amount you want to borrow: ");
    printf("\n");

    if (loan_amount > MAX_LOAN) 
    {
        printf("Loan amount exceeds the limit.\n");
        return;
    }
    else if (loan_amount <= 0) 
    {
        printf("Loan amount must be positive.\n");
        return;
    }

    if(!get_confirmation("Do you want to take a loan? (y/n): ")) 
    {
        printf("Loan cancelled.\n");
        return;
    }

    

    if((file_path = fopen("database.dat", "r+b")) == NULL) 
    {
        perror("Error opening database\n");
        exit(1);
    }

    while(fread(&temp, sizeof(Account), 1, file_path)) 
    {
        if(strcmp(temp.account_number, acc_num) == 0) 
        {
            if(temp.current_balance + loan_amount > MAX_MONEY) 
            {
                printf("Loan amount exceeds the limit.\n");
                return;
            }
            
            temp.current_balance += loan_amount;
            temp.interest_rate += INTEREST_RATE;

            temp.loan_balance += (1+ temp.interest_rate) * loan_amount;

            fseek(file_path, -sizeof(Account), SEEK_CUR);
            fwrite(&temp, sizeof(Account), 1, file_path);
            break;
        }
    }
    bool success = false;
    deposit(file_path, acc_num, loan_amount, &success);

    if(success) 
    {
        printf("Loan taken successfully.\n");
    }
    else if(!success)
    {
        printf("Loan failed.\n");
    }

    fclose(file_path);
}

void pay_debt(FILE *file_path, char *acc_num, double money)
{
    Account temp;
    bool found = false;
    bool success = false;

    if((file_path = fopen("database.dat", "r+b")) == NULL) 
    {
        perror("Error opening database\n");
        exit(1);
    }

    if (money > MAX_DEPOSIT) 
    {
        printf("Payment amount exceeds the limit.\n");
        fclose(file_path);
        return;
    }
    else if (money <= 0) 
    {
        printf("Invalid payment amount.\n");
        fclose(file_path);
        return;
    }

    while(fread(&temp, sizeof(Account), 1, file_path))
    {
        if(strcmp(temp.account_number, acc_num) == 0)
        {
            found = true;

            if(temp.loan_balance == 0)
            {
                printf("No loan to pay off.\n");
                fclose(file_path);
                return;
            }
            else if(temp.loan_balance - money < 0) 
            {
                printf("Loan balance is lower than the payment amount.\n");
                fclose(file_path);
                return;
            }
            else if(temp.current_balance - money < 0) 
            {
                printf("Insufficient funds.\n");
                fclose(file_path);
                return;
            }
            else 
            {
                temp.current_balance -= money;
                if(money >= temp.loan_balance)
                {
                    temp.loan_balance = 0;
                    temp.interest_rate = 0;
                }
                else
                {
                    temp.loan_balance -= money;
                }
                success = true;
                fseek(file_path, -sizeof(Account), SEEK_CUR);
                fwrite(&temp, sizeof(Account), 1, file_path);
                break;
            }
        }
    }
    if (success) 
    {
        printf("Payment completed.\n");
    }
    else
    {
        printf("Payment failed.\n");
    }

    if(!found) 
    {
        printf("Account not found.\n");
    }

    fclose(file_path);
}
void bank_loop(FILE *file_path, int id)
{
    int choice;
    double money;
    char input[IBAN_LENGTH + 1];
    char receiver_acc_num[IBAN_LENGTH + 1];

    while(true) 
    {
        welcome_screen();
        get_user_input(&choice);
        switch(choice)
        {
            case 1:
                create_account(&id, file_path);
                printf("\n");
                break;
            case 2:
                list_all_accounts(file_path, id);
                if (!get_confirmation("Do you want to go back to main menu? (y/n): ")) 
                {
                    printf("Exiting the banking system...\n");
                    exit(0);
                }
                printf("\n");
                break;
            case 3:
                print_search_menu(file_path);
                break;
            case 4:
                get_string("Enter account number: ", input, IBAN_LENGTH);
                money = get_double("Enter the amount you want to deposit: ");
                if (!get_confirmation("Do you want to continue? (y/n): "))
                {
                    printf("Deposit cancelled.\n");
                    break;
                }
                bool success = false;
                deposit(file_path, input, money, &success);
                if(success) 
                {
                    printf("Successful deposit of %.2lf PLN\n", money);
                }
                printf("\n");
                break;
            case 5:
                get_string("Enter account number: ", input, IBAN_LENGTH);
                money = get_double("Enter the amount you want to withdraw: ");
                if (!get_confirmation("Do you want to continue? (y/n): "))
                {
                    printf("Withdrawal cancelled.\n");
                    break;
                }
                bool success_withdraw = false;
                withdraw(file_path, input, money, &success_withdraw);
                if(success_withdraw) 
                {
                    printf("Successful withdrawal of %.2lf PLN\n", money);
                }
                printf("\n");
                break;
            case 6:
                get_string("Enter account number: ", input, IBAN_LENGTH+1);
                take_a_loan(file_path, input);
                printf("\n");
                break;
            case 7:
                get_string("Enter account number: ", input, IBAN_LENGTH+1);
                money = get_double("Enter the amount you want to pay off: ");
                if (!get_confirmation("Do you want to pay off the loan? (y/n): "))
                {
                    printf("Payment cancelled.\n");
                    break;
                }
                pay_debt(file_path, input, money);
                printf("\n");
                break;
            case 8:
                get_string("Enter sender account number: ", input, IBAN_LENGTH+1);
                input[IBAN_LENGTH + 1] = '\0';
                get_string("Enter receiver account number: ", receiver_acc_num, IBAN_LENGTH + 1);
                money = get_double("Enter the amount you want to transfer: ");
                if (!get_confirmation("Do you want to transfer money? (y/n): "))
                {
                    printf("Transfer cancelled.\n");
                    break;
                }
                transfer(file_path, input, receiver_acc_num, money);
                printf("\n");
                break;
            case 9:
                printf("Exiting the banking system...\n");
                exit(0);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}

int main() {
    srand(time(NULL));
    
    int id = 0;
    FILE *file_path;
    initialize_database(&file_path, &id);
    bank_loop(file_path, id);
    return 0;
}
