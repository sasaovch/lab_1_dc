#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "io.h"
#include "pipes.h"
#include "threads.h"

int main(int argc, char *const argv[]) {
    int number_of_threads = parse_argc(argc, argv);

    open_files();
    open_pipes(number_of_threads + 1);

    start_multithreads(number_of_threads + 1);

    return 0;
}
