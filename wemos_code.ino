#define BLYNK_TEMPLATE_ID "TMPL5AwnCw7P"
#define BLYNK_DEVICE_NAME "Monitoring JCW"
#define BLYNK_AUTH_TOKEN "lHToeQ5bpu_XX47km5RUugC3dkJYk6Ee"
#define BLYNK_PRINT Serial
// import library
#include <PZEM004Tv30.h>
#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
// define pins
#define RX1 D2
#define TX1 D1
#define RX2 D4
#define TX2 D3
#define RX3 D6
#define TX3 D5
#define BUZZER D7


// crate class
PZEM004Tv30 pzem1(TX1, RX1);
PZEM004Tv30 pzem2(TX2, RX2);
PZEM004Tv30 pzem3(TX3, RX3);
BlynkTimer timer;

//cretae variable
double    volt1, volt2, volt3;
double    amp1, amp2, amp3;
double    frequency;
double    pf;


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Komblitanol";//"AULA_PIELSTICK";
char pass[] = "12345678";//"pltdtrisakti";
uint16_t n = 0;
uint8_t z = 0;





void buzzerCheck(){
  if(z == 1){
    alarmOn();
  }
  else{
    noTone(BUZZER);
  }
}
void alarmOn(){
  
  for(int i = 0; i < 10; i++){
  tone(BUZZER, 200); // Send 200 Hz sound signal...
  delay(400);        // ...for 0.4 sec
  noTone(BUZZER);     // Stop sound...
  delay(400);        
          }
}

void cekGangguan(){
  if(amp1 > 59 || amp2 > 59 || amp3 > 59){
    z = 1;
    Blynk.virtualWrite(V11, HIGH);
    if(n <= 2){
      Blynk.logEvent("OverCurrent");
      n++;
    }
  }
  else{
    z = 0;
    Blynk.virtualWrite(V11, LOW);
    n = 0;
  }

  if(amp1 == 0 && amp2 == 0 && amp3 == 0){
    z = 0;
    Blynk.virtualWrite(V11, LOW);
    n = 0;
  }
  else if(amp1 == 0){
    z = 1;
    Blynk.virtualWrite(V12, HIGH);
    if(n <= 2){
      Blynk.logEvent("r_unbalanced");
      n++;
    }
  }

  else if(amp2 == 0){
    z = 1;
    Blynk.virtualWrite(V12, HIGH);
    if(n <= 2){
      Blynk.logEvent("s_unbalanced");
      n++;
    }
  }

  else if(amp3 == 0){
    z = 1;
    Blynk.virtualWrite(V12, HIGH);
    if(n <= 2){
      Blynk.logEvent("t_unbalanced");
      n++;
    }
  }
  else{
    z = 0;
    Blynk.virtualWrite(V12, LOW);
    n = 0;
  }

  if(volt1 == 0 && volt2 == 0 && volt3 == 0){
    z = 0;
    Blynk.virtualWrite(V12, LOW);
    n = 0;
  }
  else if(volt1 == 0){
    z = 1;
    Blynk.virtualWrite(V12, HIGH);
    if(n <= 5){
      Blynk.logEvent("r_unbalanced");
      n++;
    }
  }

  else if(volt2 == 0){
    z = 1;
    Blynk.virtualWrite(V12, HIGH);
    if(n <= 2){
      Blynk.logEvent("s_unbalanced");
      n++;
    }
  }

  else if(volt3 == 0){
    z = 1;
    Blynk.virtualWrite(V12, HIGH);
    if(n <= 2){
      Blynk.logEvent("t_unbalanced");
      n++;
    }
  }
  else{
    z = 0;
    Blynk.virtualWrite(V12, LOW);
    n = 0;
  }  
  
}

void sendSensor(){
  //if(!isnan(volt1) && !isnan(amp1) && !isnan(volt2) && !isnan(amp2) && !isnan(volt3) && !isnan(amp3) && !isnan(frequency) && !isnan(pf)){
    
  Blynk.virtualWrite(V0, volt1);
  Blynk.virtualWrite(V1, volt2);
  Blynk.virtualWrite(V2, volt3);
  Blynk.virtualWrite(V3, amp1);
  Blynk.virtualWrite(V4, amp2);
  Blynk.virtualWrite(V5, amp3);
  Blynk.virtualWrite(V6, pf);
  Blynk.virtualWrite(V7, frequency);
  
  //}
  
}


void readSensor(){
  
  //fase pertama
  double v1     = pzem1.voltage();
  if(!isnan(v1)){
    volt1 = v1;
  }
  else{
    volt1 = 0;
  }
  double i1      = pzem1.current();
  if(!isnan(i1)){
    amp1 = i1;
  }
  else{
    amp1 = 0;
  }
  double f = pzem1.frequency();
  if(!isnan(f)){
    frequency = f;
  }
  else{
    frequency = 0;
  }
  double pf1;
  double tempPF1 = pzem1.pf();
  if(!isnan(tempPF1)){
    pf1 = tempPF1;
  }
  else{
    pf1 = 0;
  }
  double pf2;
  double tempPF2 = pzem2.pf();
  if(!isnan(tempPF2)){
    pf2 = tempPF2;
  }
  else{
    pf2 = 0;
  }
  double pf3;
  double tempPF3 = pzem3.pf();
  if(!isnan(tempPF3)){
    pf3 = tempPF3;
  }
  else{
    pf3 = 0;
  }
  pf = pf1;
  if(pf < pf2){
    pf = pf2;
  }
  if(pf < pf3){
    pf = pf3;
  }

  //fase kedua
  double v2     = pzem2.voltage();
  if(!isnan(v2)){
    volt2 = v2;
  }
  else{
    volt2 = 0;
  }
  double i2      = pzem2.current();
  if(!isnan(i2)){
    amp2 = i2;
  }
  else{
    amp2 = 0;
  }


  //fase ketiga
  double v3     = pzem3.voltage();
  if(!isnan(v3)){
    volt3 = v3;
  }
  else{
    volt3 = 0;
  }
  double i3      = pzem3.current();
  if(!isnan(i3)){
    amp3 = i3;
  }
  else{
    amp3 = 0;
  }
  cekGangguan();
}


void setup() {
  Serial.begin(9600);
  while(!Serial){
    ;
  }
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); //IP dan port
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  //Serial.println("Atas readSensor");
  readSensor();
  //Serial.println("Bawah readSensor");
  Blynk.run();
  timer.run();
  buzzerCheck();
  
  Serial.print(volt1);
  Serial.print("\n");
  Serial.print(amp1);
  Serial.print("\n");
  Serial.print(volt2);
  Serial.print("\n");
  Serial.print(amp2);
  Serial.print("\n");
  Serial.print(volt3);
  Serial.print("\n");
  Serial.print(amp3);
  Serial.print("\n");
  Serial.print(frequency);
  Serial.print("\n");
  Serial.print(pf);
  Serial.print("\n");
  Serial.print(z);
  Serial.print("\n");
  
}