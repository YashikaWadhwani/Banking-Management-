#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <openssl/sha.h> // Added OpenSSL library for SHA-256 hashing
#include <ctype.h>

struct user {
    char username[15];
    char password[65]; // Increased password length to accommodate SHA-256 hash
    char fname[15];
    char lname[15];
    char faname[20];
    char maname[20];
    int date, month, year;
    char address[50];
    char accType[20];
};

struct money {
    char userto[15];
    char userfrom[15];
    long amount;
};

void login(void);
void newID(void);
void display(char*);
void account(char*);
void bal(char*);
void transfer(char*);
int check(char*);
void addmoney(void);
int checkID(char*);
void hash_password(const char*, char*);
void get_password(char*, size_t);
int validate_username(const char*);
FILE* open_file(const char*, const char*);

int main() {
    int choice;
    while (1) {
        system("cls");
        printf("Welcome to My Bank\n");
        printf("\n1. Login\n2. New user, create an account\n3. Exit\n4. Add Money");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: login(); break;
            case 2: newID(); break;
            case 3: exit(0); break;
            case 4: addmoney(); break;
            default: printf("Enter a valid option\n");
        }
        getch();
    }
    return 0;
}

// Function to hash the password using SHA-256 algorithm
void hash_password(const char* password, char* hash) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password, strlen(password), digest);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&hash[i*2], "%02x", (unsigned int)digest[i]);
    }
}

// Function to get password securely without echoing characters
void get_password(char* password, size_t max_len) {
    char ch;
    size_t i = 0;
    while ((ch = getch()) != '\r' && i < max_len - 1) {
        password[i++] = ch;
        printf("*");
    }
    password[i] = '\0';
}

// Function to validate username for alphanumeric characters and length
int validate_username(const char* username) {
    if (strlen(username) > 14) return 0; // Username length check
    for (int i = 0; i < strlen(username); i++) {
        if (!isalnum(username[i])) return 0; // Alphanumeric check
    }
    return 1;
}

// Function to open file with error handling
FILE* open_file(const char* filename, const char* mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    return fp;
}

void newID() {
    struct user u;
    FILE *fp;
    system("cls");
    printf("Welcome new user :) please give us your valuable info\n");
    printf("Username: ");
    scanf(" %s", u.username);
    if (!validate_username(u.username)) { // Validate username
        printf("Invalid username. Use only alphanumeric characters and max 14 characters.\n");
        getch();
        return;
    }
    printf("Password: ");
    char raw_password[10];
    get_password(raw_password, 10);
    hash_password(raw_password, u.password); // Hash password
    printf("First name: ");
    scanf(" %s", u.fname);
    printf("Last name: ");
    scanf(" %s", u.lname);
    printf("Father's name: ");
    scanf(" %s", u.faname);
    printf("Mother's name: ");
    scanf(" %s", u.maname);
    printf("Date of Birth (date/month/year): ");
    scanf(" %d %d %d", &u.date, &u.month, &u.year);
    printf("Address: ");
    scanf(" %s", u.address);
    printf("Account Type: ");
    scanf(" %s", u.accType);
    fp = open_file("user.txt", "ab");
    fwrite(&u, sizeof(u), 1, fp);
    fclose(fp);
    printf("\nNew Account created successfully!!!\nPress Enter to login... ");
    getch();
    login();
}

