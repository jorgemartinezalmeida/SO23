

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
enum{
    MaxArguments = 100,
};
struct Process{
    char *comand;
    char *arguments[MaxArguments];
    int narguments;
};
typedef struct Process Process;
void getreadylist(int argc,Process *listaprocess[]) {
    for (size_t i = 0; i < argc-1; i++) {
        if((listaprocess[i]=malloc(sizeof(Process)))== NULL){
            warnx ("Error in malloc");
         exit (EXIT_FAILURE);
        }
    }
}
void freelist(int argc,Process *listaprocess[argc-1]) {
    for (size_t i = 0; i < argc-1; i++) {
        free(listaprocess[i]->comand);
        for (size_t j = 0; j < MaxArguments; j++) {
            free(listaprocess[i]->arguments[j]);
        }
        free(listaprocess[i]);
    }
}
void printlist(int argc,Process *listaprocess[argc-1]) {
    for (size_t i = 0; i < argc-1; i++) {
        fprintf(stderr, "comando %ld %s\n",i,listaprocess[i]->comand);
        for (size_t j = 0; j < listaprocess[i]->narguments; j++) {
            printf("argumen[%ld]%s\n",j,listaprocess[i]->arguments[j]);
        }
    }
}


int tokenization(char *string,char *delimiter, Process *process) {
        int n=0;
        char *token = strtok(string, delimiter);
        if(token != NULL){
            while(token != NULL){
                // Sólo en la primera pasamos la cadena; en las siguientes pasamos NULL
                if (n==0) {

                    process->comand=strndup(token,strlen(token));
                }else{
                    process->arguments[n-1]=strndup(token,strlen(token));
                }
                n++;
                token = strtok(NULL, delimiter);
            }
        }
        process->narguments=n-1;
        return n;
}

int
main(int argc, char *argv[])
{
    Process *listaprocess[argc-1];
    getreadylist(argc,listaprocess);
    for (size_t i = 1; i < argc; i++) {
        fprintf(stderr, "argv[%ld]->%s\n",i,argv[i]);
        tokenization(argv[i]," ",listaprocess[i-1]);
    }
    printlist(argc,listaprocess);

	int fd[2];
	char *str = "I am your father\n";
	int status;

	if (pipe(fd) < 0) {
		err(EXIT_FAILURE, "cannot make a pipe");
	}
	switch (fork()) {
	case -1:
		err(EXIT_FAILURE, "fork failed");
	case 0:
		close(fd[1]);
		if (dup2(fd[0], 0) < 0) {
			err(EXIT_FAILURE, "dup failed");
		}
		close(fd[0]);
		execl("/usr/bin/wc", "wc", "-w", NULL);
		err(EXIT_FAILURE, "exec failed");
	default:
		close(fd[0]);
		if (write(fd[1], str, strlen(str)) != strlen(str)) {
			err(EXIT_FAILURE, "error writting in pipe");
		}
		close(fd[1]);
		if (wait(&status) < 0) {
			status = EXIT_FAILURE;
		}
	}
    freelist(argc,listaprocess);
	exit(status);
}
