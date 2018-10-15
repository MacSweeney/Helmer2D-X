//Samuel MacSweeney
//K00200955

#include "PathFinder.h"


// PathFinder Constructor
PathFinder::PathFinder(TMXTiledMap *tiledMap, pathType pathDirection)
{
	// Initialize map
	map = tiledMap;

	// Initialize pathfinding method
	direction = pathDirection;

	// Next Node in list 
	cost = 0;

	// Used to set all Layers in game
	setLayer({ });

	// Used as new node on map - usually first node during pathfind
	extractedNode = new MapNode();
};

// PathFinder Destructor 
PathFinder::~PathFinder()
{
	// Delete all open nodes in the openNode list
	for (auto openNode : openNodeList)
	{
		delete openNode.first;
	}

	// Delete all closed nodes within the closed nodes list
	for (auto closeNode : closedNodeList)
	{
		delete closeNode.first;
	}

	// Delete all closed, open and layer data
	openNodeList.clear();
	closedNodeList.clear();
	layer.clear();
};

// Function for Setting neccessary layers within game
void PathFinder::setLayer(std::vector<std::string> namedLayers)
{
	// Clear any layers from before
	layer.clear();

	// If namedLayers is empty add all layers
	if (namedLayers.size() == 0)
	{
		for (auto &nodeLayer : map->getChildren())
		{
			/*
				Dynamic_cast in C++ can be used to perform type safe down casting 
				Dynamic_cast is run time polymorphism. The dynamic_cast operator 
				Which safely converts from a pointer (or reference) to a base type to 
				A pointer (or reference) to a derived type
			*/
			TMXLayer *mapLayer = dynamic_cast<TMXLayer*>(nodeLayer);

			if (mapLayer)
			{
				// Add to layers
				layer.push_back(mapLayer);
			}
		}
	}

	else
	{
		// If layers are specified then add specified layers 
		for (auto namedLayer : namedLayers)
		{
			TMXLayer *mapLayer = map->getLayer(namedLayer);

			if (mapLayer)
			{
				layer.push_back(mapLayer);
			}
		}
	}
};

// Function to find a path based around avoiding obstacle GIDs 
std::vector<Vec2> PathFinder::getObstaclePath(Vec2 beginningPosition, Vec2 endPosition, std::vector<int> GIDs)
{
	// Get a path based around not hitting obstacle tiles
	return getFoundPath(beginningPosition, endPosition, GIDs);
};

// Function to use Manhattan Distance formula to calculate a path based on start and finish nodes
int PathFinder::getPathDistance(MapNode *beginning, MapNode *end)
{
	return manhattanDistance(beginning, end);
};

// Manhattan Distance formula
int PathFinder::manhattanDistance(MapNode *beginning, MapNode *end)
{
	int dx = end->getNodePosition().x - beginning->getNodePosition().x;
	int dy = end->getNodePosition().y - beginning->getNodePosition().y;
	int pd = abs(dx) + abs(dy);

	return pd;
};

