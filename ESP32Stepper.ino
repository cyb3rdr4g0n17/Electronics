#include <WiFi.h>
#include <ESP32WebServer.h>
#include <ESP32Stepper.h>

const char* ssid = "InnovationCell";
const char* password = "12345678";

ESP32WebServer server(80);
ESP32Stepper myStepper(1800, 18, 19);

int stepperPosition = 0;
bool forwardButtonPressed = false;
bool reverseButtonPressed = false;

void handleRoot() {
  server.send(200, "text/html",
    "<!DOCTYPE HTML>"
    "<html>"
    "<head>"
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
      "<link rel=\"icon\" href=\"data:,\">"
      "<style>"
        "html { font-family: Roboto; display: inline-block; margin: 0px auto; text-align: center;}"
        ".button { background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer;}"
        ".button_ON { background-color: white; color: black; border: 2px solid #4CAF50;}"
        ".button_OFF { background-color: white; color: black; border: 2px solid #f44336;}"
      "</style>"
    "</head>"
    "<body>"
      "<h2>Stepper Motor Control using ESP32</h2>"
      "<p>Press and hold the buttons to control the stepper motor</p>"
      "<p><a href=\"/For\" onclick=\"forwardPressed()\" onmouseup=\"stopPressed()\"><button id=\"forwardButton\" class=\"button button_OFF\">Forward</button></a></p>"
      "<p><a href=\"/Rev\" onclick=\"reversePressed()\" onmouseup=\"stopPressed()\"><button id=\"reverseButton\" class=\"button button_OFF\">Reverse</button></a></p>"
      "<script>"
        "function forwardPressed() { document.getElementById('forwardButton').className = 'button button_ON'; fetch('/updateButtons', { method: 'POST', body: 'FORWARD=1' }); }"
        "function reversePressed() { document.getElementById('reverseButton').className = 'button button_ON'; fetch('/updateButtons', { method: 'POST', body: 'REVERSE=1' }); }"
        "function stopPressed() { document.getElementById('forwardButton').className = 'button button_OFF'; document.getElementById('reverseButton').className = 'button button_OFF'; fetch('/updateButtons', { method: 'POST', body: 'STOP=1' }); }"
      "</script>"
    "</body>"
  );
}

void handleForward() {
  Serial.println("Clockwise");
  while (forwardButtonPressed) {
    myStepper.step(1);
    delay(1);
    server.handleClient();
  }
  stepperPosition++;
  server.send(200, "text/plain", "STOP");
}

void handleReverse() {
  Serial.println("Counter-Clock
