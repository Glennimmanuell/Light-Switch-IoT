#define timeSeconds 10 // lama sensor mendeteksi tidak adanya gerakan sampai mati.

//Setup Pinout untuk servo, sensor, led
const int led = 26;
const int motionSensor = 27;
const int servoPin = 25; 

// Deklarasi 
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;
boolean motion = false;

//Bagian untuk memutar derajat dari lengan servo motor
void moveServo(int degrees) {
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(500 + degrees * 11);
  digitalWrite(servoPin, LOW);
  delayMicroseconds(19500 - degrees * 11); // Untuk PWM
}

// Fungsi dimana ketika sensor mendeteksi pergerakan, nyalakan LED dan memutar lengan servo sesuai derajat yang saya inginkan
void IRAM_ATTR detectsMovement() {
  digitalWrite(led, HIGH);
  moveServo(140); //Putar lengan servo sejauh 140 derajat, ini menyesuaikan kemiringan dari saklar lampu rumah saya
  startTimer = true;
  lastTrigger = millis();
}

void setup() {
  Serial.begin(115200);
  pinMode(motionSensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING); 
  //Fungsi ini menempelkan interrupt pada pin yang terhubung dengan sensor gerak. 
  //Interrupt akan dipicu saat terjadi kenaikan (RISING) pada pin tersebut
  // dan akan memanggil fungsi detectsMovement() saat interrupt terjadi. Fungsi ini akan dieksekusi setiap kali terdeteksi gerakan oleh sensor.


  // Konfigurasi LED dan servo motor
  pinMode(led, OUTPUT);
  pinMode(servoPin, OUTPUT);
  digitalWrite(led, LOW);
  digitalWrite(servoPin, LOW);
}

void loop() {
  now = millis();
  
  if ((digitalRead(led) == HIGH) && (motion == false)) {
    Serial.println("MOTION DETECTED!!!");
    motion = true;
  }
  
  // bagian ini di eksekusi ketika dalam 10 detik sensor tidak mendeteksi adanya pergerakan
  if (startTimer && (now - lastTrigger > (timeSeconds * 1000))) {
    Serial.println("Motion stopped...");
    digitalWrite(led, LOW); // mematikan LED
    moveServo(100); //mengembalikan posisi lengan servo ke 100 derajat
    startTimer = false;
    motion = false;
  }
}
