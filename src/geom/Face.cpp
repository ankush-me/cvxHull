#include "Face.h"
#include "HalfEdge.h"
#include "utils/sorting.h"
#include "utils/geom_predicates.h"

using namespace std;
using namespace boost;


Face::Face (Vertex::Ptr pa, Vertex::Ptr pb, Vertex::Ptr pc) : toDelete(false), cList(new ConflictList(true)) {
	vertices.push_back(pa);
	vertices.push_back(pb);
	vertices.push_back(pc);

	//initEdges(pa, pb, pc);
}

/** Constructs the face such that point T is behind the face.*/
Face::Face (Vertex::Ptr pa, Vertex::Ptr pb, Vertex::Ptr pc, Vertex::Ptr t) : toDelete(false), cList(new ConflictList(true))  {

	vertices.push_back(pa);
	vertices.push_back(pb);
	vertices.push_back(pc);

	if (!isBehind(t))
		swap(vertices[1], vertices[2]);

	//initEdges(pa, pb, pc);
}



/** Initalizes the edges of the triangular face. */
void Face::initEdges(Vertex::Ptr pa, Vertex::Ptr pb, Vertex::Ptr pc) {

	HalfEdge::Ptr e0(new HalfEdge(pa,pb, shared_from_this()));
	HalfEdge::Ptr e1(new HalfEdge(pb,pc, shared_from_this()));
	HalfEdge::Ptr e2(new HalfEdge(pc,pa, shared_from_this()));

	edges.push_back(e0);
	edges.push_back(e1);
	edges.push_back(e2);

	// connect the edges up.
	for (int i=0; i <3; i++) {
		edges[i]->next = edges[mod(i+1,3)];
		edges[i]->prev = edges[mod(i-1,3)];
	}
}

/** Checks if the point P is behind this face.*/
bool Face::isBehind(Vertex::Ptr p) {
	return (  orient3d(*(vertices[0]->pt),
				   	    *(vertices[1]->pt),
					    *(vertices[2]->pt),
					    *(p->pt)          ) > 0.0    );
}

/** Connects this face to ADJ, making it adjacent.
 *  Does this by finding a matching edge in ADJ. */
void Face::connect(Face::Ptr adj, Vertex::Ptr a, Vertex::Ptr b) {
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
HalfEdge::Ptr Face::getMatchingEdge(Vertex::Ptr a, Vertex::Ptr b) {
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
