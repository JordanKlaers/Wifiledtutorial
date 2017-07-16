#include <SoftwareSerial.h>
#include <SparkFunESP8266WiFi.h> // Include the ESP8266 AT library

#define DEBUG true

//SoftwareSerial esp8266(9,8); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3
void setup()
{
  Serial.begin(9600);
  // esp8266.begin(9600); // your esp's baud rate might be different

//  pinMode(11,OUTPUT);
//  digitalWrite(11,LOW);
//
//  pinMode(12,OUTPUT);
//  digitalWrite(12,LOW);

  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);

  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80

  if (esp8266.begin()) // Initialize the ESP8266 and check it's return status
  Serial.println("ESP8266 ready to go!"); // Communication and setup successful
  else
  Serial.println("Unable to communicate with the ESP8266 :(");

  int retVal;
  retVal = esp8266.connect("myNetwork", "myNetworkPassword");     //must be changed per network im on
  if (retVal < 0)
  {
    Serial.print(F("Error connecting: "));
    Serial.println(retVal);
  }

  IPAddress myIP = esp8266.localIP(); // Get the ESP8266's local IP
  Serial.print(F("My IP is: ")); Serial.println(myIP);


  ESP8266Client client; // Create a client object

  retVal = client.connect("sparkfun.com", 80); // Connect to sparkfun (HTTP port)
  if (retVal > 0)
  Serial.println("Successfully connected!");


  client.print("GET / HTTP/1.1\nHost: example.com\nConnection: close\n\n");
}



//while (client.available()) // While there's data available
//    Serial.write(client.read()); // Read it and print to serial


void loop()
{
  if(esp8266.available()) // check if the esp is sending a message
  {


    if(esp8266.find("+IPD,"))
    {
     delay(1000); // wait for the serial buffer to fill up (read all the serial data)
     // get the connection id so that we can then disconnect
     int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns
                                           // the ASCII decimal value and 0 (the first decimal number) starts at 48

     esp8266.find("pin="); // advance cursor to "pin="

     int pinNumber = (esp8266.read()-48)*10; // get first number i.e. if the pin 13 then the 1st number is 1, then multiply to get 10
     pinNumber += (esp8266.read()-48); // get second number, i.e. if the pin number is 13 then the 2nd number is 3, then add to the first number

     digitalWrite(pinNumber, !digitalRead(pinNumber)); // toggle pin

     // make close command
     String closeCommand = "AT+CIPCLOSE=";
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";

     sendData(closeCommand,1000,DEBUG); // close connection
    }
  }
}

/*
* Name: sendData
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";

    esp8266.print(command); // send the read character to the esp8266

    long int time = millis();

    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {

        // The esp has data so display its output to the serial window
        char c = esp8266.read(); // read the next character.
        response+=c;
      }
    }

    if(debug)
    {
      Serial.print(response);
    }

    return response;
}
