# MQTT

<h6>
MQTT의 구현체로는 Mosquitto, HiveMQ, EMQ X 등 다양한 오픈 소스 브로커가 있습니다. 클라이언트 구현체로는 Paho MQTT, Eclipse MQTT 등이 있으며, 다양한 언어로 구현
</h6>

(WSL) Ubuntu애서 Eclipse mosquitto를 사용하는 경우(broker 실행 및 ..)
  
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
  $mosquitto -d -c /etc/mosquitto/mosquitto.conf -v //실행

  추가 설정(localhost가 아닌 경우 인증이 기본적으로 활성화 됨 -> allow_anonymous 를 설정하여 테스트)
  $sudo nano mosquitto.conf 
  allow_anonymous true //내용 추가후 alt+x, y, enter
  $mosquitto -d -c /etc/mosquitto/mosquitto.conf -v //실행
  
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
  
  IP주소와 접속권한를 변경하는 경우
  mosquitto.conf에
  bind_address <IP address> 추가 //localhost가 디폴트
  allow_anonymous true 추가 //false가 디폴트, localhost에서는 true
  >mosquitto -c mosquitto.conf -v //설정을 적용하여 실행
```
  
(WSL) Ubuntu에서 pyton의 paho mqtt client를 사용하는 경우 (프로그래밍, 실행은 mosquitto로)
```
  ubuntu 22.04부터 해당 패키지 지원
  $sudo apt-get install libpaho-mqtt-dev
  $gcc mqtt-subscriber.c -o ms -lpaho-mqtt3c
  
  $sudo dpkg --add-architecture arm64 //추가
  
  $sudo dpkg --remove-architecture arm64 //삭제

  $sudo dpkg --print-foreign-architectures //arm64 아키텍쳐 추가여부 확인
    
  sources.list에 주소 추가
  $sudo nano /etc/apt/sources.list
deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports jammy main multiverse universe
deb [arch=arm64] http://ports.ubuntu.com/ jammy-security main multiverse universe
deb [arch=arm64] http://ports.ubuntu.com/ jammy-backports main multiverse universe
deb [arch=arm64] http://ports.ubuntu.com/ jammy-updates main multiverse universe
  위의 내용 추가 후, alt+x, y, enter
  $sudo apt-get update
  $sudo apt-get install libpaho-mqtt-dev:arm64
  
  $aarch64-linux-gnu-gcc -Wall mqtt.c -o mq_arm -lpaho-mqtt3c //컴파일
  $aarch64-linux-gnu-gcc -Wall mqtt.c -o mq_arm2 -lpaho-mqtt3c -static //라이브러리 포함(정적)
```

테스트 프로그램 
http://mqtt-explorer.com/

JSON 사용
```
  $sudo apt-get install libjson-c-dev
  $sudo apt-get install libjson-c-dev:arm64
  $gcc -Wall mqtt.c -o test -lpaho-mqtt3c -ljson-c
  $aarch64-linux-gnu-gcc -Wall mqtt.c -o test -lpaho-mqtt3c -static -ljson-c
```
  
