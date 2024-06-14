// library file imports
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

// global variables
char option;
char firstname[256];
char lastname[256];
char username[256];
char password1[256];
char password2[256];
char email[256];
char telephone[13];
char transfer_amount[256];
char pin[256];
char account_number[256];
double balance;
char temp_buffer[256];
time_t current_time;
char formatted_time[256];
int tries = 3;
FILE* logs;
FILE* clients;
bool authenticated = false;

// user account structure
typedef struct {
    char firstname[256];
    char lastname[256];
    char username[256];
    char password[256];
    char email[256];
    char telephone[256];
    char account_number[256];
    char pin[256];
    double balance;
} Account;

// constants
char banks[][128] = {
	"Stanbic",
	"Centenary",
	"DFCU",
	"Post",
	"Equity"
};

enum InputType {
	NAME,
	USERNAME,
	PASSWORD,
	EMAIL,
	TELEPHONE,
	ACC_NUMBER,
	PIN,
	MONEY
};

// function declarations
void mem_clear(char* variable);
void log_usage(const char* message, FILE* file);
void get_menu_option();
char* validate_input(char* input, const enum InputType input_type);
int get_string_input(const char* user_prompt, char* variable, enum InputType input_type);
void register_user(FILE* log_file, FILE* clients_file, Account* account);
char* username_exists(char* username, FILE* log_file, FILE* clients_file);
void login_user(FILE* log_file, FILE* clients_file, Account* account);

void dashboard() {
	printf("You are logged in!\n\n");
	return;
}


// main application
int main() {
	// application initializations
	logs = fopen("usage_logs.txt", "a");	
	log_usage("Starting session.", logs);
	Account current_account;
	
	// main application loop
	while (true) {
		// load the user accounts data
		clients = fopen("clients.txt", "a+");
		if (clients == NULL) {
			log_usage("Failed to open \"clients.txt\".", logs);
			printf("FOE Error: Contact your system admin or try again later!\n\n");
			break;
		}
		
		// landing section menu
		printf("Welcome\n\n1. Login\n2. Register\n3. Exit\n\nSelect option: ");
		
		// prompt user and retrieve selection
		get_menu_option();
		
		// validate option
		if (!isdigit(option)) {
			log_usage("Non-digit entry in welcome section.", logs);
			printf("Invalid input! Option must be a number 1 - 3!\n\n");
			continue;
		}
		
		// main application control flow decisions
		switch(option) {
			case '3':
				// exit application
				fclose(clients);
				log_usage("Ending session.", logs);
				printf("Thank you for using Madiland Banking...\nExiting...\n");
				exit(0);
			case '2':
				// register new user
				register_user(logs, clients, &current_account);
				break;
			case '1':
				// login user
				login_user(logs, clients, &current_account);
				break;
			default:
				log_usage("Non-existent option requested at the welcome menu.", logs);
				printf("Invalid input. Try again!\n\n");
		}
		
		if (authenticated) dashboard();
	}
	
	return 0;
}

// function definitions

/*
* Takes a pointer to a character array variable
* and sets the value of each element in the array to '\0'
*/
void mem_clear(char* variable) {
	memset(variable, 0, sizeof(variable));
}

/* 
* Takes an activity or operation information and an output file
* and logs the date, time and nature of user activity to the specified file
*/
void log_usage(const char* message, FILE* file) {
	if (file == NULL) return;
	
	time(&current_time);
	
	strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M:%S\t", localtime(&current_time));
	
	strcat(formatted_time, message);
	
	fprintf(file, "%s\n", formatted_time);
	
	mem_clear(formatted_time);
	
	return;
}

/* 
* Retrieves the selected menu selection
* and clears the stream for the next input.
*/
void get_menu_option() {
	scanf(" %c", &option);
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	return;
}

/*
* Validates the user input according to the specified input type
*/
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
			for (int index = 0; index < length; index++) {
				if (!isdigit(input[index]) && input[index] != '+') return "Invalid character!\n\n";
			}
			
			if (length < 10 || length > 13) {
				return "Invalid telephone number!\n\n";
			}
			
			break;
		case ACC_NUMBER:
			if (length != 16) return "Invalid account number!\n\n";	
			
			for (int index = 0; index < length; index++) {
				if (!isdigit(input[index])) return "Invalid character!\n\n";
			}
			break;
		case MONEY:			
			for (int index = 0; index < length; index++) {
				if (!isdigit(input[index])) return "Invalid monetary value!\n\n";
			}
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

