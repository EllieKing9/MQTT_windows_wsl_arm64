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
#define MAX_BUFFER_SIZE 5120 //1024

volatile MQTTClient_deliveryToken deliveredToken;
MQTTClient_deliveryToken token;
MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

// char* executeShellCommand(const char* command) {
int executeShellCommand(const char* command, const char* searchWord) {
    FILE* fp;
    char buffer[MAX_BUFFER_SIZE];
    // char* result = NULL;

    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to execute shell command.\n");
        // return NULL;
        return 0;
    }

    // 셸 명령의 출력을 읽어옴
    while (fgets(buffer, MAX_BUFFER_SIZE, fp) != NULL){
        printf("%s", buffer);
        if (strstr(buffer, searchWord) != NULL) {
            printf("Found: %s", buffer);
            // 검색된 단어에 대한 추가 로직을 작성합니다.
        }
        // result = strdup(buffer);
    }
    // if (fgets(buffer, MAX_BUFFER_SIZE, fp) != NULL) {
    // while (fgets(buffer, MAX_BUFFER_SIZE, fp) != NULL) {
        // 결과를 복사하여 반환할 스트링에 저장
        // result = strdup(buffer);
    // }

    pclose(fp);

    // return result;
    return 1;

    // char* output = executeShellCommand("ls -l");
    // if (output != NULL) {
    //     printf("Shell command output: %s\n", output);
    //     free(output);
    // }
}

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

    //printf("%s\n",(char*)message->payload);
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

    // JSON 필드 값 확인
    json_object* cmd_obj;
    if (json_object_object_get_ex(json_obj, "cmd", &cmd_obj)) {
        cmd = json_object_get_string(cmd_obj);
        printf("Message cmd: %s\n", cmd);
    }
    
    json_object* root = json_object_new_object();

    if(strcmp(lock, cmd)==0){    
        printf("Request lock!\n");
        system("/bin/request_lock");
        // execl("/bin/", "request_lock", NULL);

        // 메시지 송신
        // char* response = "Response message";

        // JSON 객체 생성
        json_object_object_add(root, "comm_type", json_object_new_string("mqtt"));
        json_object_object_add(root, "direction", json_object_new_string("resp"));
        json_object_object_add(root, "result", json_object_new_string("success"));

        // JSON 문자열 변환
        const char* jsonString = json_object_to_json_string(root);

        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        // printf("state: %s\n", "pubmsg init");

        pubmsg.payload = (void*)jsonString; //response;
        pubmsg.payloadlen = (int)strlen(jsonString);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        // printf("state: %s\n", "pubmsg setting");

        // MQTTClient_deliveryToken token;
        // MQTTClient_publishMessage((MQTTClient)context, TOPIC, &pubmsg, &token);
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        // printf("Response message published: %s\n", response);
        
    }
    else if(strcmp(unlock, cmd)==0){ 
        printf("Request unlock!\n");
        system("/bin/request_unlock");

        json_object_object_add(root, "comm_type", json_object_new_string("mqtt"));
        json_object_object_add(root, "direction", json_object_new_string("resp"));
        json_object_object_add(root, "result", json_object_new_string("success"));

        // JSON 문자열 변환
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

        // JSON 문자열 변환
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

        // JSON 문자열 변환
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

    // MQTTClient client;
    // MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    char ipaddr[256];

    if(atoi(argv[1]) == 1){
        //execl("/mnt/c/22_04/pt/mqtt/test", "test", NULL);
        //system("/mnt/c/22_04/pt/mqtt/test");
        //printf("test\n");
        //return 0;

        executeShellCommand("ls -l", "test");
        return 0;
    }
    else if(atoi(argv[1]) == 2){
        //system("/bin/request_unlock");
        system("/sdcard/request_dispensing");
    }
    else if(atoi(argv[1]) == 3){
        sprintf(ipaddr, "tcp://192.168.1.58:1883");

        system("wpa_passphrase netis ckbs1234 > /etc/wpas_supplicant.conf");
        sleep(1);
        system("wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf");
        sleep(10);
        system("ifconfig wlan0 192.168.1.58 netmask 255.255.255.0");
        sleep(10);
        system("mosquitto -d -c /etc/mosquitto/mosquitto.conf -v");
        sleep(5);
    }
    else{
        sprintf(ipaddr, "tcp://%s:1883", argv[1]);
    }
    printf("%s\n", ipaddr);

    // MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_create(&client, ipaddr, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, (int)deliveredtoken);
    // MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, NULL);
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, deliveryComplete);

    // while (1){
        if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
            printf("Failed to connect, return code %d\n", rc);
            return 0;
            //exit(EXIT_FAILURE);
        }
    //     else if (rc == MQTTCLIENT_SUCCESS){
    //         break;
    //     }
    // }

    MQTTClient_subscribe(client, TOPIC, QOS);
    printf("Subscribed to topic: %s\n", TOPIC);

    while (1) {
    
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}


// network={
//         ssid="netis"
//         #psk="ckbs1234"
//         psk=a0f5b34d52141abc6dfd9ada24c429ab0cada7b24f8fe32f2712362f8192822a
// }


// #!/bin/sh

// if [ $# -eq 0 ]; then
//     echo missing process names!
//     exit 1
// fi

// while true;do
//         pid=`pidof $*`
//         if [ "$pid" = "" ];then
//                 echo "$* died."
//                 break
//         fi
//         sleep 0.2
// done