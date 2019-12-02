class avoidobstacle : public ArAction // Class action inherits from ArAction
{
public:
	avoidobstacle(); // Constructor
	void setup(bool * SuppressionIn);
	virtual ~avoidobstacle() {}  // Destructor
	virtual ArActionDesired * fire(ArActionDesired d); // Body of the action
	ArActionDesired desiredState; // Holds state of the robot that we wish to action
protected:
	int critDist; // Speed of the robot in mm/s
	int cooldown; //time before control is given back to other behaviours
	int stucktime; //time the robot has been stuck for
};