/*
* Prompts the user for string input using the user_prompt statement
* and stores the input in the variable,
* then it validates it against the specified input type
*/

int get_string_input(const char* user_prompt, char* variable, enum InputType input_type) {
	printf("%s: ", user_prompt);
//	fgets(variable, sizeof(variable), stdin);
	gets(variable);
//	variable[strcspn(variable, "\n")] = 0;
	
	if (strcmp(variable, "quit") == 0 || strcmp(variable, "back") == 0 || strcmp(variable, "cancel") == 0) {
		log_usage("Cancelling user input operation.", logs);
		return -1;
	}
	
	char* validation_result = validate_input(variable, input_type);
	
	if (validation_result != NULL) {
		log_usage("Input validation failed.", logs);
		printf("%s", validation_result);
		return 0;
	}
	
	return 1;
}


/*
* Initiates the new user registration protocol involving prompting
* for user credentials and initial deposit
*/
void register_user(FILE* log_file, FILE* clients_file, Account* account) {
	log_usage("Attempting new user registration.", log_file);
	
	// main registration protocol loop
	// first name
	while (true) {
		printf("\nNew User Registration\n\n");
		
		int result = get_string_input("Enter first name", firstname, NAME);
		
		if (result == 0) continue;
		else if (result == -1) {
			log_usage("Cancelling registration.", logs);
			return;
		}
		
		printf("First Name: %s\n\n", firstname);
		break;
	}
	
	// last name
	while (true) {
		int result = get_string_input("Enter last name", lastname, NAME);
		
		if (result == 0) continue;
		else if (result == -1) {
			log_usage("Cancelling registration.", logs);
			return;
		}
		
		printf("Last Name: %s\n\n", firstname);
		break;
	}
	
	// username
	while (true) {
		int result = get_string_input("Enter username", username, USERNAME);
		
		if (result == 0) continue;
		else if (result == -1) return;
		
		printf("Username: %s\n\n", username);
		break;
	}
	
	// password
	while (true) {
		int result = get_string_input("Enter password", password1, PASSWORD);
		
		if (result == 0) continue;
		else if (result == -1) return;
		
		break;
	}
	
	// password confirmation
	while (true) {
		int result = get_string_input("Confirm password", password2, PASSWORD);
		
		if (result == 0) continue;
		else if (result == -1) return;
		
		if (strcmp(password1, password2)) {
			printf("Passwords do not match!\n\n");
			continue;
		} 
		
		printf("Password: %s\n\n", password1);
		break;
	}
	
	// email
	while (true) {
		int result = get_string_input("Enter email", email, EMAIL);
		
		if (result == 0) continue;
		else if (result == -1) return;
		
		printf("Email: %s\n\n", email);
		break;
	}
	
	// telephone
	while (true) {
		int result = get_string_input("Enter telephone number", telephone, TELEPHONE);
		
		if (result == 0) continue;
		else if (result == -1) return;
		
		printf("Telephone: %s\n\n", telephone);
		break;
	}
	
	// PIN
	while (true) {
		int result = get_string_input("Enter PIN", pin, PIN);
		
		if (result == 0) continue;
		else if (result == -1) return;
		
		printf("PIN: %s\n\n", pin);
		break;
	}
	
	// initial deposit
	while (true) {
		int result = get_string_input("Enter first deposit (min. UGX 15,000.00)", transfer_amount, MONEY);
		
		if (result == 0) continue;
		else if (result == -1) return;
		
		balance = atof(transfer_amount);
		
		if (balance < 15000.0) {
			log_usage("Attempting to first deposit less than UGX 15,000.00", logs);
			printf("Deposit should be at least UGX 15,000.00!\n\n");
			continue;
		} 
		
		printf("Initial deposit: %.2lf\n\n", balance);
		
		break;
	}
	
	// Generating a random account number of 16 digits
	// **************************** check to determine if account already exists still in development ***************************
	srand(time(NULL));
	long long int min = 4111111111111111, max = 9999999999999999;
	long long int acc_number = min + rand() % (max - min + 1);
	snprintf(account_number, sizeof(account_number), "%llu", acc_number);
	
	// displaying account details to the new user
	// ************************ option to print or email this information still in development ************************************
	printf("\n**** New Account Info ****\n\nName: %s %s\nUsername: %s\nPassword: %s\nEmail: %s\nTelephone: %s\nAccount Number: %s\nPIN: %s\nBalance: %.2lf\n\n",
		firstname, lastname, username, password1, email, telephone, account_number, pin, balance);
	
	// saving the details of the new account user to the clients file
	int saved = fprintf(clients_file, "%s %s %s %s %s %s %s %s %.2lf\n", username, firstname, 
			lastname, password1, email, telephone, account_number, pin, balance);
	
	// checking if saving to file was successful	
	if (!saved) {
		log_usage("Account info write to \"clients.txt\" failed.", logs);
		printf("Account information saving failed. Please try again or contact your system admin!\n\n");
		return;
	}
	
	// populating the user's information in the Account structure
	strcpy(account->firstname, firstname);
	strcpy(account->lastname, lastname);
	strcpy(account->username, username);
	strcpy(account->password, password1);
	strcpy(account->email, email);
	strcpy(account->telephone, telephone);
	strcpy(account->account_number, account_number);
	strcpy(account->pin, pin);
	account->balance = balance;
	
	log_usage("New account generated.", log_file);
	printf("Your account has been successfully created.\n\n");
	
	// confirming authentication of user to redirect them to the dashboard section
	authenticated = true;
	
	return;
}

