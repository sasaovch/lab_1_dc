#include <string.h>

#include "structures.h"

Message create_message(MessageType msg_type, char *payload) {
  Message msg;
  msg.s_header.s_magic = MESSAGE_MAGIC;
  msg.s_header.s_type = msg_type;
  msg.s_header.s_payload_len = strlen(payload);
  
  memset(msg.s_payload, '\0', sizeof(char)*(MAX_PAYLOAD_LEN));
  memcpy(msg.s_payload, payload, strlen(payload));
  return msg;
}

size_t get_msg_size(const Message *msg) {
  return sizeof(MessageHeader) + msg->s_header.s_payload_len;
}
