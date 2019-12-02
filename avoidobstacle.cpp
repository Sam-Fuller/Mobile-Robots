#include <iostream>
#include <stdlib.h>
#include <Aria.h>

#include "avoidobstacle.h"


avoidobstacle::avoidobstacle() : ArAction("Avoid Obstacle")
{
	critDist = 500;
}

bool *avoidSuppression;
void avoidobstacle::setup(bool *SuppressionIn) {
	avoidSuppression = SuppressionIn;
}

// Body of action
ArActionDesired * avoidobstacle::fire(ArActionDesired d)
{
	desiredState.reset(); // reset the desired state (must be done)
	//desiredState.setVel(myRobot->getVel() + 10);

	int closestObject = myRobot->getClosestSonarRange(-40, 40); //get the sonars from front
	int closestSonar = myRobot->getClosestSonarNumber(-40, 40); //get the sonar number
	if (closestObject < critDist) //if an object is within critical distance
	{
		*avoidSuppression = true; //suppress all other behaviours
		cooldown = 20;
		stucktime+=10; //increase the amount of time the robot has been stuck

		if (stucktime < 200) {
			if (myRobot->getClosestSonarRange(-10, 10) <= (critDist / 2)) {
				desiredState.setVel(-25); //if the object is directly ahead reverse
			}
			else {
				desiredState.setVel(100); //else just slow down
			}

			if (closestSonar <= 3) {
				myRobot->setDeltaHeading(-25); //if the closest object is to the left turn right
				
			}
			else if (closestSonar >= 3) {
				myRobot->setDeltaHeading(25); //if the closest object is to the right turn left
			}
		}
		else {
			myRobot->setDeltaHeading(180); //if the robot has been stuck for a long time turn around
			stucktime = 0;
		}

		system("CLS"); //output
		printf("Current State: AvoidObstacle - active\n");
		printf("Nearest Object: %i\n", myRobot->getClosestSonarRange(-179, 179));
		printf("x: %f     y: %f     th: %f     vel: %f\n", myRobot->getX(), myRobot->getY(), myRobot->getTh(), myRobot->getVel());
	}
	else {
		if (cooldown <= 0) {
			*avoidSuppression = false; //unsuppress all other behaviours
			stucktime = 0;
		}
		else {
			cooldown--; //robot has just come out of being stuck (dont unsuppress other behaviours until the robot has had time to turn and back away)
			stucktime > 0 ? stucktime-=2 : false ;

			system("CLS"); //output
			printf("Current State: AvoidObstacle - cooldown\n");
			printf("Nearest Object: %i\n", myRobot->getClosestSonarRange(-179, 179));
			printf("x: %f     y: %f     th: %f     vel: %f\n", myRobot->getX(), myRobot->getY(), myRobot->getTh(), myRobot->getVel());
		}
	}

	return &desiredState; // give the desired state to the robot for actioning
}
