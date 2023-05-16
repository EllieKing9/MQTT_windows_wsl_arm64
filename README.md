# MQTT

<h6>
MQTT의 구현체로는 Mosquitto, HiveMQ, EMQ X 등 다양한 오픈 소스 브로커가 있습니다. 클라이언트 구현체로는 Paho MQTT, Eclipse MQTT 등이 있으며, 다양한 언어로 구현

(WSL) Ubuntu애서 Eclipse mosquitto를 사용하는 경우(broker 실행과 프로그래밍)
```
  $sudo apt-get update
  $sudo apt-get install libmosquitto-dev
  $gcc -Wall mqttc.c -o mq_amd -lmosquitto //컴파일
  
  $mosquitto
[  141.860311]~DLT~   71~INFO     ~FIFO /tmp/dlt cannot be opened. Retrying later...
1684213873: mosquitto version 1.6.9 starting
1684213873: Using default config.
1684213873: Opening ipv4 listen socket on port 1883.
1684213873: Opening ipv6 listen socket on port 1883.
  
  IP주소 설정을 변경하는 경우
  $cd /etc/mosquitto/mosquitto.conf
  $sudo nano mosquitto.conf 
  bind address <IP adress> //내용 추가후 alt+x, y, enter
  $mosquitto //실행
  
  $sudo apt-get install libmosquitto-dev:arm64 //크로스 컴파일 대응
```

mosquitto 에서 publishing 하는 방법
```
  $sudo apt-get install mosquitto-clients
  $mosquitto_pub -h <broker_address> -p <broker_port> -t <topic> -m <message>
```

Windows에서 Eclise mosquitto를 사용하는 경우
(https://mosquitto.org/download/)
```
  기본적으로 아래 폴더에 설치됨
  C:\Program Files\mosquitto\mosquitto.conf
  커맨드 창에서
  >mosquiito -v //서비스 실행
  
  IP주소를 바꾸는 경우
  mosquitto.conf에
  bind_address <IP address> 추가
  >mosquitto -c mosquitto.conf -v //설정을 적용하여 실행
```
  
(WSL) Ubuntu애서 Python에서 만든 paho mqtt를 사용하는 경우 (프로그래밍, 실행은 mosquitto로)
```
  ubuntu 22.04부터 해당 패키지 지원
  $sudo apt-get install libpaho-mqtt-dev
  $gcc mqtt-subscriber.c -o ms -lpaho-mqtt3c
  
  arm64용의 경우 sources.list에 주소 추가
  $sudo nano /etc/apt/sources.list
deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports jammy main multiverse universe
deb [arch=arm64] http://ports.ubuntu.com/ jammy-security main multiverse universe
deb [arch=arm64] http://ports.ubuntu.com/ jammy-backports main multiverse universe
deb [arch=arm64] http://ports.ubuntu.com/ jammy-updates main multiverse universe
  위의 내용 추가 후, alt+x, y, enter
  $sudo apt-get update
  $sudo apt-get install libpaho-mqtt-dev:arm64
```  
  
