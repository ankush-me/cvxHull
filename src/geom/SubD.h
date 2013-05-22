/** A collection of vertices, edges, and facets stored in an object-
 *  oriented version of a DCEL (doubly connected edge list).*/

#include <vector>
#include "Data.h"

class SubDivision {

	std::vector<Vector3dPtr> vertices;
	std::vector<Face> faces;

	SubDivision() : vertices(), faces() {}

	void removeVertex(Vector3dPtr p) {
		int i=0;
		while (i < vertices.size()) {
			if (*vertices[i] == *p) break;
			i += 1;
		}
		if (i< vertices.size()) vertices.erase(vertices.begin()+i);
	}

	void removeFace(Face::Ptr f) {
		int i=0;
		while (i < faces.size()) {
			if (faces[i] == f) break;
			i += 1;
		}
		if (i< faces.size()) faces.erase(faces.begin()+i);

	}

};
