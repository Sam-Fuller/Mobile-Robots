#include <iostream>
#include <stdlib.h>
#include <Aria.h>

#include "follow.h"
// Implementation

// Constructor
follow::follow() : ArAction("Follow Edge")
{
	speed = 200; // Set the robots speed to 50 mm/s. 200 is top speed
	deltaHeading = 0; // Straight line

	followDist = 1000;
	maxFollowDist = 1500;
	minFollowDist = 500;

	walltimer = 0;
}

double distance(double ar, double br) {
	double ab = sqrt(ar * ar + br * br - 2*ar*br*cos(80 * M_PI/180));
	double sinbar = br * (sin(M_PI / 3) / ab);
	double sincar = sinbar;
	double rc = sincar * ar;

	return rc;
}

double angle(double ar, double br) {
	double ab = sqrt(ar * ar + br * br - 2*ar*br*cos(80 * M_PI / 180));
	double sinbar = br * (sin(M_PI/3)/ ab);
	double bar = asin(sinbar) * (180 / M_PI);
	double arc = 90 - bar;

	return -(arc - 50);
}

//pointer to the suppression boolean
bool *followSuppression;
void follow::setup(bool *SuppressionIn) {
	followSuppression = SuppressionIn;
}

// Body of action
ArActionDesired * follow::fire(ArActionDesired d)
{
	desiredState.reset(); // reset the desired state (must be done)
	
	//if behaviour is not being suppressed
	if (*followSuppression == false) {
		leftFront = myRobot->getSonarRange(1) + myRobot->getRobotRadius(); //get the data from the sonars
		leftBack = myRobot->getSonarRange(14) + myRobot->getRobotRadius();

		leftDist = distance(leftFront, leftBack); //calculate distance from the wall

		if ((leftDist < maxFollowDist)) {
			leftError = leftDist - followDist; //error is calculated from the distances
			desiredState.setVel(speed);
		}
		else {
			leftError = 100000; //number that will never be reached to signal no wall found in range
		}

		rightFront = myRobot->getSonarRange(6) + myRobot->getRobotRadius(); //same as above but for the right
		rightBack = myRobot->getSonarRange(9) + myRobot->getRobotRadius();

		rightDist = distance(rightFront, rightBack);

		if ((rightDist < maxFollowDist)) {
			rightError = rightDist - followDist;
			desiredState.setVel(speed);
		}
		else {
			rightError = 100000;
		}

		if (abs(leftError) < abs(rightError) && (leftError != 100000 || rightError != 100000)) { //for left edge following
			desiredAngle = (leftError / 500) * 45; //calculate the desired angle from the error
			if (myRobot->getSonarRange(3) < (maxFollowDist+1000)) {
				desiredAngle = desiredAngle - (((maxFollowDist + 1000) - myRobot->getSonarRange(3)) / 1000) * 45; //scale the desired angle if there is a wall ahead
			}
			actualAngle = angle(leftFront, leftBack);  //calculate the actual angle
			deltaAngle = desiredAngle - actualAngle; //delta angle calculated
			myRobot->setDeltaHeading(deltaAngle); //angle set

			
			RMSE10 [walltimer % 10] = leftError; //store the error in the RMSE arrays
			RMSEall [walltimer] = leftError;

			RMSE10ans = 0;
			if (walltimer > 10) { //calculate the RMSE for the last 10 errors
				for (int i = 0; i < 10; i++) {
					RMSE10ans += (RMSE10[i] * RMSE10[i]);
				}
				RMSE10ans /= 10;
				RMSE10ans = sqrt(RMSE10ans);
			}

			RMSEallans = 0;
			for (int i = 0; i <= walltimer; i++) { //calculate the RSME for all errors
				RMSEallans += (RMSEall[i] * RMSEall[i]);
			}
			RMSEallans /= walltimer;
			RMSEallans = sqrt(RMSEallans);
			
			walltimer++;

			system("CLS"); //outputs
			printf("Current State: Following Wall - left\n");
			printf("Nearest Object: %i\n", myRobot->getClosestSonarRange(-179, 179));
			printf("x: %f     y: %f     th: %f     vel: %f\n", myRobot->getX(), myRobot->getY(), myRobot->getTh(), myRobot->getVel());
			printf("RMSE: %f     RMSE of last 10: %f", RMSEallans, RMSE10ans);
		}
		else if (abs(leftError) >= abs(rightError) && (leftError != 100000 || rightError != 100000)) { //for right edge following (same as above but for the right)
			desiredAngle = (rightError / 500) * 45;
			if (myRobot->getSonarRange(4) < (maxFollowDist + 1000)) {
				desiredAngle = desiredAngle - (((maxFollowDist + 1000) - myRobot->getSonarRange(4)) / 1000) * 45;
			}
			actualAngle = angle(rightFront, rightBack);
			deltaAngle = actualAngle - desiredAngle;
			myRobot->setDeltaHeading(deltaAngle);

			walltimer++;

			RMSE10[walltimer % 10] = rightError;
			RMSEall[walltimer] = rightError;

			RMSE10ans = 0;
			if (walltimer > 10) {
				for (int i = 0; i < 10; i++) {
					RMSE10ans += (RMSE10[i] * RMSE10[i]);
				}
				RMSE10ans /= 10;
				RMSE10ans = sqrt(RMSE10ans);
			}

			RMSEallans = 0;
			for (int i = 0; i <= walltimer; i++) {
				RMSEallans += (RMSEall[i] * RMSEall[i]);
			}
			RMSEallans /= walltimer;
			RMSEallans = sqrt(RMSEallans);

			walltimer++;

			system("CLS");
			printf("Current State: Following Wall - right\n");
			printf("Nearest Object: %i\n", myRobot->getClosestSonarRange(-179, 179));
			printf("x: %f     y: %f     th: %f     vel: %f\n", myRobot->getX(), myRobot->getY(), myRobot->getTh(), myRobot->getVel());
			printf("RMSE: %f     RMSE of last 10: %f", RMSEallans, RMSE10ans);
		}
		else { //for no edge following
			desiredAngle = 0;
			actualAngle = 0;
			deltaAngle = 0;
		}

		

		 // Set the heading change of the robot
	}

	return &desiredState; // give the desired state to the robot for actioning
}
