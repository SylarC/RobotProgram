#pragma config(Sensor, port2,  gyroSensor,     sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  ArmBumper,      sensorVexIQ_Touch)
#pragma config(Sensor, port7,  touchLED,       sensorVexIQ_LED)
#pragma config(Motor,  motor1,          leftDrive,     tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor4,          leftArm,       tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor5,          hookMotor,     tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor6,          rightDrive,    tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
#pragma config(Motor,  motor10,         rightArm,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor11,         sideDrive,     tmotorVexIQ, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void iqCalibrateGyro()
{
	short count = 20;

	startGyroCalibration(gyroSensor, gyroCalibrateSamples512);
	// delay so calibrate flag can be set internally to the gyro
	wait1Msec(100);

	eraseDisplay();

	// wait for calibration to finish or 2 seconds, whichever is longer
	while(getGyroCalibrationFlag(gyroSensor) || (count-- > 0))
	{
		displayTextLine(1, "Calibrating... %02d", count/10);
		wait1Msec(100);
	}

	eraseDisplay();

	// reset so this is 0 heading
	resetGyro(gyroSensor);

	displayTextLine(3, "Gyro Value is: %d", getGyroHeading(gyroSensor));
}
/**********************************/
/** Movement + Turning Functions **/
/**********************************/

// Turn Right 90 Degrees
void Turn90Right(){
	turnRight(310, degrees, 100);
}

// Turn Left 90 Degrees
void Turn90Left(){
	turnLeft(330, degrees, 100);
}

// Turn 180 Degrees
void Turn180(){
	turnRight(620, degrees, 100);
}

void WaitLED(){
	waitUntil(getTouchLEDValue(touchLED) == true);
}

void MoveArm(int EncoderDistance){
	resetMotorEncoder(leftArm);
	resetMotorEncoder(rightArm);
	setMotor(leftArm);
	setMotor(rightArm);
	waitUntil(getMotorEncoder(leftArm) == EncoderDistance);
	stopMultipleMotors(leftArm, rightArm);
}

void lowerArmUntilBumperPressed(){
	// Set Motors to run until bumper pressed
	if(getBumperValue(ArmBumper) == false){
		setMotor(leftArm, -100);
		setMotor(rightArm, -100);
		waitUntil(getBumperValue(ArmBumper) == true);
		stopMultipleMotors(leftArm, rightArm);
	}
}

/**********************/
/** Function Library **/
/**********************/

void PickUpLeftYellowHub(){
	// Lift arm to prepare to pick yellow hub
	moveMotorTarget(leftArm, 1000, 100);
	moveMotorTarget(rightArm, 1000, 100);
	// Move sideways to align with the yellow hub
	forward(0.1, rotations, 100);
	moveMotor(sideDrive, 1.7, rotations, 100);
	// Move forward and pick up yellow hub
	backward(0.5, seconds, 100);
	waitUntilMotorMoveComplete(leftArm);
	forward(1, rotations, 100);
	moveMotorTarget(leftArm, 225, 100);
	moveMotorTarget(rightArm, 225, 100);
	waitUntilMotorMoveComplete(rightArm);
	// Back up and turn
	backward(0.8, seconds, 100);
	Turn90Left();
	// Re-align with wall
	moveMotor(sideDrive, 0.6, seconds, 100);
}

