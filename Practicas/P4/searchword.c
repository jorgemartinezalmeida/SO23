#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
struct Process {
	int pid;
	char *file;
	char *word;
};
typedef struct Process Process;

void
freeprocess(int npids, Process processlist[npids])
{
	for (size_t i = 0; i < npids; i++) {
		free(processlist[i].file);
		free(processlist[i].word);
	}
}

void
checkarguments(int nargs)
{
	if (nargs % 2 == 0) {
		errx(EXIT_FAILURE,
		     "searchword usage: ./searchword file1 word1 file2 word2...");
	}
}

void
execfgrep(char *file, char *word)
{
	execl("/bin/fgrep", "fgrep", "-s", "-H", "-n", word, file, NULL);
	err(EXIT_FAILURE, "Error in execl : ");
}

void
printpids(int npids, int piderror, Process processlist[npids])
{
	for (int i = 0; i < npids; i++) {
		if (piderror == processlist[i].pid) {
			fprintf(stderr,
				"the process-> pid: %d catastrophic error\n",
				processlist[i].pid);
			fprintf(stderr, "file: %s\n", processlist[i].file);
			fprintf(stderr, "word: %s\n", processlist[i].word);
		}
	}
}

int
waitsons(int pid, int npids, Process processlist[npids])
{
	int sts;
	int exit = 0;

	while ((pid = wait(&sts)) != -1) {
		if (WIFEXITED(sts)) {
			if (WEXITSTATUS(sts) == 2) {
				fprintf(stderr, "Error fgrep, status %d: ",
					WEXITSTATUS(sts));
				printpids(npids, pid, processlist);
				exit = 1;
			}
		}
	}
	return exit;
}

int
createsons(char *argv[], int argc)
{
	int pid;
	int i, status;
	int npids = ((argc - 1) / 2);
	Process processlist[npids];
	int j = 0;

	for (i = 1; i < argc; i = i + 2) {
		switch (pid = fork()) {
		case 1:
			err(EXIT_FAILURE, "fork failed!");
		case 0:
			execfgrep(argv[i], argv[i + 1]);
			break;
		default:
			processlist[j].pid = pid;
			if ((processlist[j].word =
			     malloc(sizeof(argv[i + 1]))) == NULL) {
				err(EXIT_FAILURE, "malloc error");
			}
			if ((processlist[j].file =
			     malloc(sizeof(argv[i]))) == NULL) {
				err(EXIT_FAILURE, "malloc error");
			}
			strcpy(processlist[j].word, argv[i + 1]);
			strcpy(processlist[j].file, argv[i]);
			j++;
		}
	}
	status = waitsons(pid, npids, processlist);
	freeprocess(npids, processlist);
	return status;
}

int
main(int argc, char *argv[])
{
	checkarguments(argc);
	exit(createsons(argv, argc));

}
