#if !defined(__APP_MESSAGE_H)
#define __APP_MESSAGE_H
#include <stddef.h>



char *app_message_chars2Json(const char *chars, size_t chars_len);
int app_message_json2Chars(const char *json, char *chars_buf, int buf_size);



#endif // __APP_MESSAGE_H
