#pragma once
#ifndef  __STRUCTURES_H
#define  __STRUCTURES_H

#include "ipc.h"

struct Info {
  int pipe_matrix[10][10][2];
  local_id id;
  int n;
};

Message create_message(MessageType msg_type, char *payload);
size_t get_msg_size(const Message *msg);

#endif
