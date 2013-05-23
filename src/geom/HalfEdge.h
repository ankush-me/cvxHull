#ifndef __EDGE_H__
#define __EDGE_H__

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Data.h"
#include "Face.h"
#include "Vertex.h"

/**A class to represent an half-edge of a DCEL.*/
class HalfEdge :  public boost::enable_shared_from_this<HalfEdge> {

public:
	// define a pointer to edges.
	typedef boost::shared_ptr<HalfEdge> Ptr;

	Vertex::Ptr org;
	Vertex::Ptr dst;
	Face::Ptr face;
	HalfEdge::Ptr next;
	HalfEdge::Ptr prev;
	HalfEdge::Ptr twin;

	HalfEdge(Vertex::Ptr _org, Vertex::Ptr _dst, Face::Ptr _face);
	bool onHorizon();
	bool matches(Vertex::Ptr p, Vertex::Ptr  q);
	void findHorizon(std::vector<HalfEdge::Ptr> &horizon);
};

#endif
