#include "pipes.h"
#include "ipc.h"
#include "io.h"

int pipe_matrix[11][11][2];

int open_pipes(int number_of_threads) {
    for (local_id i = 0; i < number_of_threads; i++) {
        for (local_id j = 0; j < number_of_threads; j++) {
            if (i != j) {
                int file_descriptors[2];
                if (pipe(file_descriptors) != 0) {
                    fprintf(stdout, "Error while opening pipe");
                    exit(1);
                }

                pipe_matrix[i][j][0] = file_descriptors[0]; // read
                pipe_matrix[i][j][1] = file_descriptors[1]; // write

                fprintf(stdout, log_format, i, j, file_descriptors[0], file_descriptors[1]);
                fflush(stdout);
                
                fprintf(pipes_file, log_format, i, j, file_descriptors[0], file_descriptors[1]);
                fflush(pipes_file);
            }
        }
    }
    return 0;
}

int close_all_pipes(int number_of_threads) {
  for (local_id i = 0; i < number_of_threads; i++) {
    for (local_id j = 0; j < number_of_threads; j++) {
      if (i != j) {
        close(pipe_matrix[i][j][0]);
        close(pipe_matrix[i][j][1]);
      }
    }
  }
  return 0;
}
