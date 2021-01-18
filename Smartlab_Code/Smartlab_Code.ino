#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server_addr(158, 94, 0, 185);  // IP of the MySQL *server* here
char user[] = "xxxx";              // MySQL user login username
char password[] = "xxxx";        // MySQL user login password

// WiFi card example
char ssid[] = "xxxx";                    // Replace with your ssid
char pass[] = "xxxx";                   // Replace with your pass

// Sample query
char INSERT_SQL[] = "INSERT INTO `kostas`.`kostas_test` (`device_id`,`sweat_sensor`,`heart_beat_sensor`, `battery_percentage`) VALUES ('9', %d, %d, %d);";
char query[128];

WiFiClient client;
MySQL_Connection conn((Client *)&client);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int LED = 32; //LED to check if wifi is connected
int heartRate = 34; //Heart rate pin
int sweatSensor = 35; //Sweat sensor pin
int batteryLevel = 33; //Battery level pin

int ssValue = 0; //Variable to store sweat sensor value
int hrValue = 0; //Variable to store heart rate value
int blValue = 0; //Variable to store battery level value

void setup() {
  pinMode(LED, OUTPUT);
  analogReadResolution(10);
  Serial.begin(115200);
  delay(500);                             // Pauses the program for 500 milliseconds
  Serial.print("Connecting to..");        // Prints the data inside "" to the serial port
  Serial.println(ssid);                   // Prints the ssid number to the serial port and changes line
  delay(500);                             // Pauses the program for 500 milliseconds
  WiFi.disconnect();                      // Disconnects the WiFi shield from the current network
  WiFi.begin(ssid, pass);                 // Initializes the WiFi library's network settings and provides the current status
  while (WiFi.status() != WL_CONNECTED)   // WiFi is trying to connect to the Network
  {
    delay(500);                           // Pauses the program for 500 milliseconds
    Serial.println(".");                  // Prints the data inside "" to the serial port and changes line
  }                                       // After this line the program is connected to the Network
  Serial.print("SSID...");
  Serial.println (WiFi.SSID());
  Serial.println("Successfully connected!!!");

  Serial.print("IP Address allotted to NodeMcu ESP..");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address of ESP...");
  //Serial.println(WiFi.macAddress());
  //WiFi.printDiag(Serial);                     // will print out some diagnostic info

  Serial.println("Connecting...");
  if (conn.connect(server_addr, 3306, user, password)) {

    Serial.println("Connected!");
    // digitalWrite(2, HIGH);
  }
  else
    Serial.println("Connection failed.");
  //conn.close();
}

void loop() {
  // put your main code here, to run repeatedly:

    if (WiFi.status() == WL_CONNECTED)
    {
      digitalWrite(LED, HIGH);
    }
    else {
      digitalWrite(LED, LOW);
    }

  ssValue = getSweatValue();
  hrValue = getHeartRateValue();
  blValue = map(analogRead(batteryLevel), 732, 1023, 0, 100); //Variable to store battery level value as a percentage

  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  sprintf(query, INSERT_SQL, ssValue, hrValue, blValue);
  cur_mem->execute(query);
  delete cur_mem;
  //Serial.println("SUCCESS!");

  delay(250); //Delay for a period of time before restarting the process
}

int getSweatValue()
{
  long sum = 0;
  int sensorValue = 0;
  int sweat_avg = 0;

  sensorValue = analogRead(sweatSensor);
  // sum += sensorValue;
  delay(5);

  return sensorValue;

}

int getHeartRateValue()
{
  int sensorValue = 0;

  sensorValue = analogRead(heartRate);
  return sensorValue;
}
