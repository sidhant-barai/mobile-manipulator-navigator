# Mobile Manipulator Navigator | Autonomous Systems Project

## 📝 Project Overview
This repository documents the development of an autonomous mobile robot designed for a hybrid navigation and manipulation task. This was a **Collaborative Group Project** completed as part of the **ACS6502: Mechatronics for Robotics** module. 

The project focused on the end-to-end integration of mechanical chassis design, sensor-driven control logic, and a servo-actuated manipulator for environmental interaction.

## 👥 Collaborative Effort
As a group project, this work involved cross-functional collaboration in:
* **Mechanical Design:** Fabricating a custom chassis to house the dual-DC motor drivetrain and sensor array.
* **Electronics Integration:** Selecting and wiring IR sensors, dual ultrasonic sensors, and a servo manipulator.
* **Software Development:** Implementing a C++ state-machine for autonomous transition between navigation modes.

## 🛠️ System Features
* **Drivetrain:** Differential drive using high-torque DC motors with encoder-driven feedback.
* **Dual-Mode Navigation:** * **Spatial Awareness:** Ultrasonic sensors (HC-SR04) for wall-distance regulation.
    * **Path Following:** IR Reflectance array for high-precision tracking of designated paths.
* **Manipulation:** A servo-actuated arm designed for precision tasks within a defined workspace.

## 📂 Repository Contents
* **/code**: Final C++ source files (Arduino).
* **/media**: Workspace photos, robot design captures, and performance demonstrations.
