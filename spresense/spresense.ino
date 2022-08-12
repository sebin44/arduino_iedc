void setup() {
  // put your setup code here, to run once:
  pinMode(LED0, OUTPUT); 
  pinMode(LED1, OUTPUT); 
  pinMode(LED2, OUTPUT); 
  pinMode(LED3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED0, HIGH);
  delay(1000);
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, HIGH);
  delay(1000);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  delay(1000);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
}
