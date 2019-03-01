#pragma config(Sensor, port2,  gyroSensor,     sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  bumperSensor,   sensorVexIQ_Touch)
#pragma config(Sensor, port7,  touchLED,       sensorVexIQ_LED)
#pragma config(Motor,  motor1,          leftDrive,     tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor4,          leftArm,       tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor5,          sideDrive,     tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor6,          rightDrive,    tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
#pragma config(Motor,  motor10,         rightArm,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor11,         hookMotor,     tmotorVexIQ, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Task to syncronize arm motors
task ArmMovement(){
	while(true){
		armControl(leftArm, BtnRUp, BtnRDown);
		armControl(rightArm, BtnRUp, BtnRDown);
	}
}

// Task to move claw
task HookMovement(){
	while(true){
		armControl(hookMotor, BtnLDown, BtnLUp);
	}
}

task BaseControl(){
	while(true){
		tankControl(ChD, ChA, 30);
		armControl(sideDrive, BtnEUp, BtnFUp);
	}
}

task ExtraFunctions(){
	while(true){
		if(vexRT[BtnFDown] == true){
			stopTask(ArmMovement);
			setMotor(leftArm, -100);
			setMotor(rightArm, -100);
			waitUntil(getBumperValue(bumperSensor) == true);
			stopMultipleMotors(leftArm, rightArm);
			startTask(ArmMovement);
		}
	}
}

// Function to stop/start tasks
void startLimbTasks(){
	startTask(HookMovement);
	startTask(ArmMovement);
}


// Task for special commands

task main(){
	// Start tasks
	startLimbTasks();
	startTask(BaseControl);
	startTask(ExtraFunctions);
	while(true){
		// Display program and run Claw Control
		displayTextLine(line1, "Mushu TeleOp");
	}
}
