# BTW-Robot
![Photo of robot](/Photos/miniature.jpg?raw=true  "Balancing robot")

## Balancing Two-Wheeled Robot
This repository contains all the code files for ESP8266 microcontroller regarding the BTW-Robot project. 

The other part of code is the *android app* to control the robot. The app will have its own repo soon.

## A short description

The main working principle of a self-balancing robot is to continuously adjust its movement in order to maintain a specific position and/or velocity. 

The BTW-Robot utilizes two stepper motors which are driven to counteract its inertial movement.

## Sensor

The most important information for such a robot is the current angular position.
To get angle data I used MPU6050 inertial measurement unit. The sensor consists of both gyroscope and accelerometer (both 3 Axis).

### Accelerometer
Accelerometer measures acceleration and since under **relatively static** conditions the only significant acceleration is gravitational, we can use that measurement to estimate the tilt angle of the robot.

### Gyroscope
Although the accelerometer would give us a pretty decent estimation of angle in static conditions, the method fails if any oscillations occur.

Gyro measures angular velocity and it does so quite precisely. Precisely enough to be the only source of inertial measurements for our purpouse.
However there are two reasons we cannot use the gyro data alone:
- angular velocity is the derivative of angle and derivatives give no information about starting position. At least once we need to know actual angle, then we could rely on velocity
- gyro measurements tend to drift due to various long term noise (like Coriolis force), so even when no movement the sensor would show changing angle value over time

### Complementary filter
An easy, yet very effective solution is to combine accelerometer and gyro data.
Accelerometer doesn't like high frequecies so we'll give it a low-pass filter.
Gyro doesn't like low frequencies so we'll implement a high-pass filter.
```
angle = c * (angle + gyro * dt) + (1 - c) * accelerometer 
```
where ```c``` would be around 0.98 (depends on time constant).

The above solution is called complementary filter.


More documentation soon...
