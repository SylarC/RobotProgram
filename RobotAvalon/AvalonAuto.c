#pragma config(Sensor, port2,  touchLED,       sensorVexIQ_LED)
#pragma config(Sensor, port3,  gyroSensor,     sensorVexIQ_Gyro)
#pragma config(Motor,  motor1,          leftDrive,     tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor4,          sideDrive,     tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor6,          rightDrive,    tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
#pragma config(Motor,  motor9,          hookMotor,     tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor10,         rightArm,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor12,         leftArm,       tmotorVexIQ, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**************************/
/** Initiation Functions **/
/**************************/

void iqCalibrateGyro()
{
	short count = 20;
	startGyroCalibration(gyroSensor, gyroCalibrateSamples512);
	// delay so calibrate flag can be set internally to the gyro
	wait1Msec(100);
	eraseDisplay();
	// wait for calibration to finish or 2 seconds, whichever is longer
	while(getGyroCalibrationFlag(gyroSensor) || (count-- > 0)){
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

// Move arm to specified position
void moveArm(int EncoderAmount, char stopUntilMotorFinished){
	if(stopUntilMotorFinished == 'y'){
		setMotorTarget(leftArm, EncoderAmount, 100);
		setMotorTarget(rightArm, EncoderAmount, 100);
		waitUntilMotorMoveComplete(leftArm);
	}
	else if(stopUntilMotorFinished == 'n'){
		setMotorTarget(leftArm, EncoderAmount, 100);
		setMotorTarget(rightArm, EncoderAmount, 100);
	}
}

// Move side drive based on tiles
void moveSideDrive(char direction, int tiles){
	if(direction == 'r'){
		moveMotor(sideDrive, -tiles*500, degrees, 100);
	}
	else if(direction == 'l'){
		moveMotor(sideDrive, tiles*500, degrees, 100);
	}
}

// Move robot based on tiles
void moveRobot(char direction, int tiles, int power){
	if(direction == 'f'){
		forward(tiles*317, degrees, power);
	}
	else if(direction == 'b'){
		backward(tiles*317, degrees, power);
	}
}

// Turn robot using Gyro to ensure accuracy
void turnRobotTo(float degrees, int sleepMSec){
	//Loop while the gyro sees a value less than 90 degrees.
	int currentDegree = getGyroDegrees(gyroSensor);

	if (abs(currentDegree - degrees) <= 1.0)
		return;
	int reverseTurnFlag = 1;

	while(abs(currentDegree - degrees) > 5.0)
	{
		reverseTurnFlag = (currentDegree - degrees)>0? 1:-1;
		displayTextLine(3, "Gyro degree is: %d", currentDegree);
		displayTextLine(4, "Reverse flag: %d", reverseTurnFlag);
		//Set the motors to turn to the left at 25% speed.
		int degreeToTurn = 10 * reverseTurnFlag;
		setMotorSpeed(leftDrive, degreeToTurn);
		setMotorSpeed(rightDrive, degreeToTurn * -1);
		sleep(200);  //wait 1 second for the robot to come to a complete stop.
		//Specifically stop the motor at the end to force the robot to come to a "Holding" stop.
		setMotorSpeed(leftDrive, 0);
		setMotorSpeed(rightDrive, 0);
		// since getGyroDegrees can accumulate degrees over multiples of 360,
		// we can safely use remainder of 360
		currentDegree = getGyroDegrees(gyroSensor)%360;
	}
	sleep(100);
}

/*******************************/
/** Program Command Functions **/
/*******************************/

// PART 1 //

void pickUpRightYellowHub(){
	// Start in right position
	// Lift arm to prepare to pick yellow hub
	moveArm(1025, 'n');
	// Move forward to not scrape wall
	moveRobot('f', 0.75, 100);
	// Move sideways to align with yellow hub
	moveMotor(sideDrive, 1.65, rotations, -100);
	// Back up and re-align with wall
	backward(0.7, seconds, 100);
	// Move forward to yellow hub
	moveRobot('f', 1.5, 100);
	// Wait until arm raised so we don't knock off yellow hub
	waitUntilMotorMoveComplete(leftArm);
	moveArm(1370, 'y');
	// Back up to re-align with wall
	backward(1, seconds, 50);
}

void pickUpTwoFromTheEightHubs(){
	// Start from end of pickUpRightYellowHub
	// Move forward to allow space to turn
	forward(0.1, rotations, 50);
	// Lower arm to prevent tipping
	moveArm(630, 'n');
	// Move sideways back to starting area
	moveMotor(sideDrive, 0.5, rotations, 100);
	// Turn 90 degrees to the right
	turnRight(147, degrees, 100);
	turnRobotTo(-90, 100);
	// Go forward and align the hubs against the wall
	forward(2.2, seconds, 50);
	// Realign
	moveMotor(sideDrive, 1, seconds, -100);
	// Back Up
	backward(0.5, rotations, 100);
	// Realign with wall
	moveMotor(sideDrive, 1, seconds, -100);
	// Forward again
	forward(1, seconds, 100);
	// Hook the hubs
	moveMotor(hookMotor, 0.8, seconds, -100);
	// Back up to 1 tile right of the 1x4 scoring zone
	moveRobot('b', 5, 100);
}

void score1HubFromRowof3(){
	// Start from end of pickUpTwoFromTheEightHubs
	// Realign with wall
	moveMotor(sideDrive, 1, seconds, -100);
	// Move away from wall to allow turn
	moveMotor(sideDrive, 0.7, rotations, 100);
	// Turn 45 degrees away from drivers
	turnLeft(147, degrees, 100);
	turnRobotTo(45, 100);
	// Back up to push 1 hub form row of 3 into 2x2 scoring zone
	forward(2.05, seconds, 100);
	// Back up
	moveRobot('b', 1.5, 100);
	// Move sideways to allow turning
	moveMotor(sideDrive, -0.5, rotations, 100);
	// Turn to face driver
	turnLeft(220, degrees, 100);
	turnRobotTo(180, 100);
}

void score2OrangeHubsAndYellowHub(){
	// Start from end of score2OrangeHubsAndYellowHub
	// Push hub forward to score
	forward(2, seconds, 100);
	// Release the hooks
	moveMotor(hookMotor, 0.6, rotations, 100);
	// Back up before dropping
	backward(0.2, rotations, 50);
	// Lower the arm
	moveArm(130, 'y');
	// Back Up
	backward(0.7, rotations, 20);
}


// PART 2 //

void pickUpLeftYellowHub(){
	// Start in left position
	// Lift arm to prepare to pick yellow hub
	moveArm(1025, 'n');
	// Move forward to not scrape wall
	moveRobot('f', 0.75, 100);
	// Move sideways to align with yellow hub
	moveMotor(sideDrive, 1.65, rotations, 100);
	// Back up to re-align with wall
	backward(0.5, seconds, 100);
	// Wait until arm raised so we don't knock off yellow hub
	waitUntilMotorMoveComplete(leftArm);
	// Move forward to yellow hub
	moveRobot('f', 1.5, 100);
	// Lift hub off pedestal
	moveArm(1370, 'y');
	// Back up away from structure
	backward(1, seconds, 100);
	// Move forward to allow turning
	forward(0.2, rotations, 50);
	// Turn 90 degrees, facing 1x4 scoring zone
	turnLeft(147, degrees, 100);
	turnRobotTo(90, 100);
	// Re-align with wall
	moveMotor(sideDrive, 0.5, rotations, 100);
}

void move2OrangeHubsTo2x2(){
	// Start from end of pickUpLeftYellowHub
	// Lower Arm
	moveArm(630, 'n');
	// Move away from wall
	moveMotor(sideDrive, -0.3, rotations, 100);
	// Turn
	turnRobotTo(55, 100);
	// Move sideways away from structure
	moveMotor(sideDrive, 0.2, rotations, 100);
	// Move forward to align with row of hubs
	forward(1.85, rotations, 100);
	// Turn to face hubs
	turnRobotTo(0, 100);
	// Move forward to group hubs
	forward(4, seconds, 50);
	// Lower hooks
	moveMotor(hookMotor, 1, seconds, -50);
	// Re-align with wall
	moveMotor(sideDrive, 1, seconds, -100);
	// Back up to avoid scraping on wall
	moveRobot('b', 0.5, 100);
	// Turn to face scoring zone
	turnLeft(147, degrees, 100);
	turnRobotTo(90, 100);
	// Re-align with the wall
	moveMotor(sideDrive, 1, seconds, -100);
		// Release the hooks
	moveMotor(hookMotor, 2, seconds, 100);
	// Move forward to score
	forward(2, seconds, 50);
	// Re-re-align with wall
	moveMotor(sideDrive, 1, seconds, -100);
}

void scoreLeftYellowHub(){
	// Start from end of move2OrangeHubsTo2x2
	// Lower the arm
	moveArm(130, 'y');
	// Back Up
	backward(0.7, rotations, 20);
}

// PART 3 //

void MoveScoringZone(){
	// Start from left position
	// Lift Arm
	moveArm(1370, 'y');
	// Move forward
	forward(1.15, rotations, 100);
	// Lower arm
	moveArm(200, 'y');
}

//* Main Task *//
task main()
{
	// Pre-program functions
	resetGyro(gyroSensor);
	iqCalibrateGyro();
	setTouchLEDColor(touchLED, colorViolet);
	resetMotorEncoder(rightArm);
	resetMotorEncoder(leftArm);

	// Wait to run 1st part of program (Start Right)
	waitUntil(getTouchLEDValue(touchLED) == true);
	resetGyro(gyroSensor);
	pickUpRightYellowHub();
	pickUpTwoFromTheEightHubs();
	score1HubFromRowof3();
	score2OrangeHubsAndYellowHub();

	// Wait to run 2nd part of program (Start Left)
	waitUntil(getTouchLEDValue(touchLED) == true);
	pickUpLeftYellowHub();
	move2OrangeHubsTo2x2();
	scoreLeftYellowHub();

	// Loop "Hang on Bar" (Part 3) Function (Start Left)
	while(true){
		// Wait until robot is back to starting position
		waitUntil(getTouchLEDValue(touchLED) == true);
		MoveScoringZone();
	}
}
