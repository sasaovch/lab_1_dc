#pragma once 
#ifndef __IO_H
#define __IO_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

extern const char *log_format;

extern FILE *events_file;
extern FILE *pipes_file;

int parse_argc(int argc, char *const argv[]);
void write_to_event_file_and_stdout(const char *s);

void open_files(void);
void close_files(void);

#endif
