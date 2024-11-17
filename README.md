**IoT-Based Worker Safety Devic**
Introduction
This repository contains the code and design files for an IoT-based wearable safety device, designed specifically for high-risk work environments. The device integrates multiple sensors to monitor vital health and environmental parameters, ensuring worker safety and enhancing workplace security.

**Features**
@Health Monitoring:
     Heart rate and SpO2 monitoring.
     Fall detection using an accelerometer.
@Environmental Monitoring:
      Temperature and humidity tracking with the DHT sensor.
      Gas level detection using MQ series sensors (MQ-2, MQ-5, MQ-6).
@Location Tracking:
      GPS-based geofencing and real-time location tracking with the Neo-6M module.
@Alerts and Notifications:
       GSM module for sending alerts when critical thresholds are crossed or geofencing violations occur.
**Technologies Used**
    **Microcontroller**: Arduino and NodeMCU ESP8266
    **Programming Languages**: C++ for microcontroller programming.
**Sensors:**
      Heart rate (Pulse sensor)
      SpO2
      DHT (Temperature and Humidity)
      MQ Sensors
      Accelerometer and Gyroscope
**Communication Modules:**
     Neo-6M GPS
     GSM Module

**How It Works**
Sensors collect real-time data on health, environmental conditions, and location.
Data is processed by the microcontroller and thresholds are monitored.
Alerts are sent via GSM when critical events occur, like:
Abnormal heart rate or SpO2 levels.
Hazardous gas concentrations.
Falls detected by the accelerometer.
Workers moving outside safe zones.
**Use Cases**
Industrial worker safety in hazardous environments.
Monitoring employees in chemical factories, mining, or construction sites.
Emergency response systems for lone workers.
**Contributors**
Akshaya: Fall detection, heart rate monitoring, body temperature, SpO2 sensors.
Avanthi: GPS geofencing, indoor positioning, GSM alerts.
Jyoshna: Air quality sensors, DHT sensor, and poisonous gas detection.







