# Real-Time Embedded Telemetry and Health Monitoring Node

## Project Overview

This project was developed as part of the Pelvyn Robotics Stage-2 Technical Evaluation.

The system is designed to acquire sensor data from marine robotic platforms, store telemetry locally, and transmit data using MQTT communication.

## Hardware Components

* ESP32 DevKit V1
* MPU6050 IMU
* BMP180 Pressure Sensor
* SH1107 OLED Display

## Features

* Real-time Sensor Acquisition
* OLED Display Monitoring
* MQTT Telemetry Communication
* Health Monitoring
* JSON Data Formatting

## Architecture

The ESP32 collects data from connected sensors and publishes telemetry data to an MQTT broker. Sensor information is displayed locally and can be monitored remotely.

## Simulation Environment

Wokwi ESP32 Simulator

## Future Improvements

* SD Card Data Logging
* Cloud Dashboard Integration
* AI-Based Anomaly Detection
* OTA Firmware Updates
