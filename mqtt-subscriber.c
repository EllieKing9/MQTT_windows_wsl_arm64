#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClient"
#define TOPIC       "test/topic"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    printf("Message arrived on topic: %s\n", topicName);
    printf("Message content: %.*s\n", message->payloadlen, (char*)message->payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause) {
    printf("Connection lost: %s\n", cause);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, deliveredtoken);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    MQTTClient_subscribe(client, TOPIC, QOS);

    printf("Subscribed to topic: %s\n", TOPIC);

    while (1) {
        // 유지하기 위해 무한 루프 유지
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