void PickUpRightYellowHub(){
	// Lift arm to prepare to pick yellow hub
	//moveMotorTarget(leftArm, 1030, 100);
	//moveMotorTarget(rightArm, 1030, 100);
	moveMotorTarget(leftArm, 670, 100);
	moveMotorTarget(rightArm, 670, 100);
	// Move sideways to align with the yellow hub
	forward(0.1, rotations, 100);
	moveMotor(sideDrive, -1.7, rotations, -100);
	// Move forward and pick up yellow hub
	backward(0.5, seconds, 100);
	waitUntilMotorMoveComplete(leftArm);
	forward(1.1, rotations, 100);
	moveMotorTarget(leftArm, 225, 100);
	moveMotorTarget(rightArm, 225, 100);
	waitUntilMotorMoveComplete(rightArm);
	backward(0.8, rotations, 100);
	// Re-align with wall
	/*if(getGyroDegrees(gyroSensor > 0); moveMotorTarget(leftDrive, 10, 100);
	forward(0.2, rotations, 100);
	moveMotor(sideDrive, 1.8, seconds, 100);
	forward(0.2, rotations, 100);
	moveMotor(sideDrive, 0.4, seconds, -100);
	waitUntilMotorMoveComplete(leftDrive);
	moveMotorTarget(leftArm, -670, 100);
	moveMotorTarget(rightArm, -670, 100);
	//moveMotorTarget(leftArm, 525, 100);
	//moveMotorTarget(rightArm, 525, 100);
	waitUntilMotorMoveComplete(leftArm);
	forward(2.7, rotations, 100);
	turnLeft(0.3, rotations, 100);
	turnRight(0.25, rotations, 100);
	forward(1.6, rotations, 100);
	backward(0.5, rotations, 100);
	moveMotor(sideDrive, -0.1, rotations, 100);
	turnLeft(290, degrees, 100);
	moveMotor(sideDrive, 0.2, 100);
	backward(0.6, rotations, 100);
	moveMotor(sideDrive, -0.5, rotations, 100);
	moveMotor(sideDrive, 0.1, rotations, 100);
	waitUntilMotorMoveComplete(leftDrive);
	forward(2.7, rotations, 100);
	turnLeft(310, degrees, 100);
	moveMotor(sideDrive, -0.4, rotations, 100);
	backward(0.5, seconds, 100);
	forward(4, rotations, 100);*/
}

void moveThroughU(){
	// Start from starting position
	moveMotorTarget(leftArm, -680, 100);
	moveMotorTarget(rightArm, -680, 100);
	moveMotor(hookMotor, 0.2, rotations);
	forward(0.1, rotations, 100);
	moveMotor(sideDrive, 1.3, rotations, 100);
	forward(3, rotations, 100);
	turnLeft(0.3, rotations);
	turnRight(0.3, rotations);
	forward(0.85, rotations, 100);
	wait(0.5, seconds);
	backward(0.6, rotations, 100);
	turnLeft(150, degrees, 100);
	forward(1.4, seconds, 100);
	turnLeft(150, degrees, 100);
	forward(0.2, rotations, 100);
	//moveMotor(sideDrive, 0.5, seconds, -100);
	//moveMotor(sideDrive, 0.2, rotations, 100);
	// Turn with one side and push hubs
	moveMotor(rightDrive, 1.3, rotations, 100);
/*	int gyroReading = getGyroDegrees(gyroSensor);
	if(160 - gyroReading > 1)
		setMotor(rightDrive, 20);
	waitUntil((160 - getGyroDegrees(gyroSensor)) <= 0);
	stopMotor(rightDrive);*/
	// Turn with one side and push hubs
	/*moveMotor(rightDrive, 1.3, rotations, 100);
	wait(0.5, seconds);
	if(getGyroDegrees(gyroSensor) != 170){
	setMotor(rightDrive, 20);
	waitUntil(getGyroDegrees(gyroSensor) == 170);
	stopMotor(rightDrive);
	}*/
	forward(2.7, seconds, 100);
	moveMotor(hookMotor, -0.5, seconds);
	// Move orange hubs to scoring zone
	backward(0.1, rotations, 100);
	turnRight(0.85, rotations, 100);
	forward(3, seconds, 100);
	/*backward(1, rotations, 100);
	Turn90Left();
	backward(1, rotations, 100);
	moveMotor(sideDrive, 2, rotations, -100);
	forward(0.4, rotations, 100);*/
	// Score the yellow hub
	turnLeft(0.85, rotations, 100);
	forward(0.2, rotations, 100);
	moveMotor(hookMotor, 0.4, rotations);
	backward(0.4, rotations, 100);
	//Add a sideways left motion
	moveMotorTarget(leftArm, -200, 100);
	moveMotorTarget(rightArm, -200, 100);
	waitUntilMotorMoveComplete(leftArm);
	backward(1, rotations, 100);
}

