#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include <signal.h>
#include <json-c/json.h>

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "LHPClient"
#define TOPIC       "LHP_TEST"
#define QOS         1

volatile MQTTClient_deliveryToken deliveredToken;
MQTTClient_deliveryToken token;
MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

void deliveryComplete(void *context, MQTTClient_deliveryToken dt) {
    printf("Message delivered\n");
    deliveredToken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char *lock = "lock";
    char *unlock = "unlock";
    char *dispensing = "dispensing";
    char *device_info = "device_info";

    const char* cmd = "";

    json_object* json_obj = json_tokener_parse((char*)message->payload);
    if (json_obj == NULL) {
        printf("JSON parsing error\n");
        printf("RAW Message content: %s\n", (char*)message->payload);
        if(strcmp(lock, (char*)message->payload)==0){   
            printf("Request lock!\n");
            system("/bin/request_lock");
        }
        else if(strcmp(unlock, (char*)message->payload)==0){
            printf("Request unlock!\n");
            system("/bin/request_unlock");
        }
        else if(strcmp(dispensing, (char*)message->payload)==0){
            printf("Request dispensing!\n");
            system("/sdcard/request_dispensing");
        }
        else if(strcmp(device_info, cmd)==0){ 
            printf("Request device info!\n");
            system("/bin/request_deviceInfo");
        }
        else{
            printf("..?\n");
        }  
    }
    else {
        printf("Message arrived on topic: %s\n", topicName);
        printf("JSON Message content: %.*s\n", message->payloadlen, (char*)message->payload);
    }

    json_object* cmd_obj;
    if (json_object_object_get_ex(json_obj, "cmd", &cmd_obj)) {
        cmd = json_object_get_string(cmd_obj);
        printf("Message cmd: %s\n", cmd);
    }
    
    json_object* root = json_object_new_object();

    if(strcmp(lock, cmd)==0){    
        printf("Request lock!\n");
        system("/bin/request_lock");

        json_object_object_add(root, "comm_type", json_object_new_string("mqtt"));
        json_object_object_add(root, "direction", json_object_new_string("resp"));
        json_object_object_add(root, "result", json_object_new_string("success"));

        const char* jsonString = json_object_to_json_string(root);

        MQTTClient_message pubmsg = MQTTClient_message_initializer;

        pubmsg.payload = (void*)jsonString; //response;
        pubmsg.payloadlen = (int)strlen(jsonString);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);   
    }
    else if(strcmp(unlock, cmd)==0){ 
        printf("Request unlock!\n");
        system("/bin/request_unlock");

        json_object_object_add(root, "comm_type", json_object_new_string("mqtt"));
        json_object_object_add(root, "direction", json_object_new_string("resp"));
        json_object_object_add(root, "result", json_object_new_string("success"));
        
        const char* jsonString = json_object_to_json_string(root);

        MQTTClient_message pubmsg = MQTTClient_message_initializer;

        pubmsg.payload = (void*)jsonString;
        pubmsg.payloadlen = (int)strlen(jsonString);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    }
    else if(strcmp(dispensing, cmd)==0){ 
        printf("Request dispensing!\n");
        system("/sdcard/request_dispensing");

        json_object_object_add(root, "comm_type", json_object_new_string("mqtt"));
        json_object_object_add(root, "direction", json_object_new_string("resp"));
        json_object_object_add(root, "result", json_object_new_string("success"));

        const char* jsonString = json_object_to_json_string(root);

        MQTTClient_message pubmsg = MQTTClient_message_initializer;

        pubmsg.payload = (void*)jsonString;
        pubmsg.payloadlen = (int)strlen(jsonString);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    }
    else if(strcmp(device_info, cmd)==0){ 
        printf("Request device info!\n");
        system("/bin/request_deviceInfo");

        json_object_object_add(root, "comm_type", json_object_new_string("mqtt"));
        json_object_object_add(root, "direction", json_object_new_string("resp"));
        json_object_object_add(root, "id", json_object_new_string("LA1234567"));
        json_object_object_add(root, "device_status", json_object_new_string("ready"));
        json_object_object_add(root, "fw_ver", json_object_new_string("01.01"));
        json_object_object_add(root, "model_name", json_object_new_string("LHP001"));
        json_object_object_add(root, "error_code", json_object_new_string("0"));
        json_object_object_add(root, "module_no", json_object_new_string("1"));
        json_object_object_add(root, "result", json_object_new_string("success"));

        const char* jsonString = json_object_to_json_string(root);

        MQTTClient_message pubmsg = MQTTClient_message_initializer;

        pubmsg.payload = (void*)jsonString;
        pubmsg.payloadlen = (int)strlen(jsonString);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    }
    else{
        printf("...?\n");
    }

    MQTTClient_waitForCompletion((MQTTClient)context, token, 10000);
    json_object_put(root);
    json_object_put(json_obj);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    
    return 1;
}

void connlost(void *context, char *cause) {
    printf("Connection lost: %s\n", cause);
}

void handle_sigint(int signum) {
    printf("프로그램을 종료합니다.\n");
    exit(0);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, handle_sigint);

    int rc;
    char ipaddr[256];

    if(atoi(argv[1]) == 3){
        sprintf(ipaddr, "tcp://192.168.1.58:1883");
    }
    else{
        sprintf(ipaddr, "tcp://%s:1883", argv[1]);
    }
    printf("%s\n", ipaddr);

    MQTTClient_create(&client, ipaddr, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, deliveryComplete);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return 0;
        //exit(EXIT_FAILURE);
    }

    MQTTClient_subscribe(client, TOPIC, QOS);
    printf("Subscribed to topic: %s\n", TOPIC);

    while (1) {
    
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}


