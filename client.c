#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>
// Function declarations
void establish_connection();
void display_main_menu();
void send_signup_request();
void send_login_request();
void display_post_login_menu();
void send_process_cdr_request();
void display_billing_info_menu();
void display_customer_billing_menu();
void display_interoperator_billing_menu();
void send_search_msisdn_request();
void send_search_operator_request();
void logout();
void close_connection();
void log_message(const char *level, const char *message);

// Global variables
int sockfd;
struct sockaddr_in server_addr;
FILE *log_file;

// Function to establish connection to the server
void establish_connection() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        log_message("FATAL", "Failed to create socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6789);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        log_message("FATAL", "Failed to connect to server");
        exit(EXIT_FAILURE);
    }

    log_message("INFO", "Connected to server");
}

// Function to display the main menu
void display_main_menu() {
    printf("\nMain Menu:\n");
    printf("1. SignUp\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf("Choice: ");
}

// Function to send signup request to the server
void send_signup_request() {
    char username[50], password[50];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    // Send the signup request to the server
    char request[100];
    snprintf(request, sizeof(request), "SIGNUP|%s|%s", username, password);
    send(sockfd, request, strlen(request), 0);
}

// Function to send login request to the server
void send_login_request() {
    char username[50], password[50];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    // Send the login request to the server
    char request[100];
    snprintf(request, sizeof(request), "LOGIN|%s|%s", username, password);
    send(sockfd, request, strlen(request), 0);
}

// Function to display post-login menu
void display_post_login_menu() {
    printf("\nPost Login Menu:\n");
    printf("1. Process CDR file\n");
    printf("2. Print/Search Billing Information\n");
    printf("3. Logout\n");
    printf("Choice: ");
}

// Function to send a CDR processing request to the server
void send_process_cdr_request() {
    char file_name[50];
    printf("Enter CDR file name to process: ");
    scanf("%s", file_name);

    // Send the CDR processing request to the server
    char request[100];
    snprintf(request, sizeof(request), "PROCESS_CDR|%s", file_name);
    send(sockfd, request, strlen(request), 0);
}

// Function to display billing info menu
void display_billing_info_menu() {
    printf("\nBilling Information Menu:\n");
    printf("1. Customer Billing\n");
    printf("2. Interoperator Settlement Billing\n");
    printf("Choice: ");
}

// Function to display customer billing menu
void display_customer_billing_menu() {
    printf("\nCustomer Billing Menu:\n");
    printf("1. Search by MSISDN\n");
    printf("2. Dump all customer billing data to CB.txt\n");
    printf("Choice: ");
}

// Function to display interoperator billing menu
void display_interoperator_billing_menu() {
    printf("\nInteroperator Settlement Billing Menu:\n");
    printf("1. Search by Brand Name/Operator ID\n");
    printf("2. Dump all interoperator billing data to IOSB.txt\n");
    printf("Choice: ");
}

// Function to send MSISDN search request for customer billing
void send_search_msisdn_request() {
    char msisdn[50];
    printf("Enter MSISDN to search: ");
    scanf("%s", msisdn);

    // Send the MSISDN search request to the server
    char request[100];
    snprintf(request, sizeof(request), "SEARCH_MSISDN|%s", msisdn);
    send(sockfd, request, strlen(request), 0);
}

               // Function to send search request for interoperator billing by operator
void send_search_operator_request() {
    char operator_id[50];
    printf("Enter Operator ID to search: ");
    scanf("%s", operator_id);

    // Send the operator search request to the server
    char request[100];
    snprintf(request, sizeof(request), "SEARCH_OPERATOR|%s", operator_id);
    send(sockfd, request, strlen(request), 0);
}

// Function to logout and close the connection
void logout() {
    send(sockfd, "LOGOUT", 6, 0);
    log_message("INFO", "Logged out");
}

// Function to close the connection to the server
void close_connection() {
    close(sockfd);
    log_message("INFO", "Connection closed");
}

// Function to log messages with different severity levels
void log_message(const char *level, const char *message) {
    log_file = fopen("client_log.txt", "a");
    if (log_file == NULL) {
        printf("Error opening log file\n");
        return;
    }

    time_t now;
    time(&now);
    fprintf(log_file, "[%s] [%s] %s\n", ctime(&now), level, message);
    fclose(log_file);
}

// Main function
int main() {
    int choice;

    establish_connection();
   while(1)
   {
        display_main_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                send_signup_request();
                break;
            case 2:
                send_login_request();
                break;
            case 3:
                close_connection();
                exit(0);
            default:
                printf("Invalid choice, try again.\n");
                break;
        }

        // After login, show post-login menu
        display_post_login_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                send_process_cdr_request();
                break;
            case 2:
                display_billing_info_menu();
                scanf("%d", &choice);

                if (choice == 1) {
                    display_customer_billing_menu();
                    scanf("%d", &choice);
                    if (choice == 1) {
                        send_search_msisdn_request();
                    } else if (choice == 2) {
                        // Logic for dumping to CB.txt
                    }
                } else if (choice == 2) {
                    display_interoperator_billing_menu();
                    scanf("%d", &choice);
                    if (choice == 1) {
                        send_search_operator_request();
                    } else if (choice == 2) {
                        // Logic for dumping to IOSB.txt
                    }
                }
                break;
            case 3:
                logout();
                break;
            default:
                printf("Invalid choice, try again.\n");
                break;
        }
    }

    return 0;
}


          