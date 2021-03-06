#pragma config(Motor,  motor1,          rightDrive,    tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
#pragma config(Motor,  motor2,          fanMotor,      tmotorNone, openLoop)
#pragma config(Motor,  motor4,          rightArm,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor8,          leftArm,       tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor12,         leftDrive,     tmotorVexIQ, PIDControl, driveLeft, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task driveBase(){
	while(true){
		tankControl(ChD, ChA, 10);
	}
}

task fanControl(){
	while(true){
		armControl(fanMotor, BtnLUp, BtnLDown);
	}
}

task RV4BControl(){
	while(true){
		armControl(rightArm, BtnRDown, BtnRUp);
		armControl(leftArm, BtnRUp, BtnRDown);
	}
}

task main()
{
	// Start tasks
	startTask(driveBase);
	startTask(fanControl);
	startTask(RV4BControl);
	// Screen display
	while(true){
		displayTextLine(1, "Prototype");
	}
}
