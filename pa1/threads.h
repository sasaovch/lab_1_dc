#pragma once
#ifndef __THREADS_H
#define __THREADS_H

#include "ipc.h"

int start_multithreads(int number_of_threads);
int start_child(local_id id, int number_of_threads);
int start_parent(int number_of_theads);

void wait_for_children(int number_of_threads);
int recieve_from_others(local_id id, int n, MessageType msg_type);

int receive(void *__info, local_id from, Message *msg);
int receive_from_children(int number_of_threads, MessageType type);

void prepare_message_and_send(local_id id, int number_of_threads, MessageType msg_type, const char * const message_tmp);

int send_multicast(void *__info, const Message *msg);
void send_to_pipe(int fd, const Message *msg, size_t msg_size);

#endif
