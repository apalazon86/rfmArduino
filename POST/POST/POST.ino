#include <SPI.h>
#include <Ethernet.h>
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192,168,1,40);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 187);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

//String data="{\"rfm\":\"0001\",\"name\":\"prueba\"}";
void setup() {
  Serial.begin(9600);

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("Iniciando Ethernet Shield");
}

void loop() {
  // if you get a connection, report back via serial:
  /*if (client.connect(server, 2403)) {
    Serial.println("connected");
    client.println("POST /products HTTP/1.1"); 
    client.println("Host: 192.168.1.40:2403"); 
    client.println("Content-Type: application/json"); 
    client.print("Content-Length: "); 
    client.println(data.length()); 
    client.println(); 
    client.print(data);   
    Serial.println("posted");
  }
  else {
  // if you didn't get a connection to the server:
   Serial.println("connection failed");
  }*/
}

