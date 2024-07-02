# Madiland Banking Application in C

Welcome to the Madiland Banking Application project in C. This application provides essential banking functionalities, including creating an account, transferring and depositing money, checking the balance, and generating transaction receipts.

## Table of Contents
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Author](#author)
- [License](#license)
- [Support](#support)

## Features
- Create an account
- Deposit money
- Withdraw money
- Transfer money between accounts
- Check account balance
- Generate transaction receipts

### Potential Future Features
- **Account Statement Generation**: Generate detailed statements for accounts over a specified period.
- **Loan Management**: Allow users to apply for and manage loans.
- **Interest Calculation**: Calculate and apply interest on savings accounts.
- **Multi-Currency Support**: Support for transactions in different currencies.
- **User Authentication**: Secure login system with passwords and possibly two-factor authentication.
- **Graphical User Interface (GUI)**: A simple graphical interface for ease of use.

## Prerequisites
Before running this application, ensure you have the following:
- A C compiler (e.g., GCC)
- Make utility (optional but recommended)
- Git

## Installation
Follow these steps to get the code and run it on your system:

1. **Clone the Repository**
    ```sh
    git clone https://github.com/iraqooh/madiland_bank.git
    cd madiland_bank
    ```

2. **Compile the Application**
    ```sh
    make
    ```
    Or, if you don't have `make`:
    ```sh
    gcc -o madiland_bank madiland.c.
    ```

3. **Run the Application**
    ```sh
    ./madiland_bank
    ```

## Usage
Here is a brief user manual to help you use the application:

1. **Creating an Account**
    - Launch the application and select the option to create a new account.
    - Provide the necessary details like name, address, initial deposit, etc.

2. **Depositing Money**
    - Choose the deposit option from the main menu.
    - Enter your account number and the amount to deposit.

3. **Withdrawing Money**
    - Select the withdraw option.
    - Enter your account number and the amount to withdraw.

4. **Transferring Money**
    - Choose the transfer option.
    - Provide your account number, the recipient's account number, and the amount to transfer.

5. **Checking Account Balance**
    - Select the balance inquiry option.
    - Enter your account number to view the balance.

6. **Generating Transaction Receipts**
    - After any transaction, the application will prompt you to generate a receipt.
    - Confirm to generate and print the receipt.

## Author
This project is maintained by [Mr. Iraku](https://github.com/iraqooh/).

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Support
For support, please open an issue on the [GitHub repository](https://github.com/iraqooh/madiland_bank/issues).

---

Thank you for using the Madiland Banking Application. We hope it meets your needs for basic banking operations and serves as a valuable project for learning and development.
