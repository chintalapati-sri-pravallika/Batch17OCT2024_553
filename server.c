#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

#define MAXBUFF 1024
#define PORTNO 6789
#define MAX_USERS 100
#define FILE_NAME "cdr_data.txt"

typedef struct {
    char msisdn[20];
    char operator[20];
    int call_duration;
    int sms_count;
    int data_downloaded;
    int data_uploaded;
} CDRRecord;

typedef struct {
    int sockfd;
    struct sockaddr_in client_address;
} client_data;

typedef struct {
    char username[50];
    char password[50];
} User;

User users[MAX_USERS];
int user_count = 0;

CDRRecord cdr_records[MAXBUFF];
int cdr_count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *client_handler(void *arg);
void process_cdr_file(const char *file_path);
void print_billing_info(int choice);
void signup(char *username, char *password);
void signup(char *username, char *password);
int authenticate_user(char *username, char *password);
void handle_client(int client_sock);

int main() {
    int sfd, retValue;
    struct sockaddr_in serv_address;
    client_data client;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        perror("socket() ");
        exit(EXIT_FAILURE);
    }
    printf("[+]Server: Socket Created\n");

    memset(&serv_address, '\0', sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(PORTNO);
    serv_address.sin_addr.s_addr = INADDR_ANY;

    retValue = bind(sfd, (struct sockaddr *)&serv_address, sizeof(serv_address));
    if (retValue < 0) {
        perror("bind() ");
        exit(EXIT_FAILURE);
    }
    printf("[+]Server: Bind Done!\n");

    retValue = listen(sfd, 5);
    if (retValue < 0) {
        perror("listen() ");
        exit(EXIT_FAILURE);
    }
    printf("[+]Server: Listening...\n");

    while (1) {
        client.sockfd = accept(sfd, (struct sockaddr *)&client.client_address, (socklen_t *)&retValue);
        if (client.sockfd < 0) {
            perror("accept() ");
            continue;
        }
        printf("[+]Server: Accepted new client\n");

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, client_handler, (void *)&client) < 0) {
            perror("pthread_create() ");
            exit(EXIT_FAILURE);
        }
        pthread_detach(thread_id);  // Allow the thread to clean up after itself
    }

    close(sfd);
    return 0;
}

void *client_handler(void *arg) {
    client_data *client = (client_data *)arg;
    char msg[MAXBUFF];
    int retValue;
    int choice;

    while (1) {
        retValue = read(client->sockfd, msg, MAXBUFF);
        if (retValue <= 0) {
            printf("[+]Server: Client disconnected\n");
            close(client->sockfd);
            return NULL;
        }

        msg[retValue] = '\0';
        printf("[Client %d] %s\n", client->sockfd, msg);

        if (strcmp(msg, "SignUp") == 0) {
            write(client->sockfd, "Enter Username and Password:", 27);
            read(client->sockfd, msg, MAXBUFF);
            char username[50], password[50];
            sscanf(msg, "%s %s", username, password);
            signup(username, password);
            write(client->sockfd, "SignUp Successful!", 18);
        } else if (strcmp(msg, "Login") == 0) {
            write(client->sockfd, "Enter Username and Password:", 27);
            read(client->sockfd, msg, MAXBUFF);
            char username[50], password[50];
            sscanf(msg, "%s %s", username, password);
            if (authenticate_user(username, password)) {
                write(client->sockfd, "Login Successful! Choose an option:", 35);
                read(client->sockfd, msg, MAXBUFF);
                choice = atoi(msg);
                if (choice == 1) {
                    write(client->sockfd, "Processing CDR File", 20);
                                                                                     process_cdr_file(FILE_NAME);
                } else if (choice == 2) {
                    write(client->sockfd, "Enter 1 for Customer Billing or 2 for Interoperator Settlement Billing", 71);
                    read(client->sockfd, msg, MAXBUFF);
                    print_billing_info(atoi(msg));
                }
            } else {
                write(client->sockfd, "Invalid credentials", 20);
            }
        } else if (strcmp(msg, "exit") == 0) {
            close(client->sockfd);
            break;
        } else {
            write(client->sockfd, "Invalid choice!", 15);
        }
    }
    return NULL;
}

void process_cdr_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("fopen() ");
        return;
    }

    char line[MAXBUFF];
    while (fgets(line, MAXBUFF, file)) {
        CDRRecord record;
        sscanf(line, "%s %s %d %d %d %d", record.msisdn, record.operator, &record.call_duration,
&record.sms_count, &record.data_downloaded, &record.data_uploaded);
        pthread_mutex_lock(&mutex);
        cdr_records[cdr_count++] = record;
        pthread_mutex_unlock(&mutex);
    }

    fclose(file);
    printf("[+]Server: CDR File Processed\n");
}

void print_billing_info(int choice) {
    if (choice == 1) {
        FILE *file = fopen("CB.txt", "w");
        if (file) {
            for (int i = 0; i < cdr_count; i++) {
                fprintf(file, "%s %s %d %d %d %d\n", cdr_records[i].msisdn, cdr_records[i].operator,
                        cdr_records[i].call_duration, cdr_records[i].sms_count,
                                                    cdr_records[i].data_downloaded, cdr_records[i].data_uploaded);
            }
            fclose(file);
            printf("[+]Server: Billing Information written to CB.txt\n");
        }
    } else if (choice == 2) {
        FILE *file = fopen("IOSB.txt", "w");
        if (file) {
            for (int i = 0; i < cdr_count; i++) {
                fprintf(file, "%s %s %d %d %d %d\n", cdr_records[i].msisdn, cdr_records[i].operator,
                        cdr_records[i].call_duration, cdr_records[i].sms_count,
                        cdr_records[i].data_downloaded, cdr_records[i].data_uploaded);
            }
            fclose(file);
            printf("[+]Server: Interoperator Settlement Billing written to IOSB.txt\n");
        }
    }
}

void signup(char *username, char *password) {
    pthread_mutex_lock(&mutex);
    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);
    user_count++;
    pthread_mutex_unlock(&mutex);
}

int authenticate_user(char *username, char *password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1;
        }
    }
    return 0;
}

     