void move3OrangeHubsTo2x2(){
	// Move forward, turn, then prepare to push orange hubs
	forward(1, rotations, 100);
	moveMotor(hookMotor, 0.2, rotations, 100);
	moveMotor(sideDrive, -0.8, rotations, 100);
	moveMotor(leftDrive, 290, degrees, 100);
	// Go forward and group hubs
	forward(1.6, rotations, 60);
	turnRight(175, degrees, 60);
	// Go forward and push hubs against wall
	moveMotor(hookMotor, -0.6, rotations, 100);
	forward(1.2, seconds, 100);
	moveMotor(hookMotor, 0.4, rotations, 100);
	// Back up and go forward to push hubs agianst the wall
	backward(0.5, rotations, 100);
	turnLeft(270, degrees, 100);
	backward(1.2, seconds, 100);
	/*	wait(1, seconds);
	if(getGyroDegrees(gyroSensor) < 90){
	setMotor(rightDrive, 20);
	setMotor(leftDrive, -20);
	waitUntil(getGyroDegrees(gyroSensor) == 90);
	stopMultipleMotors(leftDrive, rightDrive);
	}
	else if(getGyroDegrees(gyroSensor) < 90){
	setMotor(rightDrive, -20);
	setMotor(leftDrive, 20);
	waitUntil(getGyroDegrees(gyroSensor) == 90);
	stopMultipleMotors(leftDrive, rightDrive);
	}*/
	//backward(1.5, rotations, 100);
	// Move sideways and align to the wall
	moveMotor(sideDrive, 1.3, seconds, -100);
	// Push hubs into sccoring zone
	forward(1.4, seconds, 100);
	backward(0.2, rotations, 100);
	moveMotorTarget(leftArm, -855, 100);
	moveMotorTarget(rightArm, -855, 100);
	waitUntilMotorMoveComplete(leftArm);
	backward(1.1, rotations, 100);
}

void KnockOffRightHub(){
	// Lift arm to prepare to pick yellow hub
	moveMotorTarget(leftArm, 800, 100);
	moveMotorTarget(rightArm, 800, 100);
	wait(1, seconds);
	// Move sideways to align with the yellow hub
	forward(0.1, rotations, 100);
	moveMotor(sideDrive, -2, rotations, 100);
	// Move forward and knock off yellow hub
	backward(1, seconds, 100);
	waitUntilMotorMoveComplete(leftArm);
	forward(1.4, seconds, 100);
	Turn90Left();
	forward(4, rotations, 100);
}

/*void ScoreYellowHub(){
// Move forward
forward(6, rotations, 100);
// Drop Hub
moveMotorTarget(leftArm, -1185, 100);
moveMotorTarget(rightArm, -1185, 100);
waitUntilMotorMoveComplete(leftArm);
// Open Claw
moveMotor(clawMotor, 0.5, seconds, 100);
// Backup
backward(0.5, rotations, 100);
// Lift arm
moveMotorTarget(leftArm, 500, 100);
moveMotorTarget(rightArm, 500, 100);
// Move sideways
moveMotor(sideDrive, -1, rotations, 100);
// Move forward
forward(1, rotations, 100);
moveMotor(sideDrive, 0.8, rotations, 100);
}*/

void MoveScoringZone(){
	moveMotor(sideDrive, 0.6, rotations, 100);
	moveMotorTarget(leftArm, 925, 100);
	moveMotorTarget(rightArm, 925, 100);
	waitUntilMotorMoveComplete(rightArm);
	forward(2.3, rotations, 100);
	moveMotorTarget(leftArm, -1215, 100);
	moveMotorTarget(rightArm, -1215, 100);
}

//* Main Task *//
task main()
{
	//resetGyro(gyroSensor);
	iqCalibrateGyro();
	setTouchLEDColor(touchLED, colorViolet);
	waitUntil(getTouchLEDValue(touchLED) == true);
	PickUpLeftYellowHub();
	move3OrangeHubsTo2x2();
	waitUntil(getTouchLEDValue(touchLED) == true);
	PickUpRightYellowHub();
	moveThroughU();
	//ScoreYellowHub();
	// Go to scoring zone
	while(true){
		waitUntil(getTouchLEDValue(touchLED) == true);
		MoveScoringZone();
	}
}
