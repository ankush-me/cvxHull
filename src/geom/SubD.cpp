/** A collection of vertices, edges, and facets stored in an object-
 *  oriented version of a DCEL (doubly connected edge list).*/
#include "SubD.h"

SubDivision::SubDivision() : vertices(), faces() {}

void SubDivision::removeVertex(Vertex::Ptr p) {
	int i=0;
	while (i < vertices.size()) {
		if (*(vertices[i]->pt )== *(p->pt)) break;
		i += 1;
	}
	if (i< vertices.size()) vertices.erase(vertices.begin()+i);
}

void SubDivision::removeFace(Face::Ptr f) {
	int i=0;
	while (i < faces.size()) {
		if (faces[i].get() == f.get()) break;
		i += 1;
	}
	if (i< faces.size()) faces.erase(faces.begin()+i);
}

