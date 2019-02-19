
void setup() {
pinMode(A0,INPUT);
pinMode(5,OUTPUT);
pinMode(6,OUTPUT);
Serial.begin(115200);
}
void loop() {
int x = analogRead(A0);
Serial.println(x);
if (x>300)




{
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  Serial.println("Alert...");
}
 else
 {
  digitalWrite(6,LOW);
  digitalWrite(5,HIGH);
 }
}
