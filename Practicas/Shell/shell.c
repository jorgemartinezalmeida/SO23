#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{ Maxsize = 1024 + 1,
    MaxComands=100,
    MaxArguments=100,
};

struct Process{
    char *comand;
    char *arguments[MaxArguments];
    int n_arguments;
};
typedef struct Process Process;
struct Data{
    int n_comands;
    Process *Process[MaxComands];
};
typedef struct Data Data;
void datamem(Data *data){
    if ((data = malloc(sizeof(Data)))==NULL){
            warnx ("Error in malloc");
         exit (EXIT_FAILURE);
    }
    for (size_t i = 0; i < MaxComands; i++) {
        if ((data->Process[i]= malloc(sizeof(Process)))==NULL){
            warnx ("Error in malloc");
         exit (EXIT_FAILURE);
        }
    }
}
void freedata(Data *data){
    for (size_t i = 0; i < data->n_comands; i++) {
        for (size_t j = 0; j < data->Process[i]->n_arguments; j++) {
            free(data->Process[i]->arguments[j]);
        }
            free(data->Process[i]->comand);
            free(data->Process[i]);
    }
            free(data);

}
void tokenization_inline(char *string,char *delimiter, Data *data) {
        int n=0;
        char *token = strtok(string, delimiter);
        if(token != NULL){
            while(token != NULL){
                fprintf(stderr, "token %s\n", token );
                data->Process[n]->comand=strndup(token,strlen(token));
                n++;
                token = strtok(NULL, delimiter);
            }
        }
        data->n_comands=n;
}
void tokenization_proccess(char *delimiter, Data *data) {
        for (size_t i = 0; i < data->n_comands; i++) {
            int n=0;
            char *string = strndup(data->Process[i]->comand,strlen(data->Process[i]->comand));
            char *token = strtok(string, delimiter);
            if(token != NULL){
                while(token != NULL){
                    data->Process[i]->arguments[n]=strndup(token,strlen(token));
                    n++;
                    token = strtok(NULL, delimiter);
                }
            }
            data->Process[i]->n_arguments=n;
            free(string);
            }
}
void print_data(Data *data) {
    for (size_t i = 0; i < data->n_comands; i++) {
        fprintf(stderr, "comando sin tokenizar: %s\n",data->Process[i]->comand);
        for (size_t j = 0; j < data->Process[i]->n_arguments; j++) {
            fprintf(stderr, "comando[%ld]: %s\n",j,data->Process[i]->arguments[j] );
            /* code */
        }
    }

}

int main(int argc, char *argv[]) {
    Data *data = NULL;

    if ((data = malloc(sizeof(Data)))==NULL){
            warnx ("Error in malloc");
         exit (EXIT_FAILURE);
    }
    for (size_t i = 0; i < MaxComands; i++) {
        if ((data->Process[i]= malloc(sizeof(Process)))==NULL){
            warnx ("Error in malloc");
         exit (EXIT_FAILURE);
     }}
    while (1) {

        char user_in[20];
        fprintf(stderr, "Promt--> ");
        fgets(user_in,20,stdin);
        tokenization_inline(user_in,"|",data);
        tokenization_proccess(" ",data);
        print_data(data);
        fprintf(stderr, "n_cmds-->%d\n",data->n_comands);

        /* code */
        
    }

    exit(EXIT_SUCCESS);
}
