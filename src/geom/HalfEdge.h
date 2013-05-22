#ifndef __EDGE_H__
#define __EDGE_H__

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Data.h"
#include "Face.h"

/**A class to represent an half-edge of a DCEL.*/
class HalfEdge :  public boost::enable_shared_from_this<HalfEdge> {

public:
	// define a pointer to edges.
	typedef boost::shared_ptr<HalfEdge> Ptr;

	Vector3dPtr org;
	Vector3dPtr dst;

	Face::Ptr face;

	HalfEdge::Ptr next;
	HalfEdge::Ptr prev;
	HalfEdge::Ptr twin;


	HalfEdge(Vector3dPtr _org, Vector3dPtr _dst, Face::Ptr _face) : org(_org), dst(_dst), face(_face) {}


	bool onHorizon() {
		return (!twin)? false : (!face->toDelete && twin->face->toDelete);
	}

	bool matches(Vector3dPtr p, Vector3dPtr  q) {
		return ((*org == *p && *dst == *q)
				|| (*org == *q && *dst == *p));
	}

	void findHorizon(std::vector<HalfEdge::Ptr> &horizon) {
		if (onHorizon()) {
			if (horizon.size() > 0 && this == horizon[0].get()) {
				return;
			} else {
				horizon.push_back(shared_from_this());
				next->findHorizon(horizon);
			}
		} else {
			if (!twin)
				twin->next->findHorizon(horizon);
		}
	}
};

#endif
