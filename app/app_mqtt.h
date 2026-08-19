#if !defined(_APP_MQTT_H)
#define _APP_MQTT_H

#define ADDRESS "ws://10.26.64.103:1883"
#define CLIENTID "b253ba38-daf6-4b37-984f-5d8fdc6a1cfc"
#define TOPIC_R2G "REMOTE_TO_GATEWAY"
#define TOPIC_G2R "GATEWAY_TO_REMOTE"
#define PAYLOAD "Hello World!"
#define QOS 1
#define TIMEOUT 10000L

int app_mqtt_init();
void app_mqtt_close();
int app_mqtt_send(char *json);
void app_mqtt_registerReceiveCallback(int(*callback)(char *json));

#endif // _APP_MQTT_H
