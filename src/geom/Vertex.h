#ifndef __VERTEX_H__
#define __VERTEX_H__


#include <Eigen/Dense>
#include <boost/shared_ptr.hpp>
#include "Data.h"

class ConflictList;

class Vertex {
public:
	bool visible;
	typedef boost::shared_ptr<Vertex> Ptr;
	Vector3dPtr pt;
	boost::shared_ptr<ConflictList> cList;

	Vertex() : visible(true) {}
};


#endif
