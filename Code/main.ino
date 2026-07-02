#include <TinyGPS++.h>
#include <HardwareSerial.h>

TinyGPSPlus gps;

HardwareSerial gpsSerial(1);
HardwareSerial sim800(2);

#define SOS_BUTTON 2
#define VIBRATION 3

void setup() {
  Serial.begin(115200);

  pinMode(SOS_BUTTON, INPUT_PULLUP);
  pinMode(VIBRATION, OUTPUT);

  gpsSerial.begin(9600, SERIAL_8N1, 4, 5);
  sim800.begin(9600, SERIAL_8N1, 6, 7);

  Serial.println("Women Safety Device Started");
}

void loop() {

  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (digitalRead(SOS_BUTTON) == LOW) {

    digitalWrite(VIBRATION, HIGH);

    if (gps.location.isValid()) {

      float lat = gps.location.lat();
      float lng = gps.location.lng();

      String message =
      "Emergency! I need help. My Location: https://maps.google.com/?q=" +
      String(lat,6) + "," + String(lng,6);

      sendSMS(message);
    }

    delay(5000);
    digitalWrite(VIBRATION, LOW);
  }
}

void sendSMS(String msg) {

  sim800.println("AT+CMGF=1");
  delay(1000);

  sim800.println("AT+CMGS=\"+911234567890\"");
  delay(1000);

  sim800.print(msg);

  delay(1000);

  sim800.write(26);

}
