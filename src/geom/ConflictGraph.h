#ifndef __CONFLICT_GRAPH_H__
#define __CONFLICT_GRAPH_H__


#include "Data.h"
#include <boost/shared_ptr.hpp>
#include <vector>

class Face;
class Vertex;

/** The Conflict Graph is completely constructed of doubly-linked lists
 *  of GraphArcs stored in the facets and vertices of the convex hull.
 *  Each GraphArc indicates that the given facet and vertex are visible
 *  to each other and therefore in conflict. */
class ConflictEdge {
public:

	typedef boost::shared_ptr<ConflictEdge> Ptr;

	// Pointers to conflict faces and vertices.
	boost::shared_ptr<Face>  face;
	boost::shared_ptr<Vertex> vertex;

	// Doubly-linked list for a vertex
	ConflictEdge::Ptr nextv;
	ConflictEdge::Ptr prevv;

	// Doubly-linked list for a facet
	ConflictEdge::Ptr nextf;
	ConflictEdge::Ptr prevf;

	/** Create a new arc for the conflict graph.  This arc won't be
	 *  connected until add() is called. */
	ConflictEdge(boost::shared_ptr<Face> f, boost::shared_ptr<Vertex>  v) : face(f), vertex(v) {}

	/** Delete this GraphArc from both doubly-linked lists. */
	void deleteEdge();
};


/**
 * Every vertex and facet in this convex hull algorithm keeps a
 * ConflictList.  For a vertex, this list contains all of the facets
 * that are visible to it.  For a facet, the list contains all of the
 * vertices that can see it.  The conflict lists are stored as doubly-
 * connected lists of GraphArcs and are designed to support constant-
 * time add and remove operations. */
class ConflictList {
public:
	typedef boost::shared_ptr<ConflictList> Ptr;

	ConflictEdge::Ptr head;
	bool face;

	ConflictList(bool _face);

	void add(ConflictEdge::Ptr e);
	bool isEmpty();
	void clear();

	/** Fill a list of vertices by walking the doubly-linked facet list.*/
	void getVertices(std::vector<boost::shared_ptr<Vertex> > &list);

	/** Fill a list of facets by walking the doubly-linked facet list. */
	void getFaces(std::vector<boost::shared_ptr<Face> > &list);

	void printVertexList();
};

#endif
