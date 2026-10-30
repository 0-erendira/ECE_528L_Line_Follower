# ECE 528/L - Robotics and Embedded Systems with Lab

**CSU Northridge**

**Department of Electrical and Computer Engineering**

## Line Follower Lab

The Line Follower lab interfaces with the following:

* User LEDs of the TI MSP432 LaunchPad
* Pololu Gearmotor with Encoder - [Product Link](https://www.pololu.com/product/3675)
* 8-Channel QTRX Sensor Array - [Product Link](https://www.pololu.com/product/3672)





# Prelab Assignment

## Erendira Miranda

<br>

1) Write a void function named Chassis\_Board\_LEDs\_Init() that takes no arguments and initializes the following pins as output GPIO pins. Initialize the value of the pins to zero.<br>

* P8.0 (Front Left Yellow LED)<br>

* P8.5 (Front Right Yellow LED)<br>

* P8.6 (Back Left Red LED)<br>

* P8.7 (Back Right Red LED)<br>

<br>

```c

Chassis_Board_LEDs_Init()

{

	//initialize P8 0, 5, 6, 7 (0xE1)

	P8->SEL0 &= ~0xE1;

	P8->SEL1 &= ~0xE1;

	P8->DIR  |=  0xE1;

	P8->OUT  &= ~0xE1;

}

```

<br>

2) Write a void function named Print_Tenth that takes no arguments. Your function must iterate from 10 to 51 using a for-loop. Print a message for every 10th iteration (10, 20, 30, 40, 50) that states "Index %d: Every 10th iteration. \n" and then print a different message for the iteration that follow after the 10th iteration (11, 21, 31, 41, 51) that states "Index %d: Following the 10th iteration. \n."<br>

<br>

Use the following starter code for your main program. Save the main program as print_tenth.c in your assignment repository.<br>

<br>

Make sure that you include the code documentation in your print_tenth.c source file and add your name in the author section to get full credit. If you've used references (e.g. GeeksforGeeks, StackOverflow, etc.), cite them below.<br>

<br>

```c

/**
 * @file print_tenth.c
 * @brief Main source code for Print Tenth program.
 *
 * This file contains the main entry point and the implementation
 * of the Print_Tenth function that iterates from 10 to 51 and 
 * prints a message every 10th iteration (10, 20, 30, 40, 50)
 * and a different message following the 10th iteration (11, 21,
 * 31, 41,51).
 *
 * @author Erendira Miranda, Jose Flores 
*/

#include <stdio.h>
void Print_Tenth();

	Int main()
	{
		Print_Tenth();
		return 0;
	}

	void Print_Tenth ()
	{
		// Your implementation goes here
		for(int i=10; i<51; i++)
	{
		if( i%10 == 0)
		{
			printf(“Index %d: Every 10th iteration. \n”, i );
		}
		else if ( i%10 == 1)
		{
			printf(“Index %d: Following the 10th iteration. \n.”, i);
		}

	}	
}
```

