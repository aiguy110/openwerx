int ledPin = 4;
bool leds_on(){
  return digitalRead(ledPin);
}

void setup(){
	Serial.begin(9600);
	pinMode(ledPin, INPUT);
}

void loop(){
	for(;;){
    bool lightsOn = digitalRead(ledPin);
    if (lightsOn){
      Serial.write('1');
    }else{
      Serial.write('0');
    }
	}
	
}
