#include <SPI.h>
#include <Adafruit_MLX90640.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "apple2g";
const char* password = "apple234";
WebServer server(80);
const int led = 13;

Adafruit_MLX90640 mlx;
float frame[32*24]; // buffer for full frame of temperatures
float maxT;
float minT;

void cameraSetup(){
  Wire.begin(21,22); //SCL=Pin 22, SDA=Pin 21
  
  if (! mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("MLX90640 not found!");
    while (1) delay(10);
  }
  Serial.println("Found Adafruit MLX90640");

  mlx.setMode(MLX90640_CHESS);
  mlx.setResolution(MLX90640_ADC_19BIT);
  mlx.setRefreshRate(MLX90640_4_HZ);

}

void setup() {
  while (!Serial) delay(10);
  Serial.begin(115200);
  delay(100);

  cameraSetup();
  webSetup();

}

void loop(){
  delay(2);
  server.handleClient();

}
