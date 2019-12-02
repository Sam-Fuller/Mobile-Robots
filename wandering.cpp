#include <iostream>
#include <stdlib.h>
#include <Aria.h>

#include "wandering.h"
// Implementation

// Constructor
wandering::wandering() : ArAction("wandering")
{
	maxDist = 1700;
	targetSpeed = 200;
	remainingDist = (rand() % 1000) + 500; //in mm
}

//pointer to the suppression boolean
bool *wanderSuppression;
void wandering::setup(bool *SuppressionIn) {
	wanderSuppression = SuppressionIn;
}

ArActionDesired * wandering::fire(ArActionDesired d)
{
	desiredState.reset();
	currentSpeed = myRobot->getVel();

	//if this behaviour is not being suppressed
	if (*wanderSuppression == false) {

		//if the closest sonar reports higher than the max distance
		if (myRobot->getClosestSonarRange(-179, 179) > maxDist) {
			//if the remaining distance has been reached
			if (remainingDist < 0) {
				lastTurn = (rand() % 280) - 140; //create a new turn angle
				myRobot->setDeltaHeading(lastTurn); //turn by that angle
				lastDist = remainingDist = (rand() % 1000) + 500; //create a new remainingDist (and set lastDist too)
				system("CLS"); //outputs
				printf("Current State: Turning\n");
				printf("Nearest Object: %i\n", myRobot->getClosestSonarRange(-179, 179));
				printf("x: %f     y: %f     th: %f     vel: %f\n", myRobot->getX(), myRobot->getY(), myRobot->getTh(), myRobot->getVel());
				printf("Total Distance to Travel: %i     Distance Until Next Turn: %i\n", lastDist, remainingDist);
				printf("Last Turn was %i degrees", lastTurn);
			}
			else {
				remainingDist = remainingDist - (currentSpeed / 10); //remove from the remaining distance the distance traveled by the robot (d = s * t) time is 1/10s
				system("CLS"); //outputs
				printf("Current State: Wander - Straight\n");
				printf("Nearest Object: %i\n", myRobot->getClosestSonarRange(-179, 179));
				printf("x: %f     y: %f     th: %f     vel: %f\n", myRobot->getX(), myRobot->getY(), myRobot->getTh(), myRobot->getVel());
				printf("Total Distance to Travel: %i     Distance Until Next Turn: %i\n", lastDist, remainingDist);
				printf("Last Turn was %i degrees", lastTurn);
			}
		}

		if (currentSpeed < targetSpeed) //whilst below target speed
			desiredState.setVel(currentSpeed + 10); //speed up

		;
	}

	return &desiredState;
}
