/***********  FILE HEADER ***************************
 * @file servo_control.cpp
 * @brief Implementation of functions for controlling servo motor using Arduino's Servo library.
 *
 * This file implements functions to rotate a servo by a specified angle and 
 * to perform a slow sweep from -90° to +90°.
 ****************************************************/
#include "ServoControl.h"
#include <Arduino.h>

#define MIN_ANGLE 60    /**< Minimum servo angle */
#define MAX_ANGLE 120  /**< Maximum servo angle */
#define STEP_DELAY 20  /**< Delay in ms between steps for smooth motion */
#define STEP_ANGLE 1   /**< Angle increment for slow sweep */

// define for continuous rotation servo
#define UP_SPEED 180   /**< Maximum PWM value */
#define DOWN_SPEED 0     /**< Minimum PWM value */
#define STOP_SPEED 90   /**< PWM for stop */
#define TIME_STEP 1000  /**< Time step for precise control in milliseconds */

/************************************************************
 * @brief Initialize the servo motor on a specified pin.
 * @param servo Reference to the Servo object.
 * @param cu8Pin The pin where the servo is connected.
 */
void vInitializeServo(Servo& servo, uint8_t cu8Pin)
{
    servo.attach(cu8Pin);
    servo.write(90);  // Set to initial angle (90°)
}

/************************************************************
 * @brief Rotate the servo by a specified angle from its current position.
 * @param servo Reference to the Servo object.
 * @param u8Angle The angle to rotate (positive for clockwise, negative for counterclockwise).
 */
void vRotateByAngle(Servo& servo, uint8_t u8Angle)
{
    static uint8_t u8CurrentAngle = 90;                 // Read current angle
    uint8_t u8NewAngle = u8CurrentAngle + u8Angle;      // Calculate new angle

    // Clamp angle within bounds
    if (u8NewAngle < MIN_ANGLE) u8NewAngle = MIN_ANGLE;
    if (u8NewAngle > MAX_ANGLE) u8NewAngle = MAX_ANGLE;

    servo.write(u8NewAngle);                           // Move servo to new angle
}

/*************************************************************
 * @brief Sweep the servo slowly
 * @param servo Reference to the Servo object.
 */
void vSlowSweep(Servo& servo)
{
    // Sweep to let fall the ball 
    for (uint8_t u8Angle = POS_U_HORRIZ; u8Angle > 0; u8Angle--)
    {
        servo.write(u8Angle);  // Convert to 0-180 range
        delay(STEP_DELAY);
    }
    delay(2000);    // Wait for the ball to fall
    // Sweep to vertical position
    for (uint8_t u8Angle = 0; u8Angle < POS_U_VERT; u8Angle++)
    {
        servo.write(u8Angle);  // Convert to 0-180 range
        delay(STEP_DELAY);
    }
    delay(2000);   // Wait for the ball to be caught
    // Sweep back to horizontal position
    for (uint8_t u8Angle = POS_U_VERT; u8Angle > POS_U_HORRIZ; u8Angle--)
    {
        servo.write(u8Angle);  // Convert to 0-180 range
        delay(STEP_DELAY);
    }
}


/****************************************************************
 * @brief Adjust the height of the platform using a continuous 
 *        rotation servo and a lead screw.
 * @param servo Reference to the Servo object.
 * @param height The desired height to adjust 
 *      (positive for upward movement, negative for downward).
 */
void vAdjustHeight(Servo& servo, int16_t s16Height)
{
    // Calculate the direction of movement (up or down)
    uint16_t u16PwmValue = STOP_SPEED;

    if (s16Height > 0) 
    {
        u16PwmValue = UP_SPEED; // Move up
    } 
    else if (s16Height < 0) 
    {
        u16PwmValue = DOWN_SPEED; // Move down
    }
    // Adjust PWM value based on desired height
    servo.write(u16PwmValue);
    // Calculate the time required based on height and speed
    // Formula explained in documentation Servo_rot_continue.docx
    uint16_t u16TimeRequired = abs(s16Height) * 1000 / 1.75;  
    // M12 lead screw with 1.75mm pitch
    // M10 lead screw with 1.5mm pitch
    // M8 lead screw with 1.25mm pitch
    delay(u16TimeRequired);     // Run the motor 
    servo.write(STOP_SPEED);    // Stop the motor 
}