void login() {
    FILE *fp;
    struct user u;
    char uname[15], upass[10];
    char hashed_password[65]; // Increased password length to accommodate hashed password
    system("cls");
    fp = open_file("user.txt", "rb");
    printf("Welcome to Login Screen\n");
    printf("Type your username: ");
    scanf(" %s", uname);
    printf("Type your password: ");
    get_password(upass, 10);
    hash_password(upass, hashed_password); // Hash password
    int found = 0;
    while (fread(&u, sizeof(u), 1, fp)) {
        if (strcmp(uname, u.username) == 0 && strcmp(hashed_password, u.password) == 0) {
            printf("\nLogin Successful!");
            getch();
            display(uname);
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found) {
        printf("\nLogin Failed");
        getch();
        login();
    }
}

void display(char uname[15]) {
    FILE *fp;
    struct user u;
    fp = open_file("user.txt", "rb");
    while (fread(&u, sizeof(u), 1, fp)) {
        if (strcmp(u.username, uname) == 0) {
            break;
        }
    }
    fclose(fp);
    int choice;
    system("cls");
    printf("Welcome to your account %s\n\n", u.fname);
    printf("1. Account details\n");
    printf("2. Bank Balance and Statement\n");
    printf("3. Money Transfer\n");
    printf("4. Log Out\n");
    printf("5. Main Menu\n");
    printf("Enter your choice: ");
    scanf(" %d", &choice);
    switch (choice) {
        case 1: account(uname); break;
        case 2: bal(uname); break;
        case 3: transfer(uname); break;
        case 4: login(); break;
        case 5: main(); break;
        default: display(uname); break;
    }
    display(uname);
}

void account(char uname[15]) {
    FILE *fp;
    struct user u;
    fp = open_file("user.txt", "rb");
    while (fread(&u, sizeof(u), 1, fp)) {
        if (strcmp(u.username, uname) == 0) {
            break;
        }
    }
    fclose(fp);
    system("cls");
    printf("Your Account Details\n\n");
    printf("Username: %s\n", u.username);
    printf("First name: %s\t Last name: %s\n", u.fname, u.lname);
    printf("Father's name: %s\t Mother's name: %s\n", u.faname, u.maname);
    printf("Date of Birth: %d/ %d/ %d\n", u.date, u.month, u.year);
    printf("Address: %s\n", u.address);
    printf("Account Type: %s\n", u.accType);
    printf("Return to previous screen...");
    getch();
}

void transfer(char ufrom[15]) {
    FILE *fm;
    fm = open_file("money.txt", "ab");
    struct money m;
    char uto[15];
    long tmoney;
    system("cls");
    printf("Money Transfer Screen\n\n");
    printf("Username to transfer: ");
    scanf(" %s", uto);
    if (checkID(uto) == 0) {
        printf("Username not found xox");
        getch();
        display(ufrom);
        return;
    }
    printf("Amount to be transferred: ");
    scanf(" %ld", &tmoney);
    if (tmoney < check(ufrom)) {
        strcpy(m.userfrom, ufrom);
        strcpy(m.userto, uto);
        m.amount = tmoney;
        fwrite(&m, sizeof(m), 1, fm);
        fclose(fm);
        printf("Transferred successfully!!!\nPress Enter to continue...");
    } else {
        printf("Insufficient Balance :(");
    }
    getch();
}

void bal(char uname[15]) {
    FILE *fp, *fm;
    struct money m;
    long total = 0;
    fp = open_file("user.txt", "rb");
    fm = open_file("money.txt", "rb");
    system("cls");
    if (fm == NULL || fp == NULL) {
        printf("Error in opening money file x-x");
    }
    printf("Your Account Statement\n\n");
    printf("From\t\tTo\t\tAmount\t\tStatus\n\n");
    while (fread(&m, sizeof(m), 1, fm)) {
        if (strcmp(uname, m.userto) == 0) {
            printf("%-15s %-15s %-15ld %-15s\n", m.userfrom, m.userto, m.amount, "Credit");
            total += m.amount;
        } else if (strcmp(uname, m.userfrom) == 0) {
            printf("%-15s %-15s %-15ld %-15s\n", m.userfrom, m.userto, m.amount, "Debit");
            total -= m.amount;
        }
    }
    fclose(fp);
    fclose(fm);
    printf("\nTotal Balance:  %ld\n", total);
    printf("\nPress Enter to continue... ");
    getch();
}

void addmoney() {
    FILE *fm;
    fm = open_file("money.txt", "ab");
    struct money m;
    char pass[10], ch, p[10] = "Hack";
    strcpy(m.userfrom, "MASTER");
    system("cls");
    printf("Type the account name to send money to: ");
    scanf(" %s", m.userto);
    if (checkID(m.userto) == 0) {
        printf("Username not found xox");
        getch();
        main();
        return;
    }
    printf("PASSWORD: ");
    get_password(pass, 10);
    if (strcmp(pass, p) != 0) {
        printf("\nWrong Password");
        getch();
        main();
        return;
    } else {
        m.amount = INT_MAX;
        fwrite(&m, sizeof(m), 1, fm);
        fclose(fm);
        printf("\nMoney Transferred Successfully! Enjoy :)");
        printf("\nPress Enter to return to the previous screen...");
    }
    getch();
    main();
}

int check(char uname[15]) {
    FILE *fp, *fm;
    struct user u;
    struct money m;
    long total = 0;
    fp = open_file("user.txt", "rb");
    fm = open_file("money.txt", "rb");
    while (fread(&m, sizeof(m), 1, fm)) {
        if (strcmp(uname, m.userto) == 0) {
            total += m.amount;
        } else if (strcmp(uname, m.userfrom) == 0) {
            total -= m.amount;
        }
    }
    fclose(fp);
    fclose(fm);
    return total;
}

int checkID(char ID[15]) {
    FILE *fp;
    fp = open_file("user.txt", "rb");
    struct user u;
    int match = 0;
    while (fread(&u, sizeof(u), 1, fp)) {
        if (strcmp(ID, u.username) == 0) {
            match = 1;
            break;
        }
    }
    fclose(fp);
    return match;
}

