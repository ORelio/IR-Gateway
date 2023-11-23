/* ============================================== */
/* IR Gateway v1.0.2 - (c) 2023 ORelio - CCDL 1.0 */
/* https://opensource.org/license/cddl-1-0/       */
/* ============================================== */

// WiFi Configuration
// https://github.com/tzapu/WiFiManager

#define USE_WIFI_MANAGER // Comment this to use hardcoded credentials
//const char* wifi_ssid   = "MyWyFiNetwork";
//const char* wifi_pass   = "S3cr3tWiFiP4ssw0rd";

#ifdef USE_WIFI_MANAGER
  #include <WiFiManager.h>
  WiFiManager wifiManager;
#endif

// IR Sender
// https://github.com/Arduino-IRremote/Arduino-IRremote/
#define DECODE_NEC
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>

// Web Server
// https://github.com/esp8266/ESPWebServer
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

// Serve the Home Page
// Contains status information and API reference
void apiHomepage() {
  String html = "<!DOCTYPE HTML>\r\n";
  html += "<html>\r\n";
  html += "  <head>\r\n";
  html += "    <title>IR Gateway</title>\r\n";
  html += "    <style type=\"text/css\">\r\n";
  html += "      html {\r\n";
  html += "        font-family: monospace;\r\n";
  html += "        background-color: black;\r\n";
  html += "        color: limegreen;\r\n";
  html += "        margin: 4.5em 3em;\r\n";
  html += "      }\r\n";
  html += "      h2, h3, a {\r\n";
  html += "        color: lime;\r\n";
  html += "      }\r\n";
  html += "      .logo {\r\n";
  html += "        display: flex;\r\n";
  html += "        color: lightgray;\r\n";
  html += "        padding-bottom: 1em;\r\n";
  html += "      }\r\n";
  html += "      .logo img {\r\n";
  html += "        transform: scale(2);\r\n";
  html += "        image-rendering: crisp-edges;\r\n";
  html += "      }\r\n";
  html += "      .logo h1 {\r\n";
  html += "        font-size: 2.5em;\r\n";
  html += "        margin-top: 1.2em;\r\n";
  html += "        margin-bottom: 0;\r\n";
  html += "      }\r\n";
  html += "      .logo .version-info {\r\n";
  html += "        margin-left: 1em;\r\n";
  html += "      }\r\n";
  html += "      .blink {\r\n";
  html += "        animation: blinking 1s linear infinite;\r\n";
  html += "      }\r\n";
  html += "      @keyframes blinking {\r\n";
  html += "        50% {\r\n";
  html += "          opacity: 0;\r\n";
  html += "        }\r\n";
  html += "      }\r\n";
  html += "    </style>\r\n";
  html += "  </head>\r\n";
  html += "  <body>\r\n";
  html += "    <div class=\"logo\">\r\n";
  html += "      <div><img src=\"data:image/gif;base64,R0lGODlhMAAwAHMAACH/C05FVFNDQVBFMi4wAwEAAAAh+QQJCgAPACwAAAAAMAAwAIMA/gDtHCQAAAAGBws8PUtqaXiampqzs7PNzc3+/v4AAAAAAAAAAAAAAAAAAAAAAAAE//DJSau9MmiNu//VJoJkOYmbqZbj6nrpC8ZPm3EyphV8cNO0HCVAKPIKvpotKCQWjQHkEMeUOZ/G3omKE9aw2AJB+is3wWExOVZdXdFjrTLZNr2LibS823Xdn3lPR0leIYCBBIhxZIUnYAmIiEeMjX+QcIONGXCXaJmVYACHnnKFdwCieKSUOVeXqKmKi4ReroGogHq0rXBFuImCpbWPssBZrFZokIhvn6acgRvBu7y9WFfOw9adx9TJxsqKwsPLeMXdmq5Y59na1mDjZ+vc8PHVj5j2vLLsyOTmuYL5kxew3CxNhopJGvjsXBZvCIk41JdOVT2ICL+QwphR40KOGRCdBKLYcdPDkjPigERppmQEADs=\"></div>\r\n";
  html += "      <div>\r\n";
  html += "        <h1>IR Gateway <span class=\"blink\">_</span></h1>\r\n";
  html += "        <span class=\"version-info\">v1.0.2 - By ORelio</span>\r\n";
  html += "      </div>\r\n";
  html += "    </div>\r\n";
  html += "    <h2>Wi-Fi Status</h2>\r\n";
  html += "    <ul>\r\n";
  html += "      <li>SSID: ";
  html += WiFi.SSID();
  html += "</li>\r\n";
  html += "      <li>Signal: ";
  html += WiFi.RSSI();
  html += " dBm</li>\r\n";
  html += "    </ul>\r\n";
  html += "    <h2>API Reference</h2>\r\n";
  html += "    <h3><a href=\"/ir/send?address=0&command=0\">/ir/send</a></h3>\r\n";
  html += "    <ul>\r\n";
  html += "      <li>Sends NEC IR Command\r\n";
  html += "      <li>Method: GET</li>\r\n";
  html += "      <li>Arguments:\r\n";
  html += "        <ul>\r\n";
  html += "          <li><strong>address</strong> (int): IR Device Address</li>\r\n";
  html += "          <li><strong>command</strong> (int): IR Device Command</li>\r\n";
  html += "        </ul>\r\n";
  html += "      </li>\r\n";
  html += "      <li>Status codes:\r\n";
  html += "        <ul>\r\n";
  html += "          <li><strong>200</strong>: Command Sent</li>\r\n";
  html += "          <li><strong>400</strong>: Invalid arguments</li>\r\n";
  html += "        </ul>\r\n";
  html += "      </li>\r\n";
  html += "    </ul>\r\n";
  html += "    <h3><a href=\"/ir/receive\">/ir/receive</a></h3>\r\n";
  html += "    <ul>\r\n";
  html += "      <li>Capture NEC IR Command\r\n";
  html += "      <li>Note: Sending is disabled while receiving\r\n";
  html += "      <li>Method: GET</li>\r\n";
  html += "      <li>Reponse body:\r\n";
  html += "        <ul>\r\n";
  html += "          <li><strong>address</strong> (int): IR Device Address</li>\r\n";
  html += "          <li><strong>command</strong> (int): IR Device Command</li>\r\n";
  html += "        </ul>\r\n";
  html += "      </li>\r\n";
  html += "      <li>Status codes:\r\n";
  html += "        <ul>\r\n";
  html += "          <li><strong>200</strong>: Command Received</li>\r\n";
  html += "          <li><strong>504</strong>: Timed out</li>\r\n";
  html += "        </ul>\r\n";
  html += "      </li>\r\n";
  html += "    </ul>\r\n";
  html += "  </body>\r\n";
  html += "</html>\r\n";
  server.send(200, "text/html", html);
}

