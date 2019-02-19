//#include 

int inputPin = A0;
//SoftwareSerial ble(6,7); //bluetooth comm

void setup() {
  // initialize serial communication with computer:
  Serial.begin(115200);
  pinMode(A0,INPUT);
  pinMode(A1,OUTPUT);
  //ble.begin(9600);
}

void loop() {
  //read ref voltage
  float ref_volt = float(readVcc())/435.0;
  
  //preallocate
  float dbValue;
  char db_str[4],ble_dat[12];
  
 //strcpy(ble_dat,"s"); //tell the iPhone app to 'start'
  
  // read and convert analog data to dB
    dbValue = (analogRead(inputPin)/1024.0)*ref_volt*50.0;    
    dtostrf(dbValue,1,2,db_str);
    Serial.println(db_str);
    analogWrite(A1,88);
    //strcat(ble_dat,db_str);
    //ble.write(ble_dat);
    //delay(80);
   // ble.write("e");
  //delay(125);
}

// read voltage to ensure ADC converts properly
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
