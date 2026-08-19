#include "app_message.h"
#include "cJSON/cJSON.h"
#include "log/log.h"
#include <string.h>
#include <stdlib.h>
#include "app_common.h"

char *app_message_chars2Json(const char *chars, size_t chars_len)
{
    int conn_type = chars[0];
    int id_len = chars[1];
    int msg_len = chars[2];
    if (chars_len != 3 + id_len + msg_len) {
        log_error("chars is not the correct length");
        return NULL;    
    }
    char id[id_len];
    memcpy(id, chars + 3, id_len);
    char msg[msg_len];
    memcpy(msg, chars + 3 + id_len, msg_len);

    char *id_hexstr = app_common_chars2Hexstr(id, id_len);
    char *msg_hexstr = app_common_chars2Hexstr(msg, msg_len);

    cJSON *root_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(root_obj, "conn_type", conn_type);
    cJSON_AddStringToObject(root_obj, "id", id_hexstr);
    cJSON_AddStringToObject(root_obj, "msg", msg_hexstr);

    char *json = cJSON_PrintUnformatted(root_obj);
    cJSON_Delete(root_obj);
    free(id_hexstr);
    free(msg_hexstr);

    return json;

}
int app_message_json2Chars(const char *json, char *chars_buf, int buf_size)
{
    cJSON *root_obj = cJSON_Parse(json);
    if (root_obj == NULL) {
        log_error("json parse error");
        return -1;
    }
    int conn_type = cJSON_GetObjectItem(root_obj, "conn_type")->valueint;
    char *id_hexstr = cJSON_GetObjectItem(root_obj, "id")->valuestring;
    char *msg_hexstr = cJSON_GetObjectItem(root_obj, "msg")->valuestring;
    int id_len = -1;
    int msg_len = -1;
    char *id = app_common_hexstr2Chars(id_hexstr, &id_len);
    char *msg = app_common_hexstr2Chars(msg_hexstr, &msg_len);
    if (buf_size < 3 + id_len + msg_len) {
        log_error("chars_buf is too small");
        cJSON_Delete(root_obj);
        free(id);
        free(msg);
        return -1;
    }
    chars_buf[0] = conn_type;
    chars_buf[1] = id_len;
    chars_buf[2] = msg_len;
    memcpy(chars_buf + 3, id, id_len);
    memcpy(chars_buf + 3 + id_len, msg, msg_len);
    cJSON_Delete(root_obj);
    free(id);
    free(msg);
    return 3 + id_len + msg_len;
}

    

  