void setup(){
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
	digitalWrite(LED_BUILTIN, HIGH);
	delay(500);
	digitalWrite(LED_BUILDIN, LOW);
	delay(500);
}
