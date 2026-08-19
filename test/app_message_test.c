#include "app_message.h"
#include "log/log.h"
#include "stdlib.h"

int main(int argc, char *argv[])
{
    char *json_message = "{\"conn_type\":1,\"id\":\"5858\",\"msg\":\"61626364\"}";
    char chars_buf[100];
    int len = app_message_json2Chars(json_message, chars_buf, sizeof(chars_buf));
    log_debug("字符数组信息: %.*s, 长度: %d", len,chars_buf, len);
    char *json_message2 = app_message_chars2Json(chars_buf, len);
    log_debug("转换后的JSON信息: %s", json_message2);
    free(json_message2);

}