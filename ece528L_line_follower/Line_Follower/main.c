/**
 * @file main.c
 *
 * @brief Main source code for the Line_Follower program.
 *
 * This file contains the main entry point for the Line_Follower program.
 * The main controller demonstrates a Line Follower robot without using an advanced algorithm.
 *
 * It interfaces the following peripherals using GPIO to demonstrate line following:
 *  - 8-Channel QTRX Sensor Array module
 *
 * Timers are used in this lab:
 *  - SysTick:  Used to generate periodic interrupts at a specified rate (1 kHz)
 *  - Timer A0: Used to generate PWM signals that will be used to drive the DC motors
 *  - Timer A1: Used to generate periodic interrupts at a specified rate (1 kHz)
 *
 * @note For more information regarding the 8-Channel QTRX Sensor Array module,
 * refer to the product page: https://www.pololu.com/product/3672
 *
 * @author Erendira Miranda, Jose Flores
 *
 */

#include "msp.h"
#include "inc/Clock.h"
#include "inc/CortexM.h"
#include "inc/GPIO.h"
#include "inc/EUSCI_A0_UART.h"
#include "inc/Motor.h"
#include "inc/Timer_A1_Interrupt.h"
#include "inc/SysTick_Interrupt.h"
#include "inc/Reflectance_Sensor.h"

// Initialize constant PWM duty cycle values for the motors
#define PWM_NOMINAL 2500

// Initialize a global variable for Timer A1 to keep track of elapsed time in milliseconds
static uint32_t Timer_A1_ms_elapsed = 0;

typedef enum
{
    CENTER              =   0b00011000,
    SLIGHT_LEFT_CENTER  =   0b00011100,
    SLIGHT_RIGHT_CENTER =   0b00111000,
    FAR_LEFT            =   0b00000001,
    FAR_RIGHT           =   0b10000000,
    LEFT_TURN           =   0b11111000,
    RIGHT_TURN          =   0b00011111,
    T_INTERSECTION      =   0b11111111,
    DEAD_END            =   0b00000000
} Line_Position;

typedef enum
{
    NO_TURN,
    TURNING_LEFT,
    TURNING_RIGHT
}Turn_State;

static Turn_State turn_state = NO_TURN;

void Detect_Line_Position(uint8_t reflectance_sensor_data)
{

    if (turn_state == TURNING_LEFT)
        {
            Motor_Left(1800, 1800);

            if (reflectance_sensor_data == CENTER)
            {
                turn_state = NO_TURN;
                Motor_Forward(PWM_NOMINAL, PWM_NOMINAL);
            }

            return;
        }

    if (turn_state == TURNING_RIGHT)
      {
          Motor_Right(1800, 1800);

          if (reflectance_sensor_data == CENTER)
          {
              turn_state = NO_TURN;
              Motor_Forward(PWM_NOMINAL, PWM_NOMINAL);
          }

          return;
      }

    switch(reflectance_sensor_data)
    {
        case CENTER:
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_GREEN);
            Motor_Forward(PWM_NOMINAL, PWM_NOMINAL);
            break;

        case SLIGHT_LEFT_CENTER:
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_YELLOW);
            Motor_Forward(PWM_NOMINAL, 2000);
            break;

        case SLIGHT_RIGHT_CENTER:
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_PINK);
            Motor_Forward(2000, PWM_NOMINAL);
            break;

        case FAR_LEFT:
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_WHITE);
            Motor_Left(3500, 500);
            break;

        case FAR_RIGHT:
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_SKY_BLUE);
            Motor_Right(500, 3500);
            break;

        case T_INTERSECTION:
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_BLUE);
            Motor_Forward(PWM_NOMINAL, PWM_NOMINAL);
            break;

        case DEAD_END:
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_RED);
            Motor_Stop();
            break;

        default:
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_OFF);
            Motor_Stop;
            break;
    }
}

void Timer_A1_Periodic_Task(void)
{
    // Increment Timer_A1_ms_elapsed by 1 every time the Timer A1 periodic interrupt occurs
    Timer_A1_ms_elapsed++;

    // Start the process of reading the reflactance sensor array every 10 ms (i.e. 10, 20, 30, ...)
    if ((Timer_A1_ms_elapsed % 10) == 0)
    {
        Reflectance_Sensor_Start();
    }

    // Finish reading the reflectance sensor sensor array after 1 ms (i.e. 11, 21, 31, ...)
    if((Timer_A1_ms_elapsed %10) == 1)
    {
        uint8_t Reflectance_Sensor_Data = Reflectance_Sensor_End();
        Detect_Line_Position(Reflectance_Sensor_Data);
    }
}

int main(void)
{
    // Ensure that interrupts are disabled during initialization
    DisableInterrupts();

    // Initialize the 48 MHz Clock
    Clock_Init48MHz();

    // Initialize motors
    Motor_Init();

    // Initialize the built-in red LED and the RGB LED on the MSP432 microcontroller
    LED1_Init();
    LED2_Init();

    // Initialize the 8-Channel QTRX Reflectance Sensor Array module
    Reflectance_Sensor_Init();

    // Initialize the SysTick timer to generate periodic interrupts every 1 ms
    SysTick_Interrupt_Init(SYSTICK_INT_NUM_CLK_CYCLES, SYSTICK_INT_PRIORITY);

    // Initialize Timer A1 periodic interrupts every 1 ms
    Timer_A1_Interrupt_Init(&Timer_A1_Periodic_Task, TIMER_A1_INT_CCR0_VALUE);

    // Enable the interrupts used by the modules
    EnableInterrupts();

    while(1)
    {

    }
}
