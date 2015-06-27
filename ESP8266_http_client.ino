/*
 * Initial test of nodemcu v2 with http client, on webpage are temperatures like " &temp1-temp2& "
 */
#include <ESP8266WiFi.h>
WiFiClient client;

const char* ssid     = "SSID";
const char* password = "password";

const char* host = "your webpage";

const int httpPort = 80;

unsigned long previousMillis, previousMillis1 = 0;
String temp0,temp1, output = "";
float Temp0,Temp1,lasttemp0,lasttemp1=0;
int interval=5000;

void setup() {
  Serial.begin(115200);
  delay(10);
  // We start by connecting to a WiFi network

  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println(0);
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if (millis() - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = millis();


    // Use WiFiClient class to create TCP connections

    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    // We now create a URI for the request
    String url = "/url/of/your/page/";
    url += "";

    //Serial.print("Requesting URL: ");
    //Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(10);

    // Read all the lines of the reply from server and print them to Serial
    while (client.available()) {
      String response = client.readStringUntil('\r');
      int startoftemp = response.indexOf('&', 0); // searching for first &
      int endoftemp = response.indexOf('&', startoftemp + 1); // searching for second &

      output = response.substring(startoftemp, endoftemp);
      output.remove(0, 1);

      //Serial.println(output);
      int dash =output.indexOf('-');
      int lengthofoutput = output.length();
      //Serial.println(dash);
      temp0 = output.substring(dash, lengthofoutput);
      temp0.remove(0,1);
      
      Temp0= temp0.toFloat();
      temp1= output.substring(0,dash);
      Temp1= temp1.toFloat();
    }
  }
  if (millis() - previousMillis1 >= interval) {
    // save the last time you blinked the LED
    previousMillis1 = millis();
    if (lasttemp0 !=Temp0 || lasttemp1 != Temp1 )
      {
        lasttemp0 = Temp0;
        lasttemp1 = Temp1;
    Serial.println("");
    Serial.print("Temp0: ");
    Serial.println(Temp0);
    Serial.print("Temp1: ");
    Serial.println(Temp1);
      }
  }
}

