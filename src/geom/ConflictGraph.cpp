#include "ConflictGraph.h"
#include <iostream>

#include "Face.h"
#include "Vertex.h"

using namespace std;

ConflictEdge::ConflictEdge(boost::shared_ptr<Face> f, boost::shared_ptr<Vertex>  v) : face(f), vertex(v) {}

/** Delete this GraphArc from both doubly-linked lists. */
void ConflictEdge::deleteEdge() {
	if (prevv) prevv->nextv = nextv;
	if (nextv) nextv->prevv = prevv;
	if (prevf) prevf->nextf = nextf;
	if (nextf) nextf->prevf = prevf;

	ConflictList::Ptr lst;
	if (!prevv) {
		vertex->cList->head = nextv;
	}
	if (!prevf) {
		face->cList->head = nextf;
	}
}

ConflictList::ConflictList(bool _face) : face(_face) {}

void ConflictList::add(ConflictEdge::Ptr e) {
	if (face) {
		if (head) { head->prevf = e; }
		e->nextf = head;
		head = e;
	} else {
		if (head) { head->prevv = e; }
		e->nextv = head;
		head = e;
	}
}

bool ConflictList::isEmpty() {return (bool) (!head);}

void ConflictList::clear() {
	while (head ) {	head->deleteEdge();}
}

/** Fill a list of vertices by walking the doubly-linked facet list.*/
void ConflictList::getVertices(std::vector<Vertex::Ptr> &list) {
	ConflictEdge::Ptr e = head;
	while (e) {
		list.push_back(e->vertex);
		e = e->nextf;
	}
}

/** Fill a list of facets by walking the doubly-linked facet list. */
void ConflictList::getFaces(std::vector<Face::Ptr> &list) {
	ConflictEdge::Ptr e = head;
	while (e) {
		list.push_back(e->face);
		e = e->nextv;
	}
}

void ConflictList::printVertexList() {
	cout << "Vertices ["<<endl;
	ConflictEdge::Ptr e = head;
	while (e) {
		cout << "\t" << e->vertex->pt->transpose()<<endl;
		e = e->nextf;
	}
	cout<< " ]"<<endl;
}
