#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>


#include "threads.h"
#include "io.h"
#include "pa1.h"
#include "structures.h"
#include "pipes.h"

int receive_from_children(int number_of_threads, MessageType type) {
  local_id i = 1;
  while (i < number_of_threads) {

    Message msg;
    struct Info info = {.id = 0, .n = number_of_threads};
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            for(int k = 0; k < 2; k++) {
                info.pipe_matrix[i][j][k] = pipe_matrix[i][j][k];
            }
        }
    }

    if (receive(&info, i, &msg) != 0) {
      fprintf(stdout, "Unable to receive message\n");
      return -1;
    }

    fprintf(stdout, "Main process received: %d\n", msg.s_header.s_type);
    fprintf(stdout, "Main process received: %s\n", msg.s_payload);
    if (msg.s_header.s_type == type) {
      i++;
    }
  }
  return 0;
}


int receive(void *__info, local_id from, Message *msg) {
  struct Info *info = (struct Info *)__info;
  int fd = info->pipe_matrix[from][info->id][0];
  
  if (read(fd, &msg->s_header, sizeof(MessageHeader)) == -1) {
    return -1;
  }
  if (read(fd, &msg->s_payload, msg->s_header.s_payload_len) == -1) {
    return -1;
  }
  return 0;
}

int recieve_from_others(local_id id, int n, MessageType msg_type) {
  local_id i = 1;
  while (i < n) {
    if (i == id) {
      i++;
    } else {
      Message msg;
      struct Info info = {.id = id, .n = n};
    
      for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            for(int k = 0; k < 2; k++) {
                info.pipe_matrix[i][j][k] = pipe_matrix[i][j][k];
            }
        }
      }

      if (receive(&info, i, &msg) != 0) {
        return -1;
      }

      if (msg.s_header.s_type == msg_type) {
        i++;
      }
    }
  }
  return 0;
}

void send_to_pipe(int fd, const Message *msg, size_t msg_size) {
  write(fd, msg, msg_size);
}

int send_multicast(void *__info, const Message *msg) {
  struct Info *info = (struct Info *)__info;

  for (int i = 0; i < info->n; i++) {
    if (i != info->id) {
      send_to_pipe(info->pipe_matrix[info->id][i][1], msg, get_msg_size(msg));
    }

  }
  return 0;
}

void prepare_message_and_send(local_id id, int number_of_threads, MessageType msg_type, const char * const message_tmp) {
  char str[MAX_PAYLOAD_LEN + 1];
  memset(str, '\0', sizeof(char)*(MAX_PAYLOAD_LEN));
  sprintf(str, message_tmp, id, getpid(), getppid());

  Message started_msg = create_message(msg_type, str);
  struct Info info = {.id = id, .n = number_of_threads};

  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 10; j++) {
        for(int k = 0; k < 2; k++) {
            info.pipe_matrix[i][j][k] = pipe_matrix[i][j][k];
        }
    }
  }

  write_to_event_file_and_stdout(started_msg.s_payload);
  send_multicast(&info, &started_msg);
}

int close_useless_pipes(local_id id, int n) {
  for (local_id i = 0; i < n; i++) {
    for (local_id j = 0; j < n; j++) {
      if (i == j) continue;

      if (i != id) {
        if (pipe_matrix[i][j][1] != -1) {
          fprintf(stdout, "Close pipe to write: %d\n %d %d\n", pipe_matrix[i][j][1], i, j);
          close(pipe_matrix[i][j][1]);
          pipe_matrix[i][j][1] = -1;
        }
      }
      
      if (j != id) {
        if (pipe_matrix[i][j][0] != -1) {
          fprintf(stdout, "Close pipe to read: %d\n %d %d\n", pipe_matrix[i][j][0], i, j);
          close(pipe_matrix[i][j][0]);
          pipe_matrix[i][j][0] = -1;
        }
      }
    }
  }
  return 0;
}


void log_received_message(local_id id, const char * const template) {
    char buf[MAX_PAYLOAD_LEN];
    memset(buf, '\0', sizeof(char)*MAX_PAYLOAD_LEN);
    sprintf(buf, template, id);
    write_to_event_file_and_stdout(buf);
}

int start_child(local_id id, int number_of_threads) {
  close_useless_pipes(id, number_of_threads);

  prepare_message_and_send(id, number_of_threads, STARTED, log_started_fmt);
  recieve_from_others(id, number_of_threads, STARTED);
  log_received_message(id, log_received_all_started_fmt);

  prepare_message_and_send(id, number_of_threads, DONE, log_done_fmt);
  recieve_from_others(id, number_of_threads, DONE);
  log_received_message(id, log_received_all_done_fmt);

  close_all_pipes(number_of_threads);
  close_files();
  
  return 0;
}

void wait_for_children(int number_of_threads) {
  for (local_id i = 1; i < number_of_threads; i++) wait(NULL);
}

int start_parent(int number_of_theads) {
  close_useless_pipes(0, number_of_theads);
  
  if (receive_from_children(number_of_theads, STARTED)) {
      fprintf(stdout, "Error while receiving STARTED\n");
  }
  
  if (receive_from_children(number_of_theads, DONE)) {
      fprintf(stdout, "Error while receiving DONE\n");
  }
  
  wait_for_children(number_of_theads);
  
  close_all_pipes(number_of_theads);
  close_files();
  
  return 0;
}

int start_multithreads(int number_of_threads) {
    for (local_id id = 1; id < number_of_threads; id++) {
        int result = fork();
        if (result == 0) {
            start_child(id, number_of_threads);
            return 0;
        }
    }

    start_parent(number_of_threads);
    return 0;
}
