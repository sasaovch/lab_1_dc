#pragma once
#ifndef  __PIPES_H
#define  __PIPES_H

#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "ipc.h"

extern int pipe_matrix[11][11][2];

int open_pipes(int number_of_threads);
int close_all_pipes(int number_of_threads);
int close_useless_pipes(local_id id, int n);

#endif
