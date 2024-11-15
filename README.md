# ESP32-CAM Dustbin Automation

An IoT-enabled smart dustbin system built with the ESP32-CAM module that uses machine learning to classify waste (e.g., paper, metal, food, etc.) and control servo motors to sort the waste into designated bins.

---

## Features
- **Camera Integration**: Captures images of waste using the ESP32-CAM.
- **Machine Learning**: Uses Edge Impulse's inferencing model to classify waste.
- **Servo Motor Control**: Automatically opens specific bins based on classification results.
- **Wi-Fi Connectivity**: Hosts a web server to capture and upload images for remote monitoring.

---

## Hardware Requirements
- ESP32-CAM (AI Thinker Module)
- Servo Motors (4x)
- External 5V Power Supply for Servos
- USB-to-Serial Adapter
- Connecting wires and jumper cables
- Waste bin compartments

---

## Software Requirements
- Arduino IDE
- ESP32 Board Support Package
- Libraries:
  - [ESP32Servo](https://github.com/madhephaestus/ESP32Servo)
  - [Edge Impulse Inferencing SDK](https://docs.edgeimpulse.com/)
  - WiFi Library (built-in)
  - ESP32 Camera Library (built-in)


## Installation

Step 1: Clone the Repository
```bash
git clone https://github.com/Sharan450522/ESP32-IntelliBIN-Integration.git
cd ESP32-IntelliBIN-Integration

Step 2: Set Up Arduino IDE
Install the ESP32 board package:
Add the URL https://dl.espressif.com/dl/package_esp32_index.json in File > Preferences > Additional Board Manager URLs.
Go to Tools > Board > Boards Manager and search for ESP32.
Install the ESP32 package.
Install the required libraries using the Library Manager.

Step 3: Configure Wi-Fi
Update your Wi-Fi credentials in the code (src/main.ino) or a separate WiFiConfig.h file:

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

Step 4: Upload the Code
Connect the ESP32-CAM to your computer using a USB-to-Serial adapter.
Ensure GPIO0 is connected to GND for programming mode.
Open src/main.ino in the Arduino IDE.
Select AI Thinker ESP32-CAM as the board under Tools > Board.
Upload the code to the ESP32-CAM.

Usage
Power On the System
Provide stable power to the ESP32-CAM and servo motors. Use a dedicated 5V external power supply for the servos.

Access the Web Interface
Open the Serial Monitor to view the ESP32-CAM's IP address.
Access the web interface by entering the IP address in your browser:
arduino
http://<ESP32-CAM-IP>/upload

Classify and Sort Waste
Place an item in front of the camera.
The system will classify the waste (e.g., paper, metal, food) and control the servo motors to open the appropriate bin.
