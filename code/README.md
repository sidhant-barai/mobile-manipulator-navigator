# Software Logic & Implementation

This folder contains the source code for the **Mobile Manipulator Navigator**. The logic is written in C++ for the Arduino platform and follows a modular design to ensure real-time responsiveness.

## 🧠 Core Logic Overview
The robot operates based on a multi-stage logic flow:
1. **Spatial Regulation:** Uses the `measureDistance()` function to process data from the front and right ultrasonic sensors to align with environment boundaries.
2. **Transition State:** Detects path entry using defined thresholds for Red, Yellow, and Black surfaces.
3. **Path Following:** Executes a feedback loop (`followBlackLine()`) using IR reflectance data to adjust motor PWM values for smooth navigation.
4. **Task Execution:** Upon reaching a distance threshold (13cm) from a target, the robot stops and triggers the `Servo` manipulator.

## 📦 Dependencies
* `Servo.h`: For controlling the manipulator arm.
* `RedBot.h`: For interfacing with the IR reflectance sensor array.

## ⚙️ Key Technical Features
* **Interrupts:** Utilizes `attachInterrupt` for low-latency wheel encoder tracking (`channelA` / `channelB`).
* **Calibration:** Includes customized sensor thresholds for robust performance across different lighting conditions.
