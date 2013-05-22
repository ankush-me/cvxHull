#include "Face.h"
#include "HalfEdge.h"

using namespace std;
using namespace boost;

/** Initalizes the edges of the triangular face. */
void Face::initEdges(Vector3dPtr pa, Vector3dPtr pb, Vector3dPtr pc) {

	HalfEdge::Ptr e0(new HalfEdge(pa,pb, shared_from_this()));
	HalfEdge::Ptr e1(new HalfEdge(pb,pc, shared_from_this()));
	HalfEdge::Ptr e2(new HalfEdge(pc,pa, shared_from_this()));

	edges.push_back(e0);
	edges.push_back(e1);
	edges.push_back(e2);

	// connect the edges up.
	for (int i=0; i <3; i++) {
		edges[i]->next(edges[mod(i+1,3)]);
		edges[i]->prev(edges[mod(i-1,3)]);
	}
}

/** Checks if the point PT is behind this face.*/
bool Face::isBehind(Vector3dPtr pt) {
	return (orient3d(*vertices[0], *vertices[1], *vertices[2], *pt) > 0.0);
}

/** Connects this face to ADJ, making it adjacent.
 *  Does this by finding a matching edge in ADJ. */
void Face::connect(Face::Ptr adj, Vector3dPtr a, Vector3dPtr b) {
	HalfEdge::Ptr in  = getMatchingEdge(a, b);
	HalfEdge::Ptr out = adj->getMatchingEdge(a, b);
	in->twin  = out;
	out->twin = in;
}

/** Connects one of the side whose end-points match that of E.*/
void Face::connect(HalfEdge::Ptr e) {
	HalfEdge::Ptr in = getMatchingEdge(e->org, e->dst);
	in->twin = e;
	e->twin  = in;
}

/** Returns one of the boundary edges if the end-points match.
 *  Otherwise returns null.*/
HalfEdge::Ptr Face::getMatchingEdge(Vector3dPtr a, Vector3dPtr b) {
	for (int i=0; i<3; i++)
		if (edges[i]->matches(a, b))
			return edges[i];

	HalfEdge::Ptr null_ptr;
	return null_ptr;
}

/** Returns a horizon edge [oppositely oriented]. */
HalfEdge::Ptr Face::getHorizonEdge() {
	HalfEdge::Ptr opp;
	for (int i=0; i<3; i++) {
		opp = edges[i]->twin;
		if (opp && opp->onHorizon())
			return edges[i];
	}
	HalfEdge::Ptr null_ptr;
	return null_ptr;
}
