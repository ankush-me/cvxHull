#ifndef __FACE_DCEL_H__
#define __FACE_DCEL_H__


#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>
#include <utility>
#include "Data.h"
#include "utils/sorting.h"

class HalfEdge;

/** Represent a face of the sub-division.
 *  Since in convex-hull computations, faces are triangular,
 *  this representation is specialized for triangular faces. */
class Face : public boost::enable_shared_from_this<Face> {
public:

	typedef boost::shared_ptr<Face> Ptr;

	std::vector<Vector3dPtr> vertices;
	std::vector<boost::shared_ptr<HalfEdge> >  edges;
	bool toDelete;

	/** Initialize the edges of the face, given the three vertices. */
	void initEdges(Vector3dPtr pa, Vector3dPtr pb, Vector3dPtr pc);

	typedef boost::shared_ptr<Face> Ptr;

	Face (Vector3dPtr pa, Vector3dPtr pb, Vector3dPtr pc) : toDelete(false) {
		vertices.push_back(pa);
		vertices.push_back(pb);
		vertices.push_back(pc);

		initEdges(pa, pb, pc);
	}

	/** Constructs the face such that point T is behind the face.*/
	Face (Vector3dPtr pa, Vector3dPtr pb, Vector3dPtr pc, Vector3dPtr t) : toDelete(false) {
		vertices.push_back(pa);
		vertices.push_back(pb);
		vertices.push_back(pc);

		if (!isBehind(t))
			swap(vertices[1], vertices[2]);

		initEdges(pa, pb, pc);
	}

	/** Returns TRUE iff, PT is behind this face.*/
	bool isBehind(Vector3dPtr pt);

	void connect(Face::Ptr adjacent, Vector3dPtr a, Vector3dPtr b);
	void connect(boost::shared_ptr<HalfEdge> e);

	boost::shared_ptr<HalfEdge> getMatchingEdge(Vector3dPtr a, Vector3dPtr b);
	boost::shared_ptr<HalfEdge> getHorizonEdge();
};

#endif
