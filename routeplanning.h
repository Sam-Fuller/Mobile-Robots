class routeplanning : public ArAction // Class action inherits from ArAction
{
public:
	routeplanning(); // Constructor
	virtual ~routeplanning() {}  // Destructor
	virtual ArActionDesired * fire(ArActionDesired d); // Body of the action
	ArActionDesired desiredState; // Holds state of the robot that we wish to action
protected:
	int currentcoord; //the current coord for an input loop
	int size; //the amount of variables in coords[] (will vary from map to map)
	int coords[500]; //an array for storing all map line coords

	int map[70][42]; //an array for storing the map and route (-1 for a wall, 0+ for the route)
};
