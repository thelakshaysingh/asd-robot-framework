#include <WiFi.h>
#include <WebServer.h>

// ======================================
// WIFI
// ======================================

const char* ssid = "ESP32_ROBOT";
const char* password = "12345678";

WebServer server(80);

// ======================================
// MOTOR PINS
// ======================================

// LEFT SIDE MOTORS
#define IN1 13
#define IN2 12

// RIGHT SIDE MOTORS
#define IN3 14
#define IN4 27

// ENABLE PINS
#define ENA 26
#define ENB 25

// ======================================
// SPEED
// ======================================

int speedValue = 200;

// ======================================
// MOTOR FUNCTIONS
// ======================================

void forward()
{
  Serial.println("FORWARD");

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward()
{
  Serial.println("BACKWARD");

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left()
{
  Serial.println("LEFT");

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right()
{
  Serial.println("RIGHT");

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopRobot()
{
  Serial.println("STOP");

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ======================================
// SETUP
// ======================================

void setup()
{
  Serial.begin(115200);

  // MOTOR OUTPUTS

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // MOTOR SPEED

  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);

  // STOP INITIALLY

  stopRobot();

  // WIFI HOTSPOT

  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("WiFi Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // ======================================
  // ROUTES
  // ======================================

  server.on("/", []()
  {
    server.send(200, "text/plain", "ESP32 Robot Running");
  });

  // FORWARD

  server.on("/F", []()
  {
    forward();
    server.send(200, "text/plain", "FORWARD");
  });

  // BACKWARD

  server.on("/B", []()
  {
    backward();
    server.send(200, "text/plain", "BACKWARD");
  });

  // LEFT

  server.on("/L", []()
  {
    left();
    server.send(200, "text/plain", "LEFT");
  });

  // RIGHT

  server.on("/R", []()
  {
    right();
    server.send(200, "text/plain", "RIGHT");
  });

  // STOP

  server.on("/S", []()
  {
    stopRobot();
    server.send(200, "text/plain", "STOP");
  });

  // ======================================
  // SPEED CONTROL
  // ======================================

  server.on("/speed", []()
  {
    if(server.hasArg("value"))
    {
      speedValue = server.arg("value").toInt();

      analogWrite(ENA, speedValue);
      analogWrite(ENB, speedValue);

      Serial.print("Speed: ");
      Serial.println(speedValue);
    }

    server.send(200, "text/plain", "OK");
  });

  // START SERVER

  server.begin();

  Serial.println("Server Started");
}

// ======================================
// LOOP
// ======================================

void loop()
{
  server.handleClient();
}