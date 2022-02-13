
void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp32!");
  digitalWrite(led, 0);
}

void displayCamera(){
  String s="[";

  if (mlx.getFrame(frame) != 0) {
    
    server.send(200,"text/plain","Read failed");
    Serial.println("Failed");
    return;
  }

  for (uint8_t h=0; h<24; h++) {
    for (uint8_t w=0; w<32; w++) {
      float t = frame[h*32 + w];
      //unsigned int c=tempToColor(t);

      //tft.fillRect(w*4,h*4,4,4,c);
      s.concat(t);
      if(!(w==31 && h == 23)){s.concat(",");}

    }
    s.concat("\r\n");
    yield();
  }
  s.concat("]");

  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200,"text/plain",s);
  return;
  
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void webSetup(){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32camera")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/camera",displayCamera);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  
}
