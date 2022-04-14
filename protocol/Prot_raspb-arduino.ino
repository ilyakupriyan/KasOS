//Скетч с простейшим односторонним синхронным протоколом для передачи сигнала, считываемого с аналогового сигнала

#define TERMO_PIN A0
#define PROTOCOL_RX 3
#define PROTOCOL_TX 4

void setup() {
  pinMode(TERMO_PIN, INPUT);
  pinMode(PROTOCOL_TX, OUTPUT);
  pinMode(PROTOCOL_RX, INPUT);
  int sensor, copy_sensor;
  bool tx, rx;

  //проверка на подключение между устройствами
  digitalWrite(PROTOCOL_TX, 1);
  while ((rx = digitalRead(PROTOCOL_RX)) != 1) {
  }
}

void loop() {
  rx = false; tx = false; //обнуление входных и выходных бит
  sensor = analogRead(TERMO_PIN);
  copy_sensor = map(sensor, 0, 1023, 0, 255); //конвертация аналогового сигнала в ШИМ-сигнал
  int mask = 0b00000001; //маска для побитового считывания ШИМ-сигнала

  //отсылка одного байта данных
  for (int i = 0; i <= 7; i++) {
    //побитовое отделение 
    tx =  (sensor & mask) << i;
    mask = mask << 1;
    digitalWrite(PROTOCOL_TX, tx);
    while ((rx = digitalRead(PROTOCOL_RX)) != tx) { //проверка на получение верного бита данных
    }
  }
}
