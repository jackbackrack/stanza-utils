// See license.txt for details about licensing.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>

typedef struct {
  FILE* in;
  FILE* out;
  FILE* err;
} process_t;

process_t* open_process(char *argv[]){
  // Make pipes
  int file_pipes_i[2];
  int file_pipes_o[2];
  int file_pipes_e[2];
  if(pipe(file_pipes_i) != 0){
    printf("Pipe Failure\n");
    exit(EXIT_FAILURE);
  }
  if(pipe(file_pipes_o) != 0){
    printf("Pipe Failure\n");
    exit(EXIT_FAILURE);
  }
  if(pipe(file_pipes_e) != 0){
    printf("Pipe Failure\n");
    exit(EXIT_FAILURE);
  }

  //Make process
  int fork_result = fork();
  if(fork_result == -1){
    printf("Fork Failure\n");
    exit(EXIT_FAILURE);
  }

  //Child process calls the executable
  if(fork_result == 0){
    //Redirect std pipes
    dup2(file_pipes_i[0], 0);
    dup2(file_pipes_o[1], 1);
    dup2(file_pipes_e[1], 2);
    close(file_pipes_i[0]);
    close(file_pipes_o[1]);
    close(file_pipes_e[1]);
    //Call executable
    if(execv(argv[0], argv) < 0){
      perror("Exec Failure:");
      exit(-1);
    }
    exit(EXIT_FAILURE);      
  }

  //Parent process creates streams 
  close(file_pipes_i[0]);
  close(file_pipes_o[1]);
  close(file_pipes_e[1]);

  process_t* p = malloc(sizeof(process_t));
  p->in  = fdopen(file_pipes_i[1], "w");
  p->out = fdopen(file_pipes_o[0], "r");
  p->err = fdopen(file_pipes_e[0], "r");

  return p;
}
