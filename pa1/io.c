#include "common.h"
#include "io.h"

const char *log_format = "Pipe %d -> %d. Fd %d -> %d\n";
FILE *events_file;
FILE *pipes_file;

int parse_argc(int argc, char *const argv[]) {
    if ((getopt(argc, argv, "p:")) != -1) {
        int n = atoi(optarg);
        if (n < 1 || n > 10) {
            fprintf(stdout, "Not enough parameters");
            exit(1);
        }
        return n;
    }
    exit(1);
}

void open_files(void) {
    events_file = fopen(events_log, "a");
    if (events_file == NULL) {
        fprintf(stdout, "Error while opening event file");
        exit(1);
    }
    pipes_file = fopen(pipes_log, "a");
    if (pipes_file == NULL) {
        fprintf(stdout, "Error while opening pipes file");
        exit(1);
    }
}

void close_files(void) {
  fclose(events_file);
  fclose(pipes_file);
}

void write_to_event_file_and_stdout(const char *s) {
  fprintf(events_file, "%s", s);
  fflush(events_file);

  fprintf(stdout, "%s", s);
  fflush(stdout);
}
