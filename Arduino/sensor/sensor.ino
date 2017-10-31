#include "DHT.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <SDS011.h>
//#include "printf.h"

//definicion de estados
#define ST_STOP       0
#define ST_RUN        3

//longitud del buffer
#define LEN_CMD     2
#define LEN_DATA    10

//tiempo en mS entre lecturas de los sensores
#define TIME_RD_SENS    1500

//definicion de comandos
#define CMD_START       'a'
#define CMD_RUN         1
#define CMD_ACTIV_RELE  2

//pines del puerto rs232 del sensor SDS011
#define PIN_SDS_RX  3
#define PIN_SDS_TX  2

//pin y tipo de sensor de temperatura y humedad
#define PIN_TEMP    4
#define TMPTYPE DHT11
//pines del acelerometro
#define PIN_CO   A0
#define PIN_O3   A1

//objeto sensor de temperatura y humedad
DHT dht(PIN_TEMP, TMPTYPE);
//objeto transceiver de radio
RF24 radio(9,10);
//objeto SDS011
SDS011 sds;
byte addrs[][6]={{0xe8,0xe8,0xf0,0xf0,0xe1},{0xf0,0xf0,0xf0,0xf0,0xe1}};


struct DatoSensores{
   unsigned long time_on;
   float pm25;
   float pm10;
   float temp;
   float hum;
   unsigned int CO;
   unsigned int O3;
} sensores;

unsigned int state, state_next;
unsigned long time_ini, time_aux; 

char buffer_rx[32];
char buffer_tx[32];

void setup() {
  Serial.begin(115200);
  //iniciacion del sensor de temperatura
  dht.begin();
  //iniciacion del rele
  sds.begin(PIN_SDS_TX, PIN_SDS_RX);
  
  //inicializacion de la radio
  radio.begin();
  //radio.Retries(0,15);                 // Smallest time between retries, max no. of retries
  radio.setChannel(0x76);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  radio.openReadingPipe(1,0xE8E8F0F0E1LL);
//  radio.printDetails();
  radio.enableDynamicPayloads();
  radio.powerUp();
  radio.startListening();
  
  state = ST_RUN;
  Serial.println("Setup end!");
}

void loop() {
  // put your main code here, to run repeatedly:
  char *buff;
  int acelx, acely, acelz;
  int cm;
  
  state_next = state;
  
  switch(state){
    case ST_STOP:
      if(radio.available()){
        Serial.println("ST_STOP Recv msg...");
        buff = buffer_rx;
        while(radio.available())
          radio.read(buff++,1);
        if(buffer_rx[0]==CMD_START){
          Serial.println("ST_STOP go to RUN...");
          time_ini = millis();
          state_next = ST_RUN;
        }
      }
    break;
    case ST_RUN:
      
      //recepcion de un comando
      if(radio.available()){
        Serial.println("Recv msg...");
        buff = buffer_rx;
        while(radio.available())
          radio.read(buff++,2);
        if(buffer_rx[0]==CMD_ACTIV_RELE){
          Serial.print("Rele Activar con ");
          Serial.println(buffer_rx[1],DEC);
        }
      }
      //envio de lecturas de sensores
      int error = sds.read(&sensores.pm25, &sensores.pm10);
      if(!error){
        Serial.print("Send Sensors...");
        sensores.time_on = millis();
        sensores.CO = analogRead(PIN_CO);
        sensores.O3 = analogRead(PIN_O3);
        sensores.temp = dht.readTemperature();
        sensores.hum = dht.readHumidity();
        if(isnan(sensores.temp) || isnan(sensores.hum)){
          Serial.println("(ERROR) failed to read DHT11");
        }
        Serial.print("Tiempo: ");
        Serial.println(sensores.time_on);
        Serial.print("material particulado PM2.5: ");
        Serial.println(sensores.pm25);
        Serial.print("material particulado PM10: ");
        Serial.println(sensores.pm10);
        Serial.print("temperatura: ");
        Serial.println(sensores.temp);
        Serial.print("humedad: ");
        Serial.println(sensores.hum);
        Serial.print("monoxido de carbono: ");
        Serial.println(sensores.CO); 
        Serial.print("Ozono: ");
        Serial.println(sensores.O3);
        //preparando la transmision
        radio.stopListening();
        Serial.print("Txing sens...");
        Serial.println(sizeof(sensores));
        if(!radio.write(&sensores,sizeof(sensores)))
          Serial.println("Error!! Txing Sens");
        //
        radio.startListening();
        time_ini = millis();
      }
      delay(100);    
  }
  state = state_next;
  
}
