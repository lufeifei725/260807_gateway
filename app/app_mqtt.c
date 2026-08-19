#include "app_mqtt.h"
#include <MQTTClient.h>
#include "log/log.h"
#include <string.h>

static MQTTClient_message pubmsg = MQTTClient_message_initializer;

static MQTTClient client;
static MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
static int (*recv_callback)(char *json) = NULL;


static void delivered(void *context, MQTTClient_deliveryToken dt)
{
    log_debug("Message delivery successful");
}

static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int result = 0;
    if(recv_callback)
    {
        result = recv_callback((char *)message->payload) == 0 ? 1 : 0;
    }
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return result;
}


static void connlost(void *context, char *cause)
{
    log_error("Connection lost, cause: %s", cause);
}


int app_mqtt_init()
{
    if (MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL) != MQTTCLIENT_SUCCESS)
    {
        log_error("Failed to create client, return code ");
        return -1;
    }
    if(MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered) != MQTTCLIENT_SUCCESS)
    {
        log_error("Failed to set callbacks, ret urn code");
        MQTTClient_destroy(&client);
        return -1;
    }
    if(MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS)
    {
        log_error("Failed to connect, return code");
        MQTTClient_destroy(&client);
        return -1;
    }
    if(MQTTClient_subscribe(client, TOPIC_R2G, QOS) != MQTTCLIENT_SUCCESS)
    {
        log_error("Failed to subscribe, return code");
        app_mqtt_close();
        return -1;
    }
    log_debug("MQTT init success");
    return 0;

}


void app_mqtt_close()
{
    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);
}

int app_mqtt_send(char *json)
{
    pubmsg.payload = json;
    pubmsg.payloadlen = strlen(json);
    pubmsg.qos = QOS;
    if(MQTTClient_publishMessage(client, TOPIC_G2R, &pubmsg, NULL) != MQTTCLIENT_SUCCESS)
    {
        log_error("Failed to publish message");
        return -1;
    }
    log_debug("Message published: %s", json);   
    return 0;
}
void app_mqtt_registerReceiveCallback(int(*callback)(char *json))
{
    recv_callback = callback;

}