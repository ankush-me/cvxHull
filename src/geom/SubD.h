#ifndef __SUBD_H__
#define __SUBD_H__

/** A collection of vertices, edges, and facets stored in an object-
 *  oriented version of a DCEL (doubly connected edge list).*/

#include <vector>
#include "Data.h"
#include "Vertex.h"
#include "Face.h"

class SubDivision {
public:
	std::vector<Vertex::Ptr> vertices;
	std::vector<Face::Ptr> faces;

	SubDivision();
	void removeVertex(Vertex::Ptr p);
	void removeFace(Face::Ptr f);
};

#endif
