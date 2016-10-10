#define FASTADC 1

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


int data = 512;
int storage[512];

long batchStarted;
long batchEnded;
int reading;
int count;
int maximum;
int minimum;
bool toSend;

const int detectLedPin = 7;

void setup() {
#if FASTADC
 // set prescale to 16
 sbi(ADCSRA,ADPS2) ;
 cbi(ADCSRA,ADPS1) ;
 cbi(ADCSRA,ADPS0) ;
#endif
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A4,INPUT);
  pinMode(detectLedPin, OUTPUT); //added to control LED when lightning is detected
  pinMode(13, OUTPUT); //so we can turn off the onboard LED
  digitalWrite(13, LOW);
  digitalWrite(detectLedPin, LOW); //set initial state of detection LED
  Serial.println(micros());
  
  batchStarted=0;
  batchEnded=0;
  reading=0;
  count=0;
  maximum=0;
  minimum=1023;
  toSend=false;
}


void loop() {
  // put your main code here, to run repeatedly:
  reading = (analogRead(A4));
  
  storage[count]=reading;
  if ((!toSend)&&(count!=0)&&((reading>storage[count-1]+10)||(reading<storage[count-1]-10))){
      toSend=true;
  }
  
  count=count+1;
  if ((count == 512) && (toSend))
  {
    count=0;
    batchEnded = millis();
    sendData();
    batchStarted = millis();
    
  }
  else if (count==512){
    count=0;
    batchEnded = millis();
    //sendData();
    batchStarted = millis();
  
  }

 
}

void sendData()
{
  Serial.print(">>>");
  Serial.println(batchStarted);

  digitalWrite(detectLedPin, HIGH); //turns on LED while sending data
  
  for (int i=0;i<data;i++){
    Serial.println(storage[i]);
  }
  Serial.print("<<<");
  Serial.println(batchEnded);
  Serial.println("END");
  
  toSend=false;
  digitalWrite(detectLedPin, LOW); //turns off LED after data is sent
}
