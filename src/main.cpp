#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

Adafruit_MPU6050 mpu6050;

const char* SSID = "Tenda_81CF20";
const char* PASSWORD = "mwUnUEEXA6";

void setup() {
    Serial.begin(115200);
    Serial.print("Adafruit test...");

    if (!mpu6050.begin()) {
        Serial.println("Erro! MPU6050 not found");
    }

    Serial.println("MPU6050 found!");

    mpu6050.setAccelerometerRange(MPU6050_RANGE_8_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu6050.getAccelerometerRange()) {
        case MPU6050_RANGE_2_G:
            Serial.println("+-2G");
            break;
        case MPU6050_RANGE_4_G:
            Serial.println("+-4G");
            break;
        case MPU6050_RANGE_8_G:
            Serial.println("+-8G");
            break;
        case MPU6050_RANGE_16_G:
            Serial.println("+-16G");
            break;
    }

    mpu6050.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu6050.getGyroRange()) {
        case MPU6050_RANGE_250_DEG:
            Serial.println("+- 250 deg/s");
            break;
        case MPU6050_RANGE_500_DEG:
            Serial.println("+- 500 deg/s");
            break;
        case MPU6050_RANGE_1000_DEG:
            Serial.println("+- 1000 deg/s");
            break;
        case MPU6050_RANGE_2000_DEG:
            Serial.println("+- 2000 deg/s");
            break;
    }

    mpu6050.setFilterBandwidth(MPU6050_BAND_5_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu6050.getFilterBandwidth()) {
        case MPU6050_BAND_260_HZ:
            Serial.println("260 Hz");
            break;
        case MPU6050_BAND_184_HZ:
            Serial.println("184 Hz");
            break;
        case MPU6050_BAND_94_HZ:
            Serial.println("94 Hz");
            break;
        case MPU6050_BAND_44_HZ:
            Serial.println("44 Hz");
            break;
        case MPU6050_BAND_21_HZ:
            Serial.println("21 Hz");
            break;
        case MPU6050_BAND_10_HZ:
            Serial.println("10 Hz");
            break;
        case MPU6050_BAND_5_HZ:
            Serial.println("5 Hz");
            break;
    }

    WiFi.begin(SSID, PASSWORD);
    while(WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("WiFi connecting...");
    }

    Serial.printf("WiFi connected to %s", SSID);
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        sensors_event_t a, g, temp;
        mpu6050.getEvent(&a, &g, &temp);

        HTTPClient http;
        
        http.begin("http://192.168.0.178:8080/mpu/set");
        http.addHeader("Content-Type", "application/json");

        String httpPostRequest = "{\"accelX\":";
        httpPostRequest += floor(a.acceleration.x);
        httpPostRequest += ",\"accelY\":";
        httpPostRequest += floor(a.acceleration.y);
        httpPostRequest += ",\"accelZ\":";
        httpPostRequest += floor(a.acceleration.z);

        httpPostRequest += ",\"rotateX\":";
        httpPostRequest += floor(g.gyro.x);
        httpPostRequest += ",\"rotateY\":";
        httpPostRequest += floor(g.gyro.y);
        httpPostRequest += ",\"rotateZ\":";
        httpPostRequest += floor(g.gyro.z);
        httpPostRequest += "}";

        int httpCode = http.POST(httpPostRequest);
        if (httpCode > 0) {    
            Serial.print("Acceleration X: ");
            Serial.print(a.acceleration.x);
            Serial.print(", Y: ");
            Serial.print(a.acceleration.y);
            Serial.print(", Z: ");
            Serial.print(a.acceleration.z);
            Serial.println(" m/s^2");

            Serial.print("Rotation X: ");
            Serial.print(g.gyro.x);
            Serial.print(", Y: ");
            Serial.print(g.gyro.y);
            Serial.print(", Z: ");
            Serial.print(g.gyro.z);
            Serial.println(" rad/s");

            Serial.print("Temperature: ");
            Serial.print(temp.temperature);
            Serial.println(" degC");

            Serial.println("");
        }

        http.end();
    }

    delay(100);
}