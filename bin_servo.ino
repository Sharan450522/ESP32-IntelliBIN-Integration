#include <WiFi.h>
#include <esp_camera.h>
#include <WebServer.h>
#include <DustbinFinalDataModel_inferencing.h>
#include <ESP32Servo.h>  // Include the ESP32 Servo library

// Wi-Fi credentials
const char* ssid = "Nithees";  // Replace with your Wi-Fi SSID
const char* password = "nithee2003";  // Replace with your Wi-Fi password

// Select camera model
#define CAMERA_MODEL_AI_THINKER

#if defined(CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM      5
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22
#else
#error "Camera model not selected"
#endif

// Servo motor pin definitions
#define SERVO1_PIN 12
#define SERVO2_PIN 13
#define SERVO3_PIN 14
#define SERVO4_PIN 15

// Servo motor setup
Servo servo1, servo2, servo3, servo4;

WebServer server(80); // Start a web server on port 80
camera_fb_t* fb;      // Frame buffer for the camera

unsigned long lastCaptureTime = 0; // To track the last capture time
const unsigned long captureInterval = 5000; // 5 seconds
bool serverActive = true; // To track server status

// Function to initialize Wi-Fi connection
void connectToWiFi() {
    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting...");
    }
    Serial.println("Connected to Wi-Fi");
    Serial.print("Server ready! Use this URL to see the uploaded image: ");
    Serial.print("http://");
    Serial.println(WiFi.localIP());
}

// Function to run inference on the image and control servos based on predictions
void runInference() {
    ei_impulse_result_t result = { 0 };
    ei::signal_t signal;
    signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
    signal.get_data = &ei_camera_get_data;

    EI_IMPULSE_ERROR err = run_classifier(&signal, &result, false);
    if (err != EI_IMPULSE_OK) {
        Serial.printf("Failed to run classifier (%d)\n", err);
        return;
    }

    // Print predictions and control servos based on classification results
    Serial.println("Predictions:");
    int detected_class = -1;
    float max_score = 0.0f;
    for (int i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        Serial.printf("%s: %.5f\n", ei_classifier_inferencing_categories[i], result.classification[i].value);
        if (result.classification[i].value > max_score) {
            max_score = result.classification[i].value;
            detected_class = i;
        }
    }

    switch (detected_class) {
        case 0:
            Serial.println("Paper detected");
            bin1Drop();
            break;
        case 1:
            Serial.println("Metal detected");
            bin2Drop();
            break;
        case 2:
            Serial.println("Food detected");
            bin3Drop();
            break;
        case 3:
            Serial.println("Other detected");
            bin4Drop();
            break;
        default:
            Serial.println("No object detected or low confidence");
            defaultBin();
            break;
    }
}

// Function to handle image capture and serve as HTTP response
void handleCaptureAndUpload() {
    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        server.send(500, "text/plain", "Camera capture failed");
        return;
    }

    // Send image as response to the request
    server.sendHeader("Content-Type", "image/jpeg");
    server.send_P(200, "image/jpeg", (const char *)fb->buf, fb->len);

    // Return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);

    // Print message
    Serial.println("Image uploaded.");
}

// Servo control functions
void defaultBin() {
    servo1.write(50);
    servo2.write(0);
    servo3.write(10);
    servo4.write(77);
}

void openBin() {
    servo1.write(50);
    servo2.write(0);
    servo3.write(90);
    servo4.write(77);
    delay(3000);
    servo3.write(10);
}

void bin1Drop() {
    servo1.write(0);
    servo2.write(0);
    servo3.write(10);
    servo4.write(20);
    delay(1000);
    servo4.write(77);
}

void bin2Drop() {
    servo1.write(90);
    servo2.write(0);
    servo3.write(10);
    servo4.write(30);
    delay(1000);
    servo4.write(77);
}

void bin3Drop() {
    servo1.write(180);
    servo2.write(0);
    servo3.write(10);
    servo4.write(30);
    delay(1000);
    servo4.write(77);
}

void bin4Drop() {
    servo1.write(180);
    servo2.write(90);
    servo3.write(10);
    servo4.write(30);
    delay(1000);
    servo4.write(77);
}

// Function to setup camera
bool setupCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return false;
    }
    return true;
}

// Function to convert camera frame buffer to a signal
int ei_camera_get_data(size_t offset, size_t length, float *out_ptr) {
    size_t pixel_ix = offset * 3;
    size_t pixels_left = length;
    size_t out_ptr_ix = 0;

    while (pixels_left != 0) {
        out_ptr[out_ptr_ix] = (fb->buf[pixel_ix + 2] << 16) + (fb->buf[pixel_ix + 1] << 8) + fb->buf[pixel_ix];
        out_ptr_ix++;
        pixel_ix += 3;
        pixels_left--;
    }
    return 0;
}

void setup() {
    Serial.begin(115200);

    connectToWiFi(); // Connect to Wi-Fi
    if (!setupCamera()) {
        Serial.println("Camera setup failed");
        return;
    }

    // Attach servo motors
    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    servo3.attach(SERVO3_PIN);
    servo4.attach(SERVO4_PIN);

    // Set servos to default positions
    defaultBin();

    // Capture the first image and run inference
    fb = esp_camera_fb_get();
    if (fb) {
        runInference();
        esp_camera_fb_return(fb);  // Clear buffer
    }

    // Set up the image capture route
    server.on("/upload", HTTP_GET, handleCaptureAndUpload);
    server.begin();
    serverActive = true;
    lastCaptureTime = millis();
    Serial.println("Server started");
}

void loop() {
    if (serverActive) {
        server.handleClient();
        
        if (millis() - lastCaptureTime >= captureInterval) {
            server.stop();
            serverActive = false;
            Serial.println("Server stopped after 5 seconds.");
        }
    }
}