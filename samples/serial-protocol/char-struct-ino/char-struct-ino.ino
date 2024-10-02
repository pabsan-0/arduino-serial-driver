// Define the same struct as in the C program
typedef struct {
    int16_t a;
    long b:4;
    char c[4];
    char d;
} __attribute__((__packed__)) structTable;


void setup() {
  Serial.begin(9600);  // Initialize serial port for communication with the computer
}


void loop() {
  delay(500);

  // Seek separator and parse structTable
  char buffer[10];
  size_t read_count = Serial.readBytesUntil('\r', buffer, sizeof(structTable) * 2);

  if (read_count != sizeof(structTable)) { return; } // incomplete message

  structTable receivedData;
  memcpy(&receivedData, buffer, sizeof(structTable));

  
  // Send the response back over the serial port
  //Serial.println(response);
  //Serial.print('\\');
  
  Serial.write((uint8_t *)&receivedData, sizeof(structTable));
  Serial.write((const char *)"\r", 1);

}
