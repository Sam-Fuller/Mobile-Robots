// Signatures

class follow : public ArAction // Class action inherits from ArAction
{
public:
	follow(); // Constructor
	void setup(bool * SuppressionIn);
	virtual ~follow() {}  // Destructor
	virtual ArActionDesired * fire(ArActionDesired d); // Body of the action
	ArActionDesired desiredState; // Holds state of the robot that we wish to action
protected:
	int speed; // target speed of the robot in mm/s
	double deltaHeading; 

	//following distances (so they can be edited if needed)
	double followDist; 
	double maxFollowDist;
	double minFollowDist;
	
	//variables for calculating the RMSE and storing previous Errors
	int walltimer; 
	double RMSE10[10];
	double RMSEall[10000];
	double RMSE10ans;
	double RMSEallans;

	//variables used in calculating and storing angles
	double desiredAngle; 
	double actualAngle;
	double deltaAngle;

	//variables for storing sensors, distance, and errors relating to the left
	double leftFront;
	double leftBack;
	double leftDist;
	double leftError;

	//variables for storing sensors, distance, and errors relating to the left
	double rightFront;
	double rightBack;
	double rightDist;
	double rightError;


