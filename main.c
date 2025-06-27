#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

#define STR_SIZE 65536
#define COMMAND_SIZE 256
#define ARGMAX 256

void type_prompt() {
	printf("$ ");
}

// argv array of args terminated with null pointer
void read_command(char* command, char** argv) {
	char str[STR_SIZE];
	fgets(str, STR_SIZE, stdin);
	int wordStart = 0;
	int wordLen;
	int wordIndex = 0;
	int i = 0;
	while(true) {
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\0') {
			wordLen = i-wordStart;
			if (wordIndex == 0) {
				memcpy(command, str + wordStart, wordLen);
				command[wordLen] = '\0';
			}
			argv[wordIndex] = malloc(wordLen+1);
			memcpy(argv[wordIndex], str+wordStart, wordLen);
			argv[wordIndex][wordLen+1] = '\0';
			wordIndex++;
			if (str[i] == '\n' || str[i] == '\0') break;
			
			while(str[i] == ' ') i++;

			wordStart = i;
		}
		else i++;
	}
	argv[wordIndex] = NULL;
//	printf("%s\n", command);
//	while(*argv != NULL) {
//		printf("%s\n", *argv);
//		argv++;
//	}
}

int main() {
	char command[COMMAND_SIZE];
	char* argv[ARGMAX];
	int status = 0;
	while(true) {
		type_prompt();
		read_command(command, argv);
		if(fork() != 0) {
			waitpid(-1, &status, 0);
			printf("%d\n", status);
		} else {
			return execve(command, argv, 0);
		}
	}
}
