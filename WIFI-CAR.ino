#include <WiFi.h>
#include <WebServer.h>  // Include WebServer library

// Wi-Fi Credentials (used for Access Point)
const char* ssid = "ESP32-4WD";
const char* password = "12345678";

// Create a web server on port 80
WebServer server(80);

// Configure IP Address so it doesnt overried with school IP

IPAddress ip(192, 168, 5, 0);

// Motor pins
#define ENA 21  // PWM for left motors
#define ENB 22  // PWM for right motors
#define IN1 18  // Left motor forward
#define IN2 19  // Left motor backward
#define IN3 23  // Right motor forward
#define IN4 5   // Right motor backward

void setup() {
  // Setup motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Start Serial Monitor
  Serial.begin(115200);

  // Setting the IP Configuration

  WiFi.config(ip);

  // Start Wi-Fi in Access Point (AP) mode
  WiFi.softAP(ssid, password);
  Serial.println("Wi-Fi started in AP mode");

  // Print the IP address of the ESP32
  Serial.print("Access Point IP: ");
  Serial.println(WiFi.softAPIP());

  // Setup web server routes
  server.on("/", handleRoot);  // Main page
  server.on("/forward", moveForward);
  server.on("/backward", moveBackward);
  server.on("/left", turnLeft);
  server.on("/right", turnRight);
  server.on("/stop", stopMotors);

  // Start the web server
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  // Handle web server requests
  server.handleClient();
}

// Web page for the car controls
void handleRoot() {
  String html = "<h1>ESP32 4WD Car Control</h1>"
                "<button onclick=\"location.href='/forward'\">Forward</button><br>"
                "<button onclick=\"location.href='/backward'\">Backward</button><br>"
                "<button onclick=\"location.href='/left'\">Left</button><br>"
                "<button onclick=\"location.href='/right'\">Right</button><br>"
                "<button onclick=\"location.href='/stop'\">Stop</button>";
  server.send(200, "text/html", html);
}

// Functions to control the motors
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255);  // Full speed
  analogWrite(ENB, 255);
  Serial.println("Moving Forward");
  server.send(200, "text/html", "Moving Forward");
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  Serial.println("Moving Backward");
  server.send(200, "text/html", "Moving Backward");
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 150);  // Moderate speed
  analogWrite(ENB, 150);
  Serial.println("Turning Left");
  server.send(200, "text/html", "Turning Left");
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  Serial.println("Turning Right");
  server.send(200, "text/html", "Turning Right");
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  Serial.println("Motor Stopped");
  server.send(200, "text/html", "Motors Stopped");
}
