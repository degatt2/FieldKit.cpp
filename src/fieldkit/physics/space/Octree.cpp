/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 30/06/2010.
 */

#include "fieldkit/physics/space/Octree.h"

using namespace fieldkit::physics;

// -- Octree -------------------------------------------------------------------
Octree::Octree()
{
	init(Vec3f::zero(), Vec3f(100.0f, 100.0f, 100.0f), 10.0f);
}

Octree::Octree(Vec3f offset, Vec3f dimension, float minSize)
{
	init(offset, dimension, minSize);
}

Octree::~Octree() 
{
	root->destroy(ownsSpatials);
	delete root;
}

void Octree::init(Vec3f offset, Vec3f dimension, float minSize)
{
	// init bounds
	this->position = offset + dimension * 0.5f;
	this->extent = dimension * 0.5f;
	updateBounds();
	
	// init tree
	root = new Branch();
	root->init(offset, dimension, minSize, 0);
}

void Octree::clear() 
{
	root->clear();
}

void Octree::insert(Spatial* spatial) 
{
	root->insert(spatial);
}

void Octree::select(BoundingVolume* volume, SpatialListPtr result)
{
	result->clear();
	root->select(volume, result);
}


// -- Node ---------------------------------------------------------------------
void Octree::Node::init(Vec3f const& offset, Vec3f const& dimension, float minSize, int depth)
{
	this->offset = offset;
	this->minSize = minSize;
	this->depth = depth;
	
	isEmpty = true;
	
	// init bounds
	this->position = offset + dimension;
	this->extent = dimension;
	updateBounds();
}


// -- Branch -------------------------------------------------------------------
Octree::Branch::Branch() 
{
	type = TYPE_BRANCH;	
}

Octree::Branch::~Branch() 
{
	children.clear();
}

void Octree::Branch::init(Vec3f const& offset, Vec3f const& dimension, float minSize, int depth)
{
	Node::init(offset, dimension, minSize, depth);
	
	// create child nodes
	Vec3f halfSize = dimension * 0.5f;
	
	for(int octant=0; octant<8; octant++) {
		Vec3f o = offset;
		if((octant & 1) != 0) o.x += dimension.x;
		if((octant & 2) != 0) o.y += dimension.y;
		if((octant & 4) != 0) o.z += dimension.z;
	
		// create leaf nodes when we reached a certain minSize
		if(halfSize.x < minSize || halfSize.y < minSize || halfSize.z < minSize) {
			Leaf* leaf = new Leaf();
			leaf->init(o, halfSize, minSize, depth+1);
			children.push_back(leaf);
			
		// otherwise subdivide into further branches
		} else {
			Branch* branch = new Branch();
			branch->init(o, halfSize, minSize, depth+1);
			children.push_back(branch);	
		}
	}
}

void Octree::Branch::destroy( bool ownsSpatials )
{
	BOOST_FOREACH(Octree::NodePtr child, children) {
		child->destroy(ownsSpatials);
		delete child;
		child = NULL;
	}
	clear();
}


void Octree::Branch::clear() 
{
	if(isEmpty) return;
	
	BOOST_FOREACH(Octree::NodePtr child, children) {
		child->clear();
	}
	isEmpty = true;
}

void Octree::Branch::insert(Spatial* spatial)
{
	Vec3f p = spatial->getPosition();
	
	// check if point is inside box
	if(!contains(p)) return;
	
	isEmpty = false;
	
	int octant = getOctantID(p.x - offset.x, p.y - offset.y, p.z - offset.z);
	children[octant]->insert(spatial);
}

void Octree::Branch::select(BoundingVolume* volume, SpatialListPtr result)
{
	if(isEmpty) return;
	
	// check wether bounding volume and this node intersect at all
	if(!intersects(volume)) return;
	
	// traverse all children until we find a leaf node
	BOOST_FOREACH(Octree::NodePtr child, children) {
		child->select(volume, result);
	}	
}

int Octree::Branch::getOctantID(float x, float y, float z) 
{
	int id = 0;
	if(x >= extent.x) id += 1;
	if(y >= extent.y) id += 2;
	if(z >= extent.z) id += 4;
	return id;
}


// -- Leaf ---------------------------------------------------------------------
Octree::Leaf::Leaf() 
{
	type = TYPE_LEAF;
}

Octree::Leaf::~Leaf() 
{
	// assumes destroy was called before!
}

void Octree::Leaf::destroy(bool ownsSpatials)
{
	if(ownsSpatials) {
		BOOST_FOREACH(Spatial* spatial, data) {
			if(spatial != NULL) {
				delete spatial;
				spatial = NULL;
			}
		}
	}

	clear();
}

void Octree::Leaf::clear() 
{
	if(isEmpty) return;
	
	data.clear();
	isEmpty = true;
}

void Octree::Leaf::insert(Spatial* spatial)
{		
	// check if point is inside box
	if(!contains(spatial->getPosition())) return;
	
	isEmpty = false;
	
	// insert spatial into data
	data.push_back(spatial);
}

void Octree::Leaf::select(BoundingVolume* volume, SpatialListPtr result)
{
	if(isEmpty) return;
	
	// check wether bounding volume and this node intersect at all
	if(!intersects(volume)) return;
	
	BOOST_FOREACH(Spatial* s, data) {
		if(volume->contains(s->getPosition()))
			result->push_back(s);	
	}
}