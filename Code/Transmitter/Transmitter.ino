#include "DHT.h"
#include "LoRaWan_APP.h"
#include "Arduino.h"
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif
#define RF_FREQUENCY                                915000000 // Hz
#define TX_OUTPUT_POWER                             14        // dBm
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false
#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 60 // Define the payload size here
#define null                                        0
void Temp_Fun();
char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];
static RadioEvents_t RadioEvents;
double txNumber;
int16_t rssi,rxSize;
float h,t,f;
float  DoubleToString( char *str, double double_num,unsigned int len);
DHT dht(DHTPIN, DHTTYPE);
void setup() {
Serial.begin(115200);
    txNumber=0;
    rssi=0;
    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 ); 
   dht.begin();
   }
void Temp_Fun() 
{
 delay(2000);
    h = dht.readHumidity();
    t = dht.readTemperature();
    f = dht.readTemperature(true);
   if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
   }
 
void loop()
{
  float x=0,y=0,z=0;
  delay(1000);
Temp_Fun();
  sprintf(txpacket,"%s","Current Humidity %: ");  //start a package
  sprintf(txpacket+strlen(txpacket),"%f",h); //add to the end of package  
  DoubleToString(txpacket,h,2);    //add to the end of package 
  turnOnRGB(COLOR_SEND,0); //change rgb color
  Serial.printf("\r\n \"%s\" , %f\r\n" ,txpacket,h);
  Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out 
delay(800);
  sprintf(txpacket,"%s","Temperature °C :");  //start a package
  sprintf(txpacket+strlen(txpacket),"%f",t); //add to the end of package
  DoubleToString(txpacket,t,2);    //add to the end of package
  turnOnRGB(COLOR_SEND,0); //change rgb color
  Serial.printf("\r\n\"%s\", %f\r\n",txpacket,t);
  Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out
delay(800);
  sprintf(txpacket,"%s","Temperature °F :");  //start a package
  sprintf(txpacket+strlen(txpacket),"%f",f); //add to the end of package
  DoubleToString(txpacket,f,2);    //add to the end of package
  turnOnRGB(COLOR_SEND,0); //change rgb color
  Serial.printf("\r\n\"%s\", %f\r\n",txpacket,f);
  Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out
delay(800); 
}
float  DoubleToString( char *str, double double_num,unsigned int len) { 
  double fractpart, intpart;
  fractpart = modf(double_num, &intpart);
  fractpart = fractpart * (pow(10,len));
  sprintf(str + strlen(str),"%d", (int)(intpart)); //Integer part
  sprintf(str + strlen(str), ".%d", (int)(fractpart)); //Decimal part
}
