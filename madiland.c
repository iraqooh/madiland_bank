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
short pin_number;
char account_number[256];
double balance;
char account_info[256];
char temp_buffer[256];
time_t current_time;
char formatted_time[256];
int tries = 3;

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

void validate_username(const char* username_input, FILE* file) {
	char account_info[256];
	
	while (fgets(account_info, sizeof(account_info), file)) {
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
			
			return;
		}
		
		memset(account_info, 0, sizeof(account_info));
	}
	
	return;
}

bool username_exists(const char* username_input, FILE* file) {
	memset(temp_buffer, 0, sizeof(temp_buffer));
	
	while (fgets(temp_buffer, sizeof(temp_buffer), file)) {
		char* token = strtok(temp_buffer, " ");
		
		if (token != NULL && !strcmp(token, username_input)) {
			return true;
		}
		memset(temp_buffer, 0, sizeof(temp_buffer));
	}
	
	return false;
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
			if (!isalpha(input[0])) return "Username must start with a letter!\n\n";
			for (int index = 0; index < length; index++) {
				if (isspace(input[index])) return "Username must not contain any whitespace!\n\n";
			}
			break;
		case PASSWORD:			
			for (int index = 0; index < length; index++) {
				if (isspace(input[index])) return "Password must not contain any whitespace!\n\n";
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

void mem_clear(char* variable) {
	memset(variable, 0, sizeof(variable));
}

void log_usage(const char* message, FILE* file) {
	if (file == NULL) return;
	
	time(&current_time);
	
	strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M:%S\t", localtime(&current_time));
	
	strcat(formatted_time, message);
	
	fprintf(file, "%s\n", formatted_time);
	
	mem_clear(formatted_time);
	
	return;
}

int main() {
	FILE* logs = fopen("usage_logs.txt", "a");	
	log_usage("Starting session.", logs);
	
	while (true) {	
		FILE* clients = fopen("clients.txt", "r");
		if (clients == NULL) {
			log_usage("Failed to open \"clients.txt\".", logs);
			printf("FOE Error: Contact your system admin or try again later!\n\n");
			sleep(1);
			break;
		}
		
		menu:
		printf("Welcome\n\n1. Login\n2. Register\n3. Exit\n\nSelect option: ");
		scanf(" %c", &option);
		clear_stream();
		
		if (!isdigit(option)) {
			log_usage("Non-digit entry in welcome section.", logs);
			printf("Invalid input! Option must be a number 1 - 3!\n\n");
			sleep(1);
			continue;
		}
		
		switch(option) {
			case '3':
				log_usage("Ending session.", logs);
				printf("Thank you for using Madiland Banking...\nExiting...\n");
				sleep(2);
				return 0;
			case '2':
				log_usage("Attempting new user registration.", logs);
				while (true) {
					printf("\nNew User Registration\n\n");
					printf("Enter first name: ");
					fgets(firstname, sizeof(firstname), stdin);
					firstname[strcspn(firstname, "\n")] = 0;
					
					if (strcmp(firstname, "quit") == 0) {
						log_usage("Cancelling registration at firstname.", logs);
						goto menu;
					}
					
					char* validation_result = validate_input(firstname, NAME);
					
					if (validation_result != NULL) {
						log_usage("Firstname validation failed.", logs);
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
					
					if (strcmp(lastname, "quit") == 0) {
						log_usage("Cancelling registration at lastname.", logs);
						goto menu;
					}
					
					char* validation_result = validate_input(lastname, NAME);
					
					if (validation_result != NULL) {
						log_usage("Lastname validation failed.", logs);
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
					
					if (strcmp(username, "quit") == 0) {
						log_usage("Cancelling registration at username.", logs);
						goto menu;
					}
					
					char* validation_result = validate_input(username, USERNAME);
					
					if (validation_result != NULL) {
						log_usage("Username validation failed.", logs);
						printf("%s", validation_result);
						continue;
					}
					
					if (username_exists(username, clients)) {
						log_usage("Username provided is unavailable.", logs);
						printf("Username already taken. Choose another.\n\n");
						sleep(1);
						continue;
					}
					
					printf("Username: %s\n\n", username);
					break;
				}
				
				while (true) {
					printf("Enter password: ");
					fgets(password1, sizeof(password1), stdin);
					password1[strcspn(password1, "\n")] = 0;
					
					if (strcmp(password1, "quit") == 0) {
						log_usage("Cancelling registration at password.", logs);
						goto menu;
					}
					
					char* validation_result = validate_input(password1, PASSWORD);
					
					if (validation_result != NULL) {
						log_usage("Password validation failed.", logs);
						printf("%s", validation_result);
						continue;
					}
					
					break;
				}
				
				while (true) {
					printf("\nConfirm password: ");
					fgets(password2, sizeof(password2), stdin);
					password2[strcspn(password2, "\n")] = 0;
					
					if (strcmp(password2, "quit") == 0) {
						log_usage("Cancelling registration at password confirmation.", logs);
						goto menu;
					}
					
					char* validation_result = validate_input(password2, PASSWORD);
					
					if (validation_result != NULL) {
						log_usage("Password confirmation validation failed.", logs);
						printf("%s", validation_result);
						continue;
					}
					
					if (strcmp(password1, password2) != 0) {
						log_usage("Passwords do not match.", logs);
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
					
					if (strcmp(email, "quit") == 0) {
						log_usage("Cancelling registration at email.", logs);
						goto menu;
					}
					
					char* validation_result = validate_input(email, EMAIL);
					
					if (validation_result != NULL) {
						log_usage("Email validation failed.", logs);
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
					
					if (strcmp(telephone, "quit") == 0) {
						log_usage("Cancelling registration at telephone.", logs);
						goto menu;
					}
					
					char* validation_result = validate_input(telephone, TELEPHONE);
					
					if (validation_result != NULL) {
						log_usage("Telephone validation failed.", logs);
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
					
					if (strcmp(initialDeposit, "quit") == 0) {
						log_usage("Cancelling registration at deposit.", logs);
						goto menu;
					}
					
					char* validation_result = validate_input(initialDeposit, MONEY);
					
					if (validation_result != NULL) {
						log_usage("Deposit amount validation failed.", logs);
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
					
					if (strcmp(pin, "quit") == 0) {
						log_usage("Cancelling registration at PIN.", logs);
						goto menu;
					}
					
					char* validation_result = validate_input(pin, PIN);
					
					if (validation_result != NULL) {
						log_usage("PIN validation failed.", logs);
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
					log_usage("Account info write to \"clients.txt\" failed.", logs);
					printf("Account information saving failed. Please try again or contact your system admin!\n\n");
					sleep(1);
					break;
				}
					
				sleep(3);
				
				log_usage("New account generated.", logs);
				printf("Your account has been successfully created.\n\n");
				
				mainbank(firstname, lastname, username, password1, email, telephone, account_number, atoi(pin), balance);
				break;
			case '1':
				while (true) {
					printf("Enter username: ");
				    fgets(username, sizeof(username), stdin);
				    username[strcspn(username, "\n")] = 0;
				    
				    if (!strcmp(username, "quit")) {
				    	log_usage("Cancelling login at username.", logs);
				    	printf("\n\n");
				    	goto menu;
					}
				    
				    char* validation_result = validate_input(username, USERNAME);
					
					if (validation_result != NULL) {
						log_usage("Username validation failed during login attempt.", logs);
						printf("%s", validation_result);
						continue;
					}
				    
				    int match;
				    
				    while (fgets(temp_buffer, sizeof(temp_buffer), clients)) {
				        char* token = strtok(temp_buffer, " ");
				        
				        if (strcmp(token, username) == 0) {
				        	match = 1;
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
				            if (token != NULL) strcpy(pin, token);
				            pin_number = atoi(pin);
				            
				            token = strtok(NULL, " ");
				            if (token != NULL) strcpy(initialDeposit, token);
				            
				            balance = atof(initialDeposit);
				            
				            break;
				        }
				        memset(temp_buffer, 0, sizeof(temp_buffer));
				    }
				    
				    if (!match) {
				    	log_usage("Non-existent username provided during login attempt.", logs);
				    	printf("Username \"%s\" does not exist\n\n", username);
				    	mem_clear(username);
				    	sleep(1);
				    	continue;
					} else {
						if (tries == 0 && !strcmp(username, "iraqooh")) {
							log_usage("Attempting login with exhausted tries.", logs);
							printf("You have exceeded your login attempts for today!\n\n");
							break;
						}
						bool authenticated = false;
						
						while (tries > 0) {
							printf("Enter password (max. 3 tries): ");
							
							fgets(password2, sizeof(password2), stdin);
							password2[strcspn(password2, "\n")] = 0;
							
							if (strcmp(password1, "quit") == 0) {
								log_usage("Cancelling registration at password.", logs);
								goto menu;
							}
							
							char* validation_result = validate_input(password2, PASSWORD);
							
							if (validation_result != NULL) {
								log_usage("Password validation failed.", logs);
								printf("%s", validation_result);
								continue;
							}
							
							if (strcmp(password1, password2)) {
								log_usage("Incorrect password entered.", logs);
								tries--;
								
								if (tries == 0) {
									log_usage("Login attempts exhausted.", logs);
									printf("You have exhausted your login attempts for this username! Try again later.\n\n");
									goto menu;
								}
								printf("Incorrect password. %d tries Remaining!\n\n", tries);
								continue;
							} else authenticated = true;
						}
						
						if (authenticated) mainbank(firstname, lastname, username, password1, email, telephone, account_number, pin_number, balance);
					}
				}
				
				fclose(clients);
				
				
				break;
			default:
				log_usage("Non-existent option requested at the welcome menu.", logs);
				printf("Invalid input. Try again!\n\n");
		}
		
		fclose(logs);
	}
	return 0;
}