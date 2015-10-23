#pragma config(Sensor, dgtl1, boomMax, sensorTouch) //Sensors
#pragma config(Sensor, dgtl2, boomMin, sensorTouch)
#pragma config(Motor,  port2, leftMotor, tmotorNomral, openLoop) //Motors
#pragma config(Motor,  port3, rightMotor, tmotorNormal, openLoop, reversed)
#pragma config(Motor,  port4, boomMotor, tmotorNomral, openLoop, reversed)
#pragma config(Motor,  port6, servoA, tmotorNormal, openLoop) //Servos
#pragma config(Motor,  port7, servoB, tmotorNormal, openLoop)
#pragma config(Motor, port8, servoC, tmotorNormal, openLoop)
#pragma config(Motor, port9, servoD, tmotorNormal, openLoop)

float k1 = 1; //proportionality constant for relation of joystick and motor speed
float k2 = 0;
float k3 = 1; // proportionally constant for scoop motor speed

//float currentAngle6 = 0;
float currentAngle7 = 127;
float currentAngle8 = 0;
float currentAngle9 = -127;
//float deltaAngle6 = 1; //how much the angle changes each iteration
//float deltaAngle7 = 1;
int pause = 4;  // delay between iterations
//ratio of deltaAngle & delay determines how fast the servo turns (50 degs/s right now)

//increases the speed only when Btn5D is held down

void kValues()
{
	k1 = 1;
	k2 = 0;
	k3 = 1;
	if(vexRT[Btn5U] == 1)
	{ //wheels are slowed down
		k1 = 0.3;
		k2 = 0;
	}
	if (vexRT[Btn5D] ==1)
	{ // scoop motor is slowed down
		k3 = 0.2;
	}
	/* -- Code to stop motors when pressing R
	if(vexRT[Btn8R] == 1)
	{ //boom turned on, wheels off
		k1 = 0;

	}
	*/
	k2 = 1;
}
/*
void servoPort6()  // currently not used in 2015
{
	motor[servoA] = currentAngle6;
	if(vexRT[Btn8D] == 1 && currentAngle6 > -127 + deltaAngle6)
	{ //runs when 5D is pressed and the angle won't hit the minimum (it resets to 0 when it does)
		currentAngle6 -= deltaAngle6;
		sleep(pause);
	}
	if(vexRT[Btn8U] == 1 && currentAngle6 < 127 - deltaAngle6)
	{ //runs when 5U is pressed and the angle won't hit the maximum
		currentAngle6 += deltaAngle6;
		sleep(pause);
	}
}
*/


/*void servoPort7() // currently not used in 2015
{
	motor[servoB] = currentAngle7;
	if(vexRT[Btn6D] == 1 && currentAngle7 > -127 + deltaAngle7)
	{ //runs when 6D is pressed and the angle won't hit the minimum (it resets to 0 when it does)
		currentAngle7 -= deltaAngle7;
		sleep(pause);
	}
	if(vexRT[Btn6U] == 1 && currentAngle7 < 127 - deltaAngle7)
	{ //runs when 6U is pressed and the angle won't hit the maximum
		currentAngle7 += deltaAngle7;
		sleep(pause);
	}
}
*/
 //collection bin release - must press both buttons to release the bin
 //  or reset the bin release pin
void servoPort8(){
	motor[servoC] = currentAngle8;
	if(vexRT[Btn7U] == 1 && vexRT[Btn8U] == 1){
		currentAngle8 = 90;
	}else if(vexRT[Btn7D] == 1 && vexRT[Btn8D] == 1){
		currentAngle8 = 0;
	}
}
/* blade right sweeper servo (goes from 127 to -127 with buttons 5U and 5D) */
void servoPort7(){
	motor[servoB] = currentAngle7;
	if(vexRT[Btn8L] == 1 && currentAngle7 + 1 <= 127){
		currentAngle7+= 5;
	}else if(vexRT[Btn8R] == 1 && currentAngle7 - 1 >= -127){
		currentAngle7-= 5;
	}
	sleep(pause);  // slow down
}

/* blade left sweeper servo (goes from 127 to -127 with buttons 5U and 5D) */
void servoPort9(){
	motor[servoD] = currentAngle9;
	if(vexRT[Btn7L] == 1 && currentAngle9 + 1 <= 127){
		currentAngle9+= 5;
	}else if(vexRT[Btn7R] == 1 && currentAngle9 - 1 >= -127){
		currentAngle9-= 5;
	}
	sleep(pause);  // slow down the sweeper servo action
}

void checkSensors()
{
	//if((SensorValue(boomMin) == 1 && vexRT[Ch3]<0) || (SensorValue(boomMax == 1) && vexRT[Ch3]>0))

	if (SensorValue(boomMin) == 1 && vexRT[Ch3]<0)
	{ //fixed so it can move in the opposite direction when it hits a sensor
		k2 = 0; }
	if (SensorValue(boomMax) == 1 && vexRT[Ch3]>0)
	{ k2 = 0;
//	else {
//		k2 = 1;
	} //if limit "Min" or "Max" switch is hit, motor turns off */

}

task main()
{
	while(true)
	{
		motor[leftMotor] = vexRT[Ch3]*k1;
		motor[rightMotor] = vexRT[Ch2]*k1;
		//motor[boomMotor] = vexRT[Ch3]*k2;
		if(vexRT[Btn6U] == 1){
			motor[boomMotor] = -255*k3;
		}else if(vexRT[Btn6D] == 1){
			motor[boomMotor] = 255*k3;
		}else{
			motor[boomMotor] = 0;
		}


//		servoPort6();
		servoPort7();  // right sweeper
		servoPort8();
		servoPort9();  // left sweeper
		kValues();
		checkSensors();
	}
}
