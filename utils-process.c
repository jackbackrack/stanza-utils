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
  int   failure;
} process_t;

typedef enum {
  NO_FAILURE_CODE   =  0,
  PIPE_FAILURE_CODE = -1,
  FORK_FAILURE_CODE = -2,
  EXEC_FAILURE_CODE = -3,
  EXIT_FAILURE_CODE = -4
} FAILURE_CODES;

process_t* open_process(char *argv[]){
  process_t* p = malloc(sizeof(process_t));
  p->failure = NO_FAILURE_CODE;

  // Make pipes
  int file_pipes_i[2];
  int file_pipes_o[2];
  int file_pipes_e[2];
  if(pipe(file_pipes_i) != 0){
    p->failure = PIPE_FAILURE_CODE; return p;
  }
  if(pipe(file_pipes_o) != 0){
    p->failure = PIPE_FAILURE_CODE; return p;
  }
  if(pipe(file_pipes_e) != 0){
    p->failure = PIPE_FAILURE_CODE; return p;
  }

  //Make process
  int fork_result = fork();
  if(fork_result == -1){
    p->failure = FORK_FAILURE_CODE; return p;
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
      p->failure = EXEC_FAILURE_CODE; return p;
    }
    p->failure = EXIT_FAILURE_CODE; return p;
  }

  //Parent process creates streams 
  close(file_pipes_i[0]);
  close(file_pipes_o[1]);
  close(file_pipes_e[1]);

  p->in  = fdopen(file_pipes_i[1], "w");
  p->out = fdopen(file_pipes_o[0], "r");
  p->err = fdopen(file_pipes_e[0], "r");

  return p;
}
