# Mobile Manipulator Navigator

## 📝 Project Description
This repository documents the development of a fully autonomous mobile robot designed to bridge the gap between mechanical design, electronic control, and real-time software logic. The system was engineered to navigate complex environments using a dual-stage sensing approach: regulating distance from obstacles before transitioning to high-precision path following.

## 🛠️ System Architecture
* **Controller:** Arduino Uno.
* **Actuation:** * Differential drive using dual high-torque DC motors for locomotion.
    * Servo-actuated manipulator for environmental interaction (task execution).
* **Sensors:**
    * **Dual Ultrasonic Sensors (HC-SR04):** Employed for real-time wall-distance regulation and spatial alignment.
    * **RedBot IR Reflectance Sensors:** Utilized for high-precision line tracking (calibrated for black/red paths).
    * **Encoder-Driven Odometry:** Integrated wheel encoders for tracking angular displacement and velocity.

## 💻 Technical Implementation
* **Hybrid Control Strategy:** Implemented a state-machine in **C++** that intelligently manages transitions between ultrasonic wall-following and IR-based line tracking.
* **Power Management:** Conducted a comprehensive power audit to calculate voltage and current requirements, ensuring stable operation for both logic and high-draw actuators.
* **Simulation & Validation:** * Circuits and component interfacing were validated using **TinkerCAD**.
    * Mechanical parts were designed in **Fusion 360** and manufactured via **3D printing** and **Laser Cutting**.

## 📹 Performance Demo
*(Your muted video demo goes here)*
The demonstration showcases the robot successfully identifying the path, maintaining safe distances from environment boundaries, and utilizing the manipulator once the target zone is reached.

## 📂 Repository Contents
* `/code`: Cleaned Arduino source files and logic descriptions.
* `/design`: TinkerCAD schematics and Fusion 360 assembly photos.
* `/docs`: Power Management reports and component datasheets.