/* 
* starts the login protocol involving prompting the user for their username and password,
* validating the input, checking if username exists in the clients file 
* and authenticating the user
*/
void login_user(FILE* log_file, FILE* clients_file, Account* account) {
	log_usage("Attempting login.", log_file);
	
	while (tries > 0) {
		// username
		while (true) {
			int result = get_string_input("Enter username", username, USERNAME);
		
			if (result == 0) continue;
			else if (result == -1) return;
			
			break;
		}
		
		// password
		while (true) {
			int result = get_string_input("Enter password", password1, PASSWORD);
			
			if (result == 0) continue; 
			else if (result == -1) return;
			
			break;
		}
		
		// checking if credentials provided are correct
		char* account_info = username_exists(username, log_file, clients_file);
		if (account_info == NULL) {
			printf("Username or password is incorrect!\n\n");
			log_usage("Incorrect username or password entered.", log_file);
			tries--;
			
			if (tries == 0) {
				log_usage("Login attempts exhausted.", log_file);
				printf("You have exhausted your login attempts for this username! Try again later.\n\n");
				return;
			}
			printf("Incorrect password. %d tries Remaining!\n\n", tries);
			continue;
		} else {
			// username found
			strcpy(account->username, username);
			
			char* token = strtok(account_info, " ");
			if (token != NULL) {
				// extract first name
				token = strtok(NULL, " ");
	            if (token != NULL) strcpy(account->firstname, token);
	            
	            // extract last name
	            token = strtok(NULL, " ");
	            if (token != NULL) strcpy(account->lastname, token);
	            
	            // extract password
	            token = strtok(NULL, " ");
	            
	            // check if passwords match
	            if (token != NULL && strcmp(token, password1) == 0) {
	            	// password is correct
	            	strcpy(account->password, password1);
	            	
	            	// extract email
	            	token = strtok(NULL, " ");
		            if (token != NULL) strcpy(account->email, token);
		            
		            // extract telephone
		            token = strtok(NULL, " ");
		            if (token != NULL) strcpy(account->telephone, token);
		            
		            // extract account number
		            token = strtok(NULL, " ");
		            if (token != NULL) strcpy(account->account_number, token);
		            
		            // extract pin number
		            token = strtok(NULL, " ");
		            if (token != NULL) strcpy(account->pin, token);
		            
		            // extract account balance
		            token = strtok(NULL, " ");
		            if (token != NULL) account->balance = atof(token);
		            
		            log_usage("Login successful.", log_file);
		            
		            authenticated = true;
		            
		            return;
		            
		        } else {
		        	// passwords do not match
		        	printf("Username or password is incorrect!\n\n");
					log_usage("Incorrect username or password entered.", log_file);
					tries--;
					
					if (tries == 0) {
						log_usage("Login attempts exhausted.", log_file);
						printf("You have exhausted your login attempts for this username! Try again later.\n\n");
						return;
					}
					printf("Incorrect password. %d tries Remaining!\n\n", tries);
					continue;
				}
			}
			
		}
	}
	
	return;
}

