#include <ESP32Servo.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

Servo base;
Servo el;
Servo claw;

// Track current positions
int baseAngle = 90;
int elAngle = 90;
int clawAngle = 90;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Robotic_Arm");
  Serial.println(" Bluetooth Ready. Waiting for commands...");

  base.attach(15);
  el.attach(17);
  claw.attach(19);

  // Set all to starting positions
  base.write(baseAngle);
  el.write(elAngle);
  claw.write(clawAngle);}
//  Smooth movement function
void smoothMove(Servo &servo, int &currentAngle, int targetAngle, int delayMs = 5) {
  if (currentAngle == targetAngle) return;
  int step = (currentAngle < targetAngle) ? 1 : -1;

  for (int pos = currentAngle; pos != targetAngle; pos += step) {
    servo.write(pos);
    delay(delayMs);
  }
  servo.write(targetAngle); // Ensure final position
  currentAngle = targetAngle;
}
void loop() {
  if (SerialBT.available()) {
    char command = SerialBT.read();
    Serial.print("Received: ");
    Serial.println(command);

    // --- Base rotation ---
    if (command == '1') {
      smoothMove(base, baseAngle, 30);
      Serial.println("Base: Left");
    } else if (command == '2') {
      smoothMove(base, baseAngle, 150);
      Serial.println("Base: Right");
    } else if (command == '0') {
      smoothMove(base, baseAngle, 90);
      Serial.println("Base: Stop");
    }
     // --- Elbow control ---
    else if (command == '3') {
      smoothMove(el, elAngle, 30);
      Serial.println("Elbow: Down");
    } else if (command == '4') {
      smoothMove(el, elAngle, 130);
      Serial.println("Elbow: Up");
    



} else if (command == '7') {
      smoothMove(el, elAngle, 90);
      Serial.println("Elbow: Center");
    }
     // --- Claw control ---
    else if (command == '5') {
      smoothMove(claw, clawAngle, 180);
      Serial.println("Claw: Close");
    } else if (command == '6') {
      smoothMove(claw, clawAngle, 90);
      Serial.println("Claw: Open");
    } else if (command == '8') {
      smoothMove(claw, clawAngle, 135);
      Serial.println("Claw: Center");
    }

    // --- Reset all servos ---
    else if (command == 'r' || command == 'R') {
      resetAll();
    }

    else {
      Serial.println(" Invalid command");
    }
  }
}

//  Reset all servos smoothly
void resetAll() {
  Serial.println(" Resetting all servos...");
  smoothMove(base, baseAngle, 90);
  smoothMove(el, elAngle, 90);
  smoothMove(claw, clawAngle, 180);
  Serial.println(" All Servos Reset to Center (90°)");
  SerialBT.println(" Arm Reset to Default Positions");}
