# ESP32-IntelliBIN-Integration\

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

---

## Installation

### 1. Clone the Repository
```bash
git clone https://github.com/<your-username>/ESP32-CAM-Dustbin-Automation.git
cd ESP32-CAM-Dustbin-Automation
