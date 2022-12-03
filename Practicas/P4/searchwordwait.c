#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
void
comprobarargumentos(int nargs)
{
	if (nargs % 2 == 0) {
		errx(EXIT_FAILURE,
		     "searchword usage: ./searchword file1 word1 file2 word2...");
	}
}

void
execfgrep(char *file, char *word)
{
	execl("/bin/fgrep", "fgrep", "-s","-H", "-n", word, file, NULL);
	err(EXIT_FAILURE, "Error in execl : ");
}
void printpids(int listpids[], int npids, char *argv[],int piderror) {
	int j=1;
	for (int i = 0; i < npids; i++) {
		if (piderror == listpids[i]) {
		fprintf(stderr, "el proceso con pid %d ha salido con error catastrofico\n",listpids[i] );
		fprintf(stderr, "fichero: %s\n",argv[j]);
		fprintf(stderr, "palabra: %s\n",argv[j+1]);
		}
		j=j+2;
	}
}
void
waitsons(int pid, int listpids[],int npids,char *argv[])
{
	int sts;

	while ((pid = wait(&sts)) != -1) {
		if (WIFEXITED(sts)) {
			if (WEXITSTATUS(sts) == 2) {
				fprintf(stderr, "Error fgrep, status %d: ",
					WEXITSTATUS(sts));
				printpids(listpids,npids,argv,pid);
			}
		}
	}
}

void
createsons(char *argv[], int argc)
{
	int pid;
	int i;
	int npids = ((argc-1)/2);
	int pids[npids];
	int j=0;
	for (i = 1; i < argc; i = i + 2) {
		switch (pid = fork()) {
		case 1:
			err(EXIT_FAILURE, "fork failed!");
		case 0:
			execfgrep(argv[i], argv[i + 1]);
			break;
		default:
			pids[j]=pid;
			j++;
		}
	}
	waitsons(pid,pids,npids,argv);
}

int
main(int argc, char *argv[])
{
	comprobarargumentos(argc);
	createsons(argv, argc);
	exit(EXIT_SUCCESS);
}