// Serve 404 errors as JSON responses
void apiNotFound() {
  String json = "{\"success\": false, \"message\":\"404 Not Found\"}";
  server.send(404, "application/json", json);
}

// API for Sending an IR code
// Takes IR Address and Command
void apiIrSend() {
  int address = -1;
  int command = -1;
  bool success = false;
  String json = "{\"success\": false}";

  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "address")
      address = server.arg(i).toInt();
    if (server.argName(i) == "command")
      command = server.arg(i).toInt();
  }

  if (address >= 0 && command >= 0) {
    success = true;

    json = "{\"success\": true, \"message\":\"Sending Command\", \"address\":";
    json += address;
    json += ", \"command\":";
    json += command;
    json += "}";

    Serial.print("Sending Address=");
    Serial.print(address);
    Serial.print(", Command=");
    Serial.println(command);

    IrSender.sendNEC(address, command, 3);
  } else {
    json = "{\"success\": false, \"message\":\"Address/Command is missing or invalid\"}";
  }

  server.send(success ? 200 : 400, "application/json", json);
}

// API for receiving an IR code
// Listens for 10 seconds max and returns IR Address and Code
void apiIrReceive() {
  bool success = false;
  String json = "{\"success\": false}";

  Serial.print("Receiving... ");
  digitalWrite(BUILTIN_LED, LOW);
  IrReceiver.start();

  for (int i = 1; i < 100; i++) {
    if (IrReceiver.decode() && IrReceiver.decodedIRData.protocol != UNKNOWN) {
      success = true;

      json = "{\"success\": true, \"message\":\"Received Command\", \"address\":";
      json += IrReceiver.decodedIRData.address;
      json += ", \"command\":";
      json += IrReceiver.decodedIRData.command;
      json += "}";

      Serial.print("Address=");
      Serial.print(IrReceiver.decodedIRData.address);
      Serial.print(", Command=");
      Serial.println(IrReceiver.decodedIRData.command);

      break;
    } else {
      delay(100);
      if (i == 99) {
        Serial.println("Timeout");
        json = "{\"success\": false, \"message\":\"Timed out\"}";
      }
    }
  }

  IrReceiver.stop();
  digitalWrite(BUILTIN_LED, HIGH);
  server.send(success ? 200 : 504, "application/json", json);
}

// Main Setup function
void setup(void) {
  // Turn off LED
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  // Serial
  Serial.begin(115200);

  // Wi-Fi
  WiFi.setHostname("IR-Gateway");
  #ifdef USE_WIFI_MANAGER
    Serial.println("Running Wi-Fi Manager");
    wifiManager.setConnectTimeout(10);
    wifiManager.setConnectRetries(2);
    wifiManager.autoConnect();
  #else
    Serial.print("Connecting to ");
    Serial.print(wifi_ssid);
    WiFi.begin(wifi_ssid, wifi_pass);
    int delay_retries = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if (++delay_retries == 60) {
        Serial.println(" Timeout");
        ESP.reset();
      }
    }
    Serial.println(" Connected");
  #endif

  // IR Sender
  IrSender.begin();
  Serial.print("IR sender started on pin ");
  Serial.println(IR_SEND_PIN);

  // IR Receiver
  IrReceiver.begin(IR_RECEIVE_PIN);
  Serial.print("IR receiver started on pin ");
  Serial.println(IR_RECEIVE_PIN);

  // HTTP Server
  server.on("/", apiHomepage);
  server.on("/ir/send", apiIrSend);
  server.on("/ir/receive", apiIrReceive);
  server.onNotFound(apiNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

// HTTP Server loop
void loop(void) {
  server.handleClient();
}
