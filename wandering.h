class wandering : public ArAction // Class action inherits from ArAction
{
public:
	wandering(); // Constructor
	void setup(bool * SuppressionIn);
	virtual ~wandering() {}  // Destructor
	virtual ArActionDesired * fire(ArActionDesired d); // Body of the action
	ArActionDesired desiredState; // Holds state of the robot that we wish to action
	

protected:
	int targetSpeed; //the target speed of the robot (200mm/s)
	int currentSpeed; //the current speed of the robot
	int maxDist; //the distance from an object before the robot wanders
	int remainingDist; //the distance before the next turn
	int lastDist; //the distance of the current 'leg'
	int lastTurn; //the angle of the last turn
};