/*
* Reads the clients file to compare the provided username 
* against every username on file and returns the account details,
* else NULL is returned
*/
char* username_exists(char* username_input, FILE* log_file, FILE* clients_file) {
	memset(temp_buffer, 0, sizeof(temp_buffer));
	
	while (fgets(temp_buffer, sizeof(temp_buffer), clients_file)) {
		char temp_buffer_2[strlen(temp_buffer)];
		strcpy(temp_buffer_2, temp_buffer);
		
		char* token = strtok(temp_buffer_2, " ");
		
		if (token != NULL && !strcmp(token, username_input)) {
			log_usage("Username found during login attempt.", log_file);
			
			return temp_buffer;
		}
		memset(temp_buffer, 0, sizeof(temp_buffer));
	}
	
	log_usage("Username not found during login attempt.", log_file);
	return NULL;
}

// to be doc'd
//void mainbank(Account* account) {
//	while (true) {
//		printf("\n\nMain Menu\n\n1. Transfer Money\n2. Deposit Money\n3. Check Balance\n4. Logout\n\nSelect option: ");
//		scanf(" %c", &option);
//		clear_stream();
//		
//		if (!isdigit(option)) {
//			printf("Invalid input. Option should be an integer 1 - 4\n\n");
//			sleep(1);
//			continue;
//		}
//		
//		switch (option) {
//			case '4':
//				printf("Thank you for using Madiland Banking.\nLogging out...\n\n");
//				sleep(2);
//				return;
//			case '1':
//				printf("\n\nTransferring money...\n\n");
//				sleep(1);
//				break;
//			case '2':
//				printf("\n\nDepositing money...\n\n");
//				sleep(1);
//				break;
//			case '3':
//				printf("\n\nChecking balance...\n\n");
//				sleep(1);
//				break;
//			default:
//				printf("\n\nInvalid input. Please try again!\n\n");
//				sleep(1);
//		}
//	}
//	
//	return;
//}

//void transfer_money() {
//	while (true) {
//	    while (true) {
//	    	menu_2:
//	        printf("Select payment channel: ");
//	    
//	        printf("1. Bank\n2. Mobile Money (in development)\n3. PayPal (in development)\n4. Back");
//	        
//	        scanf(" %c", &option);
//	        clear_stream();
//	        
//	        if (option != '1') {
//	        	printf("Feature %d is in development.\n\n", option);
//	        	continue;
//			}
//			
//			while (true) {
//		        printf("Select recipient's bank: ");
//		    
//		        printf("1. Stanbic\n2. Centenary\n3. DFCU\n4. Post\n5. Equity\n6. Back\n\nEnter option: ");
//		        
//		        scanf(" %c", &option);
//	        	clear_stream();
//	        	
//	        	switch (option) {
//	        		case '6':
//	        			goto menu_2;
//	        		case '1':
//	        		case '2':
//	        		case '3':
//	        		case '4':
//	        		case '5':
//	        			while (true) {
//					        printf("Enter recipient's account number: ");
//					    
//					        fgets(temp_buffer, sizeof(temp_buffer), stdin);
//					        temp_buffer[strcspn(temp_buffer, "\n")] = 0;
//					        
////					        int validation_result = validate_input(buffer, MONEY);
//							
//							
//					        
//					        break;
//					    }
//	        			break;
//	        		default:
//	        			printf("Invalid input\n\n");
//				}
//		    }
//	        
//	        break;
//	    }
//	    
//	    
//	    
//	    while (true) {
//	        printf("Balance: UGX %.2lf:\nEnter amount: ", balance);
//	    
//	        scanf("%lf", &transfer_amount);
//	    
//	        if (transfer_amount <= (balance - 5000.0)) {
//	            balance -= transfer_amount;
//	            
//	            FILE* clients = fopen("clients.txt", "r+");
//	            while (fgets(buffer, sizeof(buffer), clients)) {
//	                char* token = strtok(buffer, " ");
//	                if (!strcmp(token, "iraqooh")) {
//	                	printf("Match at %d\n\n", ftell(clients));
//	                	
//	                    break;
//	                }
//	            }
//	            
//	            
//	            
//	            printf("Transfer of UGX %.2lf to %s complete.", transfer_amount, rec_account);
//	            
//	        } else {
//	            printf("Insufficient balance!\n\n");
//	            continue;
//	        }
//	        
//	        break;
//	    }
//	    
//	    break;
//	}
//}