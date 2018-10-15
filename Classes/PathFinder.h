//Samuel MacSweeney
//K00200955

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "cocos2d.h"
#include "math.h"

USING_NS_CC;

class PathFinder
{
	public:

		enum pathType
		{
			four = 4,
			eight = 8
		};

		// Functions

		PathFinder(TMXTiledMap *tiledMap, pathType pathDirection);
		~PathFinder();

		void setLayer(std::vector<std::string> namedLayers);
		std::vector<Vec2> getObstaclePath(Vec2 beginningPosition, Vec2 endPosition, std::vector<int> GIDs);
		std::vector<Vec2> enemyPathFind;

	private:

		// Map Struct
		struct MapNode
		{
			Vec2 position;
			int beginningCost;
			int goalCost;
			int completeCost;
			MapNode *parentNode;

			MapNode()
			{
				beginningCost = 0;
				goalCost = 0;
				completeCost = 0;
				position = Vec2::ZERO;
			}

			inline Vec2 getNodePosition()
			{
				return position;
			}

			inline void setPositionX(int x)
			{
				position.x += x;
			}

			inline void setPositionY(int y)
			{
				position.y += y;
			}

			inline int getBeginningCost()
			{
				return beginningCost;
			}

			inline void setBeginningCost(int cost)
			{
				beginningCost += cost;
			}

			inline int getGoalCost()
			{
				return goalCost;
			}

			inline void setGoalCost(int cost)
			{
				goalCost = cost;
			}

			inline int getCompleteCost()
			{
				return completeCost;
			}

			inline void setCompleteCost()
			{
				completeCost = beginningCost + goalCost;
			}

			inline void setCompleteCost(int cost)
			{
				completeCost = cost;
			}

			inline MapNode *getParentNode()
			{
				return parentNode;
			}

			inline void setParentNode(MapNode* node)
			{
				parentNode = node;
			}
		};

		int getPathDistance(MapNode *beginning, MapNode *end);
		int manhattanDistance(MapNode *beginning, MapNode *end);
		std::vector<Vec2> getFoundPath(Vec2 beginningPosition, Vec2 endPosition, std::vector<int> impassableGIDs);

		TMXTiledMap *map;
		std::vector<TMXLayer*> layer;

		// Variables

		MapNode *createNode;
		MapNode *changedNode;
		MapNode *extractedNode;
		MapNode *beginningNode;
		MapNode *endNode;

		int direction;
		int cost;
		int minimum;
		int openCost;
		int closeCost;

		std::unordered_map<MapNode*, int> openNodeList;
		std::unordered_map<MapNode*, int> closedNodeList;

		bool nodeInOpen;
		bool nodeInClose;
};

#endif