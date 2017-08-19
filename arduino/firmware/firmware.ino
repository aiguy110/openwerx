int rstBtn1 = 3;
int rstBtn2 = 4;
int ledPin  = 5;

bool rst1_pressed(){
  return !digitalRead(rstBtn1);
}

bool rst2_pressed(){
  return !digitalRead(rstBtn2);
}

bool leds_on(){
  return !digitalRead(ledPin);
}


void setup(){
	Serial.begin(19200);
	pinMode(ledPin, INPUT);
  pinMode(rstBtn1, INPUT);
  pinMode(rstBtn2, INPUT);
}

char b2c(bool x){
  if(x){
    return '1';
  }else{
    return '0';
  }
}

void loop(){
  for(;;){
    Serial.write(leds_on());
    Serial.write(rst1_pressed());
    Serial.write(rst2_pressed());
    Serial.write('\n');
    delay(10);
	}
	
}
