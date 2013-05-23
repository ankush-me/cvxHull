#ifndef __FACE_DCEL_H__
#define __FACE_DCEL_H__


#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>
#include <utility>
#include "Data.h"
#include "ConflictGraph.h"

class HalfEdge;

/** Represent a face of the sub-division.
 *  Since in convex-hull computations, faces are triangular,
 *  this representation is specialized for triangular faces. */
class Face : public boost::enable_shared_from_this<Face> {
public:
	typedef boost::shared_ptr<Face> Ptr;

	std::vector<Vertex::Ptr> vertices;
	std::vector<boost::shared_ptr<HalfEdge> >  edges;

	bool toDelete;
	ConflictList::Ptr cList;



	/** Initialize the edges of the face, given the three vertices. */
	void initEdges(Vertex::Ptr pa, Vertex::Ptr pb, Vertex::Ptr pc);


	Face (Vertex::Ptr pa, Vertex::Ptr pb, Vertex::Ptr pc) : toDelete(false), cList(new ConflictList(true)) {
		vertices.push_back(pa);
		vertices.push_back(pb);
		vertices.push_back(pc);

		initEdges(pa, pb, pc);
	}

	/** Constructs the face such that point T is behind the face.*/
	Face (Vertex::Ptr pa, Vertex::Ptr pb, Vertex::Ptr pc, Vertex::Ptr t) : toDelete(false) {
		vertices.push_back(pa);
		vertices.push_back(pb);
		vertices.push_back(pc);

		if (!isBehind(t))
			swap(vertices[1], vertices[2]);

		initEdges(pa, pb, pc);
	}

	/** Returns TRUE iff, PT is behind this face.*/
	bool isBehind(Vertex::Ptr pt);

	void connect(Face::Ptr adjacent, Vertex::Ptr a, Vertex::Ptr b);
	void connect(boost::shared_ptr<HalfEdge> e);

	boost::shared_ptr<HalfEdge> getMatchingEdge(Vertex::Ptr a, Vertex::Ptr b);
	boost::shared_ptr<HalfEdge> getHorizonEdge();
};

#endif