// Function to grab the path that has been found based on data so far
std::vector<Vec2> PathFinder::getFoundPath(Vec2 beginningPosition, Vec2 endPosition, std::vector<int> impassableGIDs)
{
	// Clear any nodes that are still in the open/closed list
	openNodeList.clear();
	closedNodeList.clear();

	// endNode is the Finish point of path
	endNode = new MapNode();

	// X and Y coords for endNode
	endNode->setPositionX(endPosition.x);
	endNode->setPositionY(endPosition.y);

	// Start node for Path Found
	beginningNode = new MapNode();

	// X and Y for Start of path
	beginningNode->setPositionX(beginningPosition.x);
	beginningNode->setPositionY(beginningPosition.y);

	// Clear previous cost node if any - cost node is usually second node in path - first is enemy current Position
	beginningNode->setBeginningCost(0);

	// Calculate the distance between the start node and end node
	int pathCost = getPathDistance(beginningNode, endNode);

	// Goal cost is the distance between the start node and end node
	beginningNode->setGoalCost(pathCost);

	/*
		Emplace - This effectively increases the container size by one.

		An automatic reallocation of the allocated storage space happens if 
		- and only if -	
		the new vector size surpasses the current vector capacity.
	*/
	// Add The begining node and the complete cost of the path to the open node list
	// Open node list consists of all nodes that can be added to path
	openNodeList.emplace(beginningNode, beginningNode->getCompleteCost());

	while (openNodeList.size() != 0)
	{

		/*
			An int holds [-32767, +32767], but it is permitted to hold more
			Minimum range is used to prevent the possibility of integer overflow
			Which causes "undefined behaviour"
		*/
		// Initialize minimum value
		minimum = 32767;

		// Minimum node is lowest possible node in path - The Starting Node - Enemy Current Location on map
		MapNode *minimumNode;

		// Iteration through the openNodeList
		for (auto a : openNodeList)
		{
			// Extracted Node is the Fist node in the list - The Starting Node - Enemy Current Location on map
			extractedNode = a.first;

			// Cost is the second node on the list, this is what it will cost to move to the next node on the path 
			// - IS the next node on the path
			cost = a.second;

			// Check if the cost is less then the minimum value to ensure the prevention integer overflow
			if (cost < minimum)
			{
				// As long as the cost is valid it becomes the new minium for this path
				minimum = cost;

				// Minmum node is the starting point and becomes the extracted node - Fist node in list
				minimumNode = extractedNode;
			}
		}

		// Minimum node is then erased in order to prevent memory issues - At this point enemy path should have updated
		// Past the initial starting Node so it's safe to clear it
		extractedNode = minimumNode;
		openNodeList.erase(minimumNode);

		// Check to see if the path is at the Final Node
		if (extractedNode->getNodePosition() == endNode->getNodePosition())
		{
			// Create a vector that holds the path position
			std::vector<Vec2> pathPositions;

			// Add Starting node to the path position and get it's position on map
			pathPositions.push_back(extractedNode->getNodePosition());

			// Path Size now equals the begining cost - Still just cost though
			int pathSize = extractedNode->getBeginningCost();

			// Increase pathSize by adding each node in the path 
			for (int i = 0; i < pathSize; i++)
			{
				// Pushing in each X and Y coord to the pathPosition Vector
				pathPositions.push_back(Vec2(extractedNode->getParentNode()->getNodePosition().x, 
					extractedNode->getParentNode()->getNodePosition().y));
				extractedNode = extractedNode->getParentNode();
			}

			// The Path is initially Reversed Here 
			// In order to make the Enemy Move in the Correct direction flip the vector around
			// End Node is now Start Node - End Node is the Enemy Position
			std::reverse(pathPositions.begin(), pathPositions.end());

			// Return the PathPositions 
			return pathPositions;
		}

		else
		{
			// Set Movement Based on the Direction type 
			// - Used to detemermin how enemies will be able to move in game
			for (int i = 0; i < direction; i++)
			{
				// Open cost is used to determine the amount of valid nodes
				openCost = 0;

				// Close Cost is the amount of invalid nodes 
				closeCost = 0;

				// Is node Valid
				nodeInOpen = false;

				// Is node Invalide
				nodeInClose = false;

				// Create a node that is valid on the current Map
				createNode = new MapNode();

				// Set the X value for a node
				createNode->setPositionX(extractedNode->getNodePosition().x);

				// Set the Y value for a node
				createNode->setPositionY(extractedNode->getNodePosition().y);
				
				// Swith case to set positions for directions path can take
				switch (i)
				{
					case 0:

						// Move left
						createNode->setPositionX(-1);
						createNode->setPositionY(0);

						break;

					case 1:

						// Move Right
						createNode->setPositionX(1);
						createNode->setPositionY(0);

						break;

					case 2:

						// Move Up
						createNode->setPositionX(0);
						createNode->setPositionY(1);

						break;

					case 3:

						// Move Down
						createNode->setPositionX(0);
						createNode->setPositionY(-1);

						break;

					case 4:

						// Move Up & Left
						createNode->setPositionX(-1);
						createNode->setPositionY(1);

						break;

					case 5:

						// Move Down & left
						createNode->setPositionX(-1);
						createNode->setPositionY(-1);

						break;

					case 6:

						// Move Right & Down
						createNode->setPositionX(1);
						createNode->setPositionY(-1);

						break;

					case 7:

						// Move Right & Up
						createNode->setPositionX(1);
						createNode->setPositionY(1);

						break;
				}

				// Check to see if node position is not the last nodes position
				if (createNode->getNodePosition() != endNode->getNodePosition())
				{
					// Check to see if the Node contains a valid value
					if (createNode->getNodePosition().x < 0 
						|| createNode->getNodePosition().y < 0 
						|| createNode->getNodePosition().x >= map->getMapSize().width 
						|| createNode->getNodePosition().y >= map->getMapSize().height)
					{
						// If node is valid continue looking for end node
						continue;
					}

					// If end node is reached stop looking for path
					bool continuePathFinding = false;

					// Set up GID collision detection
					// Check through each added layer 
					for (auto mapLayer : layer)
					{
						// Check through each tile marked as collidable
						for (int GID : impassableGIDs)
						{
							// Check where the Position of the collidable tile is
							if (mapLayer->getTileGIDAt(createNode->getNodePosition()) == GID)
							{
								// If the Path is still being calculated - Stop
								continuePathFinding = false;

								// Break as pathFind has hit a wall 
								break;
							}
						}
					}

					// If no issues path Finding Continue pathfinding
					if (continuePathFinding)
					{
						continue;
					}
				}

				// Path Cost is now the distance between the Start and end node
				// How far is this path
				pathCost = getPathDistance(createNode, endNode);

				// Set the Begining node to the next node - Move along path
				createNode->setBeginningCost(extractedNode->getBeginningCost() + 1);

				// The distance to the end node is the path cost
				createNode->setGoalCost(pathCost);

				// The Parent node is Start node - Path Expands from this point
				createNode->setParentNode(extractedNode);

				// Complete cost is the entire path distance
				createNode->setCompleteCost();

				nodeInOpen = false;
				nodeInClose = false;

				// Iterate through the openNodes list
				for (auto a : openNodeList)
				{

					// Get the fist node in the list
					MapNode *node = a.first;

					// Check that this node is open
					if (createNode->getNodePosition() == node->getNodePosition())
					{
						openCost = node->getCompleteCost();
						nodeInOpen = true;
						break;
					}
				}

				// Iterate through the closedNodeList
				for (auto a : closedNodeList)
				{
					// Get the first node in the list
					MapNode * node = a.first;

					// Check that this node is closed
					if (createNode->getNodePosition() == node->getNodePosition())
					{
						closeCost = node->getCompleteCost();
						nodeInClose = true;
						break;
					}
				}

				// Check if nodes are greater then the cost 
				if ((nodeInOpen && (createNode->getCompleteCost() >= openCost)) 
					|| (nodeInClose && (createNode->getCompleteCost() >= closeCost)))
				{
					// Keep checking until they are all less then the cost
					continue;
				}
				else
				{
					// Delete all closed nodes 
					if (nodeInClose)
					{
						closedNodeList.erase(createNode);

						// If a closed node from the Closed list is actually a valid node add to the openNodeList
						openNodeList.emplace(createNode, createNode->getCompleteCost());
					}


					if (nodeInOpen)
					{
						// Cost for open nodes equals the cost
						cost = openCost;

						// Move new nodes into the Changed nodes 
						// changing nodes here allows me to delete the data for the 
						//last created node and update it with new data
						changedNode = createNode;

						// Erase unessessary data for reuse of variable
						openNodeList.erase(createNode);

						// Initialise path cost for changed node
						changedNode->setCompleteCost(createNode->getCompleteCost());

						// Add this data to the openNode list which is now the path
						openNodeList.emplace(changedNode, changedNode->getCompleteCost());
					}
					else
					{
						// Add this data to the openNode list which is now the path
						// Ensuring node is added to path
						openNodeList.emplace(createNode, createNode->getCompleteCost());
					}
				}
			}
		}

		// Add the extracted node to closed nodes list - clean up
		closedNodeList.emplace(extractedNode, extractedNode->getCompleteCost());
	}

	// Return the Valid Shortest Path
	return enemyPathFind;
};