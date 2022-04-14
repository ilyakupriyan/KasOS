void setup() {
  pinMode (4, INPUT);
  pinMode (13, OUTPUT);
}

void loop() {
  bool sign = digitalRead (4);
  digitalWrite (13, sign);
}
