#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

char option;
char firstname[256];
char password1[256];
char password2[256];
long long int account_number;
double balance;
char account_info[256];
typedef struct {
	char* firstname;
	char* password;
	char* account_number;
	double balance;
} Account;

void clear_stream() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void mainbank(Account current_user) {
	while (true) {
		printf("\n\nMain Menu\n\n1. Transfer Money\n2. Deposit Money\n3. Check Balance\n4. Logout\n\nSelect option: ");
		scanf(" %c", &option);
		clear_stream();
		
		if (!isdigit(option)) {
			printf("Invalid input. Option should be an integer 1 - 4\n\n");
			sleep(1);
			continue;
		}
		
		switch (option) {
			case '4':
				printf("Thank you for using Madiland Banking.\nLogging out...\n\n");
				sleep(2);
				return;
			case '1':
				printf("\n\nTransferring money...\n\n");
				sleep(1);
				break;
			case '2':
				printf("\n\nDepositing money...\n\n");
				sleep(1);
				break;
			case '3':
				printf("\n\nChecking balance...\n\n");
				sleep(1);
				break;
			default:
				printf("\n\nInvalid input. Please try again!\n\n");
				sleep(1);
		}
	}
	
	return;
}

int main() {
	
	FILE* clients = fopen("clients.txt", "a+");
	if (clients == NULL) {
		printf("FOE Error. Contact your system admin!\n");
		return 0;
	}
	
	while (true) {
		printf("Welcome\n\n1. Login\n2. Register\n3. Exit\n\nSelect option: ");
		scanf(" %c", &option);
		clear_stream();
		
		if (!isdigit(option)) {
			printf("Invalid input! Option must be a number 1 - 3!\n\n");
			sleep(1);
			continue;
		}
		
		switch(option) {
			case '3':
				printf("Thank you for using Madiland Banking...\nExiting...\n");
				sleep(2);
				return 0;
			case '2':
				while (true) {
					printf("\nNew User Registration\n\n");
					printf("Enter first name: ");
					fgets(firstname, sizeof(firstname), stdin);
					firstname[strcspn(firstname, "\n")] = 0;
					
					printf("First Name: %s\n\n", firstname);
					break;
				}
				
				while (true) {
					printf("Enter password: ");
					fgets(password1, sizeof(password1), stdin);
					password1[strcspn(password1, "\n")] = 0;
					
					break;
				}
				
				while (true) {
					printf("\nConfirm password: ");
					fgets(password2, sizeof(password2), stdin);
					password2[strcspn(password2, "\n")] = 0;
					
					if (strcmp(password1, password2) != 0) {
						printf("Passwords do not match. Try again!\n\n");
						sleep(1);
						continue;
					}
					
					break;
				}
				
				srand(time(NULL));
				long long int min = 111111111111, max = 9999999999999999;
				account_number = min + rand() % (max - min + 1);
				
				balance = 15000.00;
				
				printf("\nNew Account Info\nName: %s\nPassword: %s\nNumber: %llu\nBalance: %.2lf\n\n",
					firstname, password1, account_number, balance);
				
				fprintf(clients, "%s %s %llu %.2lf\n", firstname, password1, 
					account_number, balance);
					
				sleep(2);
				
				printf("Your account has been successfully created.\n\n");
				
				char account_number_str[256];
				snprintf(account_number_str, sizeof(account_number_str), "%llu", account_number);
				Account current_user = {firstname, password1, account_number_str, balance};
				
				mainbank(current_user);
				break;
			case '1':
				printf("Logging in...\n\n");
				while (fgets(account_info, sizeof(account_info), clients)) {
					char* tokens = strtok(account_info, " ");
					while (tokens != NULL) {
						printf("%s ", tokens);
						tokens = strtok(NULL, " ");
					}
					memset(account_info, 0, sizeof(account_info));
				}
				
				mainbank(current_user);
				break;
			default:
				printf("Invalid input. Try again!\n\n");
		}
	}
	
	fclose(clients);
	return 0;
}