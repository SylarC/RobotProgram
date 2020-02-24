#pragma config(Sensor, port2,  gyroSensor,     sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  ArmBumper,      sensorVexIQ_Touch)
#pragma config(Sensor, port7,  touchLED,       sensorVexIQ_LED)
#pragma config(Motor,  motor1,          leftDrive,     tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor4,          leftArm,       tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor5,          sideDrive,     tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor6,          rightDrive,    tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
#pragma config(Motor,  motor10,         rightArm,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor11,         clawMotor,     tmotorVexIQ, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Ky Robot Celebratory Dance
void ClapClaw(int times){
	repeat(times){
		moveMotor(clawMotor, 0.2, rotations, 100);
		moveMotor(clawMotor, -0.2, rotations, 100);
	}
}

void LiftArm(int Direction){
	moveMotorTarget(leftArm, Direction*400, 100);
	moveMotorTarget(rightArm, Direction*400, 100);
}

/*

TouchLED Color Code:

Blue      == Tier 1
BlueGreen == Tier 2
Orange    == Tier 3
Red       == Tier 4

*/

// armPos 5 == lowered from yellow hub
int armPos = 1;

// Task to syncronize arm motors
task ArmMovement(){
	while(true){
		armControl(leftArm, BtnLUp, BtnLDown);
		armControl(rightArm, BtnLUp, BtnLDown);
	}
}

// Function to lower arm until bumper is pressed
void lowerArmUntilBumperPressed(){
	// Set Motors to run until bumper pressed
	setMotor(leftArm, -100);
	setMotor(rightArm, -100);
	waitUntil(getBumperValue(ArmBumper) == true);
	stopMultipleMotors(leftArm, rightArm);
}


int ClawStatus = 0;
// 0 = Open
// 1 = closed

// Task to move claw
task ClawMovement(){
	while(true){
		if(vexRT[BtnFUp] == true && ClawStatus == 0){
			moveMotor(clawMotor, -45, degrees, 20);
			ClawStatus = 1;
		}
		else if(vexRT[BtnFDown] == true && ClawStatus == 1){
			moveMotor(clawMotor, 45, degrees, 20);
			ClawStatus = 0;
		}
	}
}

// Function to stop/start tasks
void startLimbTasks(){
	startTask(ClawMovement);
	startTask(ArmMovement);
}

void stopLimbTasks(){
	stopTask(ClawMovement);
	stopTask(ArmMovement);
}




// Task for special commands
task BaseControl(){
	while(true){
		tankControl(ChD, ChA, 30);
		armControl(sideDrive, BtnRUp, BtnRDown);
	}
}

task main(){
	// Start tasks
	startLimbTasks();
	startTask(BaseControl);
	// Display program and run Claw Control
	while(true){
		displayTextLine(line1, "Tenacity TeleOp");
		// Function to lift hub to teir 3
		if(vexRT[BtnEDown] == true){
			armPos = 3;
			setTouchLEDColor(touchLED, colorBlueGreen);
			// Stop Arm Movement Task
			stopLimbTasks();
			// Lower arm until pumper pressed
			lowerArmUntilBumperPressed();
			// Move arm to stack/ init tilt
			moveMotorTarget(leftArm, 600, 100);
			moveMotorTarget(rightArm, 600, 100);
			waitUntilMotorMoveComplete(rightArm);
			// Restart Arm Movement Task
			waitUntilMotorMoveComplete(rightArm);
			startLimbTasks();
		}

		// Move Joysticks ( =>  +  =>) to lower arm from the yellow hub to be able to go under the bar
		else if (getJoystickValue(ChB) <= -50 && getJoystickValue(ChC) <= -50 && armPos == 4){
			playSound(soundTada);
			armPos = 5;
			stopLimbTasks();
			// Set LED color to Violet
			setTouchLEDColor(touchLED, colorViolet);
			// Lower motors without going all the way to go press the bumper
			moveMotorTarget(leftArm, -525, 100);
			moveMotorTarget(rightArm, -525, 100);
			// Add a "waitUntilMotorMoveComplete" as "moveMotorTarget" is not a blocking motor move
			waitUntilMotorMoveComplete(rightArm);
			startLimbTasks();
			wait(0.5, seconds);
		}
		// Move Joysticks ( =>  +  =>) to move arm all the way to yellow hub height
		else if(getJoystickValue(ChB) >= 50 && getJoystickValue(ChC) >= 50){
			armPos = 4;
			playSound(soundTollBooth);
			stopLimbTasks();
			lowerArmUntilBumperPressed();
			// Set LED color to Red
			setTouchLEDColor(touchLED, colorRed);
			// Lift arms all the way to yellow hub height
			moveMotorTarget(leftArm, 900, 100);
			moveMotorTarget(rightArm, 900, 100);
			// Add a "waitUntilMotorMoveComplete" as "moveMotorTarget" is not a blocking motor move
			waitUntilMotorMoveComplete(rightArm);
			startLimbTasks();
			wait(0.5, seconds);
		}
		// Move arm to floor
		else if(getJoystickValue(ChB) >= 50 && getJoystickValue(ChC) <= -50){
			stopTask(ArmMovement);
			lowerArmUntilBumperPressed();
			startTask(ArmMovement);
		}
		else if(getJoystickValue(ChB) <= -50 && getJoystickValue(ChC) >= 50){
			stopTask(ArmMovement);
			lowerArmUntilBumperPressed();
			moveMotorTarget(leftArm, 435, 100);
			moveMotorTarget(rightArm, 435, 100);
			// Restart Arm Movement after reached bottom
			waitUntilMotorMoveComplete(rightArm);
			startTask(ArmMovement);
		}
		else if(vexRT[BtnEUp] == true){
			wait(0.2, seconds);
			stopTask(BaseControl);
			stopLimbTasks();
			setTouchLEDColor(touchLED, colorRed);
			ClapClaw(2);
			LiftArm(1);
			setTouchLEDColor(touchLED, colorViolet);
			turnRight(0.35, rotations, 100);
			ClapClaw(3);
			turnLeft(0.7, rotations, 100);
			ClapClaw(2);
			turnRight(0.35, rotations, 100);
			LiftArm(-1);
			ClapClaw(1);
			playSound(soundTada);
			startLimbTasks();
			startTask(BaseControl);
		}
	}
}
