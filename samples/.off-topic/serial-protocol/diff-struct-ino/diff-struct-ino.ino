#include "reqres.h"


void setup() {
  Serial.begin(9600);  // Initialize serial port for communication with the computer
}


void loop() {
  delay(500);

  char buffer[sizeof(Request) + 2];
  size_t read_count = Serial.readBytesUntil('\r', buffer, sizeof(Request) * 2);

  if (read_count != sizeof(Request)) { return; } // incomplete message

  Request req;
  memcpy(&req, buffer, sizeof(Request));


  Response res;
  res.a = req.a++;
  strncpy(res.b, "bye", 4);
    
  Serial.write((uint8_t *)&res, sizeof(Response));
  Serial.write((const char *)"\r", 1);

}
