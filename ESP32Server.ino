/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Assign output variables to GPIO pins
const int motor1A = 26;
const int motor1B = 27;
const int motor2A = 14;
const int motor2B = 12;

void setup() 
{
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  // Set outputs to LOW
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop()
{
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) 
  { // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) 
    { // loop while the client's connected
      if (client.available()) 
      {// if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') 
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) 
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the buttons 
            client.println("<style>html { font-family: Arial; display: flex; justify-content: center; align-items: center; height: 100%;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 20px 40px;");
            client.println("text-decoration: none; font-size: 24px; margin: 20px; cursor: pointer; border-radius: 10px;}</style></head>");
            // JavaScript for button functionality
            client.println("<script>");
            client.println("function sendCommand(command) {");
            client.println("var xhttp = new XMLHttpRequest();");
            client.println("xhttp.open('GET', command, true);");
            client.println("xhttp.send();");
            client.println("}");
            client.println("</script>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Motor Control</h1>");
            
            // Forward button
            client.println("<button class=\"button\" onmousedown=\"sendCommand('/forward')\" onmouseup=\"sendCommand('/stop')\">Forward</button>");
            // Backward button
            client.println("<button class=\"button\" onmousedown=\"sendCommand('/backward')\" onmouseup=\"sendCommand('/stop')\">Backward</button>");
            // Left button
            client.println("<button class=\"button\" onmousedown=\"sendCommand('/left')\" onmouseup=\"sendCommand('/stop')\">Left</button>");
            // Right button
            client.println("<button class=\"button\" onmousedown=\"sendCommand('/right')\" onmouseup=\"sendCommand('/stop')\">Right</button>");
            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } 
          else 
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } 
        else if (c != '\r') 
        {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
