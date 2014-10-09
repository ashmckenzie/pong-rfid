#include "application.h"
#include "Serial2.h"
#include "spark-restclient/rest_client.h"

int LEDPin = D7;
int RFIDResetPin = D6;

// 206325  - Ash Zendesk work FOB
// 507736  - White card

RestClient client = RestClient("192.168.140.205.xip.io", 4567);
// RestClient client = RestClient("requestb.in");

void lightUpLED() {
  digitalWrite(LEDPin, HIGH);
  delay(500);
  digitalWrite(LEDPin, LOW);
}

int makeHTTPRequest(char *cardNumber) {
  String response = "";
  char body[50];
  int statusCode;

  sprintf(body, "{\"cardNumber\":\"%s\"}", cardNumber);

  client.setHeader("Accept: application/json");
  client.setHeader("Content-Type: application/json");

  statusCode = client.post("/lookup", body, &response);
  // statusCode = client.post("/oqd6epoq", body, &response);

  Serial.println(statusCode);

  return statusCode;
}

void resetReader() {
  digitalWrite(RFIDResetPin, LOW);
  digitalWrite(RFIDResetPin, HIGH);
  delay(150);
}

void verifyCard(char *cardNumber) {
  Serial.println(cardNumber);

  if (makeHTTPRequest(cardNumber) == 200) {
    lightUpLED();
  }
}

void readRfid() {
  byte val = 0;
  char cardNumber[13];
  byte bytesread = 0;

  if (Serial1.available() > 0) {

    // check for header
    if ((val = Serial1.read()) == 2) {
      bytesread = 0;

      // read 10 digit code + 2 digit checksum
      while (bytesread < 12) {
        if (Serial1.available() > 0) {

          val = Serial1.read();
          Serial1.flush();

          // if header or stop bytes before the 10 digit reading
          if ((val == 0x0D)||(val == 0x0A)||(val == 0x03)||(val == 0x02)) { break; }

          if ((bytesread & 1) == 1) {
            cardNumber[bytesread >> 1] = val;
          };

          bytesread++; // ready to read next digit
        }
      }

      if (bytesread == 12) {
        resetReader();
        Serial.println("Verifying card...");
        verifyCard(cardNumber);

        while(Serial1.available())
          Serial1.read();
      }

      bytesread = 0;
    }
  }
}

// =============================================================================

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(LEDPin, OUTPUT);

  pinMode(RFIDResetPin, OUTPUT);
  digitalWrite(RFIDResetPin, HIGH);
}

void loop() {
  readRfid();
}
