#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

char option;
char firstname[256];
char lastname[256];
char username[256];
char password1[256];
char password2[256];
char email[256];
char telephone[256];
char initialDeposit[256];
char pin[256];
char account_number[256];
double balance;
char account_info[256];

enum InputType {
	NAME,
	USERNAME,
	PASSWORD,
	EMAIL,
	TELEPHONE,
	MONEY,
	PIN
};

//typedef struct {
//	char* firstname;
//	char* lastname;
//	char* username;
//	char* password;
//	char* email;
//	char* telephone;
//	char* account_number;
//	char* pin_number;
//	double balance;
//} Account;

void clear_stream() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void mainbank(char* firstname, char* lastname, char* username, char* password, char* email, char* telephone, char* account_number, short pin_number, double balance) {
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

void validate_username(const char* username_input) {
	FILE* clients = fopen("clients.txt", "r");
	char account_info[256];
	
	while (fgets(account_info, sizeof(account_info), clients)) {
		char* token = strtok(account_info, " ");
		
		if (token != NULL && strcmp(token, username_input) == 0) {
			strcpy(username, username_input);
			
			token = strtok(NULL, " ");
			if (token != NULL) strcpy(firstname, token);
			
			token = strtok(NULL, " ");
			if (token != NULL) strcpy(lastname, token);
			
			token = strtok(NULL, " ");
			if (token != NULL) strcpy(password1, token);
			
			token = strtok(NULL, " ");
			if (token != NULL) strcpy(email, token);
			
			token = strtok(NULL, " ");
			if (token != NULL) strcpy(telephone, token);
			
			token = strtok(NULL, " ");
			if (token != NULL) strcpy(account_number, token);
			
			token = strtok(NULL, " ");
			if (token != NULL) {
				strcpy(pin, token);
			}
			
			token = strtok(NULL, " ");
			char balance_buffer[256];
			if (token != NULL) strcpy(balance_buffer, token);
			balance = atof(balance_buffer);
			
			fclose(clients);
			
			return;
		}
		
		memset(account_info, 0, sizeof(account_info));
	}
	
	fclose(clients);
	
	return;
}

char* validate_input(char* input, const enum InputType input_type) {
	int length = strlen(input);
	
	if ((input_type == NAME || input_type == USERNAME) && (length < 2 || length > 16)) {
		return "Input should be at least 2 or at most 16 characters long!\n\n";
	}
	
	switch (input_type) {
		case NAME:			
			for (int index = 0; index < length; index++) {
				if (!isalpha(input[index])) return "Names must contain only alphabetic characters!\n\n";
			}
			break;
		case USERNAME:			
			for (int index = 0; index < length; index++) {
				if (isspace(input[index])) return "Names must contain only alphabetic characters!\n\n";
			}
			break;
		case PASSWORD:			
			for (int index = 0; index < length; index++) {
				if (!isalpha(input[index])) return "Usernames must not contain any whitespace!\n\n";
			}
			break;
		case EMAIL:
			if (length < 2 || length > 32) return "Email must be at least 9 and at most 32 characters long!\n\n";
			
			for (int index = 0; index < length; index++) {
				if (isspace(input[index])) return "Email must not contain any whitespace!\n\n";
			}
			break;
		case TELEPHONE:
			if (length < 9 || length > 13) return "Invalid telephone number!\n\n";	
			
			for (int index = 0; index < length; index++) {
				if (!isdigit(input[index]) && input[index] != '+') return "Invalid character!\n\n";
			}
			break;
		case MONEY:			
			for (int index = 0; index < length; index++) {
				if (!isdigit(input[index])) return "Invalid monetary value!\n\n";
			}
			
			if (length < 5) return "Deposit should be at least UGX 15,000.00!\n\n";
			
			balance = atof(input);
			
			if (balance < 15000.0) return "Deposit should be at least UGX 15,000.00!\n\n";
			
			break;
		case PIN:
			if (length != 4) return "PIN must be 4 digits long!\n\n";
			
			for (int index = 0; index < length; index++) {
				if (!isdigit(input[index])) return "PIN must contain only digits!\n\n";
			}
			break;
	}
	return NULL;
}


int main() {
	
	FILE* clients = fopen("clients.txt", "a+");
	if (clients == NULL) {
		printf("FOE Error. Contact your system admin!\n");
		return 0;
	}
	
	while (true) {
		menu:
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
					
					if (strcmp(firstname, "quit") == 0) goto menu;
					
					char* validation_result = validate_input(firstname, NAME);
					
					if (validation_result != NULL) {
						printf("%s", validation_result);
						continue;
					}
					
					printf("First Name: %s\n\n", firstname);
					break;
				}
				
				while (true) {
					printf("Enter last name: ");
					fgets(lastname, sizeof(lastname), stdin);
					lastname[strcspn(lastname, "\n")] = 0;
					
					if (strcmp(lastname, "quit") == 0) goto menu;
					
					char* validation_result = validate_input(lastname, NAME);
					
					if (validation_result != NULL) {
						printf("%s", validation_result);
						continue;
					}
					
					printf("Last Name: %s\n\n", lastname);
					break;
				}
				
				while (true) {
					printf("Enter username: ");
					fgets(username, sizeof(username), stdin);
					username[strcspn(username, "\n")] = 0;
					
					if (strcmp(username, "quit") == 0) goto menu;
					
					char* validation_result = validate_input(username, USERNAME);
					
					if (validation_result != NULL) {
						printf("%s", validation_result);
						continue;
					}
					
					printf("Username: %s\n\n", username);
					break;
				}
				
				while (true) {
					printf("Enter password: ");
					fgets(password1, sizeof(password1), stdin);
					password1[strcspn(password1, "\n")] = 0;
					
					if (strcmp(password1, "quit") == 0) goto menu;
					
					char* validation_result = validate_input(password1, PASSWORD);
					
					if (validation_result != NULL) {
						printf("%s", validation_result);
						continue;
					}
					
					break;
				}
				
				while (true) {
					printf("\nConfirm password: ");
					fgets(password2, sizeof(password2), stdin);
					password2[strcspn(password2, "\n")] = 0;
					
					if (strcmp(password2, "quit") == 0) goto menu;
					
					char* validation_result = validate_input(password2, PASSWORD);
					
					if (validation_result != NULL) {
						printf("%s", validation_result);
						continue;
					}
					
					if (strcmp(password1, password2) != 0) {
						printf("Passwords do not match. Try again!\n\n");
						sleep(1);
						continue;
					}
					
					printf("Password: %s\n\n", password1);
					break;
				}
				
				while (true) {
					printf("Enter email: ");
					fgets(email, sizeof(email), stdin);
					email[strcspn(email, "\n")] = 0;
					
					if (strcmp(email, "quit") == 0) goto menu;
					
					char* validation_result = validate_input(email, EMAIL);
					
					if (validation_result != NULL) {
						printf("%s", validation_result);
						continue;
					}
					
					printf("Email: %s\n\n", email);
					break;
				}
				
				while (true) {
					printf("Enter telephone number: ");
					fgets(telephone, sizeof(telephone), stdin);
					telephone[strcspn(telephone, "\n")] = 0;
					
					if (strcmp(telephone, "quit") == 0) goto menu;
					
					char* validation_result = validate_input(telephone, TELEPHONE);
					
					if (validation_result != NULL) {
						printf("%s", validation_result);
						continue;
					}
					
					printf("Telephone: %s\n\n", telephone);
					break;
				}
				
				while (true) {
					printf("Specify amount of initial deposit (min. UGX 15,000.00): ");
					fgets(initialDeposit, sizeof(initialDeposit), stdin);
					initialDeposit[strcspn(initialDeposit, "\n")] = 0;
					
					if (strcmp(initialDeposit, "quit") == 0) goto menu;
					
					char* validation_result = validate_input(initialDeposit, MONEY);
					
					if (validation_result != NULL) {
						printf("%s", validation_result);
						continue;
					}
					
					printf("Deposit: %s\n\n", initialDeposit);
					break;
				}
				
				while (true) {
					printf("Enter 4-digit PIN: ");
					fgets(pin, sizeof(pin), stdin);
					pin[strcspn(pin, "\n")] = 0;
					
					if (strcmp(pin, "quit") == 0) goto menu;
					
					char* validation_result = validate_input(pin, PIN);
					
					if (validation_result != NULL) {
						printf("%s", validation_result);
						continue;
					}
					
					printf("PIN: %s\n\n", pin);
					break;
				}
				
				srand(time(NULL));
				long long int min = 1111111111111111, max = 9999999999999999;
				long long int acc_number = min + rand() % (max - min + 1);
				snprintf(account_number, sizeof(account_number), "%llu", acc_number);
				
				printf("\n**** New Account Info ****\n\nName: %s %s\nUsername: %s\nPassword: %s\nEmail: %s\nTelephone: %s\nAccount Number: %s\nPIN: %s\nBalance: %.2lf\n\n",
					firstname, lastname, username, password1, email, telephone, account_number, pin, balance);
				
				int saved = fprintf(clients, "%s %s %s %s %s %s %s %s %.2lf\n", username, firstname, lastname, password1, 
					email, telephone, account_number, pin, balance);
					
				if (!saved) {
					printf("Account information saving failed. Please try again or contact your system admin!\n\n");
					sleep(1);
					break;
				}
					
				sleep(3);
				
				printf("Your account has been successfully created.\n\n");
				
				mainbank(firstname, lastname, username, password1, email, telephone, account_number, atoi(pin), balance);
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
				
				mainbank(firstname, lastname, username, password1, email, telephone, account_number, atoi(pin), balance);
				break;
			default:
				printf("Invalid input. Try again!\n\n");
		}
	}
	
	fclose(clients);
	return 0;
}