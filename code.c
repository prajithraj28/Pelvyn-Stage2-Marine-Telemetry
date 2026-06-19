#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>

#include <U8g2lib.h>

// ---------------- Sensors ----------------

Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;

// ---------------- OLED ----------------

U8G2_SH1107_SEEED_128X128_F_HW_I2C u8g2(U8G2_R0);

// ---------------- WiFi ----------------

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ---------------- MQTT ----------------

const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// ------------------------------------------------

void setup_wifi() {

  Serial.println();
  Serial.print("Connecting WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ------------------------------------------------

void reconnect() {

  while (!client.connected()) {

    Serial.print("Connecting MQTT...");

    String clientId = "PelvynESP32-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {

      Serial.println("Connected");

    } else {

      Serial.print("Failed rc=");
      Serial.print(client.state());

      Serial.println(" retrying...");
      delay(3000);
    }
  }
}

// ------------------------------------------------

void setup() {

  Serial.begin(115200);

  Wire.begin();

  // OLED
  u8g2.begin();

  // MPU6050
  if (!mpu.begin()) {

    Serial.println("MPU6050 not found");

    while (1);
  }

  // BMP180
  if (!bmp.begin()) {

    Serial.println("BMP180 not found");

    while (1);
  }

  setup_wifi();

  client.setServer(mqtt_server, 1883);

  Serial.println("System Started");
}

// ------------------------------------------------

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  sensors_event_t a, g, temp;

  mpu.getEvent(&a, &g, &temp);

  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0;

  // ---------------- Serial Monitor ----------------

  Serial.println("================================");

  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Pressure    : ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Accel X : ");
  Serial.println(a.acceleration.x);

  Serial.print("Accel Y : ");
  Serial.println(a.acceleration.y);

  Serial.print("Accel Z : ");
  Serial.println(a.acceleration.z);

  Serial.print("Gyro X : ");
  Serial.println(g.gyro.x);

  Serial.print("Gyro Y : ");
  Serial.println(g.gyro.y);

  Serial.print("Gyro Z : ");
  Serial.println(g.gyro.z);

  // ---------------- OLED ----------------

  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_6x10_tf);

  u8g2.drawStr(0, 10, "Pelvyn Stage-2");

  u8g2.setCursor(0, 30);
  u8g2.print("Temp:");
  u8g2.print(temperature);
  u8g2.print(" C");

  u8g2.setCursor(0, 45);
  u8g2.print("Press:");
  u8g2.print(pressure);

  u8g2.setCursor(0, 60);
  u8g2.print("AX:");
  u8g2.print(a.acceleration.x);

  u8g2.setCursor(0, 75);
  u8g2.print("AY:");
  u8g2.print(a.acceleration.y);

  u8g2.setCursor(0, 90);
  u8g2.print("AZ:");
  u8g2.print(a.acceleration.z);

  u8g2.sendBuffer();

  // ---------------- MQTT Payload ----------------

  String payload = "{";

  payload += "\"temperature\":";
  payload += String(temperature);

  payload += ",\"pressure\":";
  payload += String(pressure);

  payload += ",\"ax\":";
  payload += String(a.acceleration.x);

  payload += ",\"ay\":";
  payload += String(a.acceleration.y);

  payload += ",\"az\":";
  payload += String(a.acceleration.z);

  payload += ",\"gx\":";
  payload += String(g.gyro.x);

  payload += ",\"gy\":";
  payload += String(g.gyro.y);

  payload += ",\"gz\":";
  payload += String(g.gyro.z);

  payload += "}";

  client.publish("pelvyn/telemetry", payload.c_str());

  Serial.println("MQTT Sent:");
  Serial.println(payload);

  delay(2000);
}
