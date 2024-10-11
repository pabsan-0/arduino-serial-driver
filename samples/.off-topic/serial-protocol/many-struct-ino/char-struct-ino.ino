// Define the same struct as in the C program
struct __attribute__((packed)) structTable {
  char page;
  char cric;
  char crac;
  char croc;
};

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

  // Prepare a response to send back
  char response[50];
  snprintf(response, sizeof(response), "Page: %c, Cric: %c, Crac: %c, Croc: %c",
    receivedData.page, 
    receivedData.cric, 
    receivedData.crac, 
    receivedData.croc
  );
  
  // Send the response back over the serial port
  //Serial.println(response);
  //Serial.print('\\');
  
  receivedData.cric += 1;
  receivedData.croc = 'D';

  Serial.write((const char *)&receivedData, sizeof(structTable));
  Serial.write((const char *)"\r", 1);

}
