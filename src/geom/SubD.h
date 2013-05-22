#ifndef __SUBD_H__
#define __SUBD_H__

/** A collection of vertices, edges, and facets stored in an object-
 *  oriented version of a DCEL (doubly connected edge list).*/

#include <vector>
#include "Data.h"

class SubDivision {

	std::vector<Vertex::Ptr> vertices;
	std::vector<Face::Ptr> faces;

	SubDivision() : vertices(), faces() {}

	void removeVertex(Vertex::Ptr p) {
		int i=0;
		while (i < vertices.size()) {
			if (*(vertices[i]->pt )== *(p->pt)) break;
			i += 1;
		}
		if (i< vertices.size()) vertices.erase(vertices.begin()+i);
	}

	void removeFace(Face::Ptr f) {
		int i=0;
		while (i < faces.size()) {
			if (faces[i].get() == f.get()) break;
			i += 1;
		}
		if (i< faces.size()) faces.erase(faces.begin()+i);

	}

};

#endif
