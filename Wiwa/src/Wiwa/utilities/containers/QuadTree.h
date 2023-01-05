#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/AABB.h>
#include <Wiwa/ecs/EntityManager.h>

//TODO: Research topic
namespace Wiwa {
	struct Point {
		int x;
		int y;
		Point(int _x, int _y)
		{
			x = _x;
			y = _y;
		}
		Point()
		{
			x = 0;
			y = 0;
		}
	};
	struct Node {
		Point pos;
		int data;
		Node(Point _pos, int _data)
		{
			pos = _pos;
			data = _data;
		}
		Node() { data = 0; }
	};
	class WI_API QuadTree
	{
	public:
		QuadTree();
		~QuadTree();

		void Create(class AABB limits);
		void Clear();
		void Insert(EntityId entity);
		void Remove(EntityId entity);
	private:
		std::vector<EntityId> entities;

		// Hold details of the boundary of this node
		Point topLeft;
		Point botRight;

		// Contains details of node
		Node* n;

		// Children of this tree
		QuadTree* topLeftTree;
		QuadTree* topRightTree;
		QuadTree* botLeftTree;
		QuadTree* botRightTree;
	};
}