#include<stdio.h>
#include<conio.h>
#include<windows.h>

struct user{
    char username[15];
    char password[10];
    char fname[15];
    char lname[15];
    char faname[20];
    char maname[20];
    int date,month,year;
    char address[50];
    char accType[20];
};

struct money{
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

int main(){

    int choice;
    system("cls");
    printf("Welcome to My Bank\n");
    printf("\n1.Login\n2.New user, create an account\n3.Exit\n4.Add Money");
    printf("\nEnter your choice: ");
    scanf("%d",&choice);
    switch(choice){
        case 1: login(); break;
        case 2: newID(); break;
        case 3: exit(0); break;
        case 4: addmoney(); break;
        default : printf("Enter a valid option");
        getch();
    }
    main();

    return 0;
}

void newID(){
    struct user u;
    FILE *fp;
    system("cls");
    printf("Welcome new user :) please give us your valueable info\n");
    printf("Username: ");
    scanf(" %s",u.username);
    printf("Password: ");
    scanf(" %s",u.password);
    printf("First name: ");
    scanf(" %s",u.fname);
    printf("Last name: ");
    scanf(" %s",u.lname);
    printf("Father's name: ");
    scanf(" %s",u.faname);
    printf("Mother's name: ");
    scanf(" %s",u.maname);
    printf("Date of Birth(date/month/year): ");
    scanf(" %d %d %d",&u.date,&u.month,&u.year);
    printf("Address: ");
    scanf(" %s",u.address);
    printf("Account Type: ");
    scanf(" %s",u.accType);
    fp=fopen("user.txt","ab");
    fwrite(&u,sizeof(u),1,fp);
    fclose(fp);
    printf("\nNew Account created succesfully!!!\nPress Enter to login... ");
    getch();
    login();
}

void login(){
    FILE *fp;
    struct user u;
    char uname[15], upass[10], ch;
    system("cls");
    fp=fopen("user.txt","rb");
    if(fp==NULL){
        printf("Error in opening file X-X");
    }
    printf("Welcome to Login Screen\n");
    printf("Type your username: ");
    scanf(" %s",uname);
    printf("Type your password: ");
    for(int i=0; i<10; i++){
        ch=getch();
        if(ch!='\r'){
            upass[i]=ch;
            printf("*");
        }
        else{break;}
    }
    while(fread(&u,sizeof(u),1,fp)){
        if(strcmp(uname,u.username)==0 && strcmp(upass,u.password)==0){
            printf("\nLogin Succesfull!");
            getch();
            display(uname);
        }
    }
    printf("\nLogin Fail");
    getch();
    login();
}

void display(char uname[15]){
    FILE *fp;
    struct user u;
    fp=fopen("user.txt","rb");
    while(fread(&u,sizeof(u),1,fp)){
        if(strcmp(u.username,uname)==0){
            break;
        }
    }
    fclose(fp);
    int choice;
    system("cls");
    printf("Welcome to your account %s\n\n",u.fname);
    printf("1.Account details\n");
    printf("2.Bank Balance and Statement\n");
    printf("3.Money Transfer\n");
    printf("4.Log Out\n");
    printf("5.Main Menu\n");
    printf("Enter your choice: ");
    scanf(" %d",&choice);
    switch(choice){
        case 1: account(uname); break;
        case 2: bal(uname); break;
        case 3: transfer(uname); break;
        case 4: login(); break;
        case 5: main(); break;
        default: display(uname); break;
    }
    display(uname);
}

void account(char uname[15]){
    FILE *fp;
    struct user u;
    fp=fopen("user.txt","rb");
    if(fp==NULL){
        printf("Error in opening file X-X");
    }
    while(fread(&u,sizeof(u),1,fp)){
        if(strcmp(u.username,uname)==0){
            break;
        }
    }
    fclose(fp);
    system("cls");
    printf("Your Account Details\n\n");
    printf("Username: %s\n",u.username);
    printf("Password: %s\n",u.password);
    printf("First name: %s\t Last name: %s\n",u.fname,u.lname);
    printf("Father's name: %s\t Mother's name: %s\n",u.faname,u.maname);
    printf("Date of Birth: %d/ %d/ %d\n",u.date,u.month,u.year);
    printf("Address: %s\n",u.address);
    printf("Account Type: %s\n",u.accType);
    printf("Return to previous screen...");
    getch();
}

void transfer(char ufrom[15]){
    FILE *fm;
    fm=fopen("money.txt","ab");
    struct money m;
    char uto[15];
    int tmoney;
    system("cls");
    printf("Money Transfer Screen\n\n");
    printf("Username to transfer: ");
    scanf(" %s",uto);
    if(checkID(uto)==0){
        printf("Username not found xox");
        getch();
        display(ufrom);
    }
    printf("Amount to be transferred: ");
    scanf(" %d",&tmoney);
    if(tmoney<check(ufrom)){
        strcpy(m.userfrom,ufrom);
        strcpy(m.userto,uto);
        m.amount=tmoney;
        fwrite(&m,sizeof(m),1,fm);
        fclose(fm);
        printf("Tansfered succesfully!!!\nPress Enter to continue...");
    }
    else{
        printf("Insufficient Balance :(");
    }
    getch();
}

int check(char uname[15]){
     FILE *fp,*fm;
    struct user u;
    struct money m;
    int total=0;
    fp=fopen("user.txt","rb");
    fm=fopen("money.txt","rb");
    while(fread(&m,sizeof(m),1,fm)){
        if(strcmp(uname,m.userto)==0){
            total+=m.amount;
        }
        else if(strcmp(uname,m.userfrom)==0){
            total-=m.amount;
        }
    }
    fclose(fp);
    fclose(fm);
    return total;
}

void bal(char uname[15]){
    FILE *fp,*fm;
    struct user u;
    struct money m;
    long total=0;
    fp=fopen("user.txt","rb");
    fm=fopen("money.txt","rb");
    system("cls");
    if(fm==NULL || fp==NULL){
        printf("Error in opening money file x-x");
    }
    printf("Your Account Statement\n\n");
    printf("From\t\tTo\t\tAmount\t\tStatus\n\n");
    while(fread(&m,sizeof(m),1,fm)){
        if(strcmp(uname,m.userto)==0){
            printf("%-15s %-15s %-15d %-15s\n",m.userfrom,m.userto,m.amount,"Credit");
            total+=m.amount;
        }
        else if(strcmp(uname,m.userfrom)==0){
            printf("%-15s %-15s %-15d %-15s\n",m.userfrom,m.userto,m.amount,"Debit");
            total-=m.amount;
        }
    }
    fclose(fp);
    fclose(fm);
    printf("\nTotal Balance:  %ld\n",total);
    printf("\nPress Enter to continue... ");
    getch();

}

void addmoney(){
    FILE *fm;
    fm=fopen("money.txt","ab");
    struct money m;
    char pass[10],ch,p[10]="Hack";
    strcpy(m.userfrom,"MASTER");
    system("cls");
    printf("Type the account name to send money to: ");
    scanf(" %s",m.userto);
    if(checkID(m.userto)==0){
        printf("Username not found xox");
        getch();
        main();
    }
    printf("PASSWORD: ");
    for(int i=0; i<10; i++){
        ch=getch();
        if(ch!=13){
            pass[i]=ch;
            printf("*");
        }
        else{
            pass[i]='\0';
            break;
        }
    }
    if(strcmp(pass,p)!=0){
        printf("\nWrong Password");
        getch();
        main();
    }
    else{
        m.amount=INT_MAX;
        fwrite(&m,sizeof(m),1,fm);
        fclose(fm);
        printf("\nMoney Transferred Succesfully! Enjoy :)");
        printf("\nPress Enter to return to previous screen...");
    }
    getch();
    main();
}

int checkID(char ID[15]){
    FILE *fp;
    fp=fopen("user.txt","rb");
    struct user u;
    int match;
    while(fread(&u,sizeof(u),1,fp)){
        if(strcmp(ID,u.username)==0){
            match=1; break;
        }
        else{
            match=0;
        }
    }
    fclose(fp);
    return match;
}