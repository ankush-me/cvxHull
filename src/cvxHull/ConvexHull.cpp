#include "ConvexHull.h"
#include "io_utils.h"

#include <iostream>

using namespace std;

/** ConvexHull is a 3D polytope which implements the randomized
 *  incremental algorithm for constructing a convex hull from a point
 *  cloud.*/

/** Constructor. */
ConvexHull::ConvexHull(std::string fname, std::string outname) :current(0), initFromFile(true) {
	if (fname.substr(fname.length()-5,5)!= ".node") {
		std::cout << "Expecting input file with .node extension. Instead, found "
				<<fname.substr(fname.length()-5,5)<<". Exiting.\n";
		exit(-1);
	}

	outPrefix  = (outname =="xdefaultx")? fname.substr(0, fname.length()-5) : outname;
	readNodeFile(fname, this);

	// randomly shuffle the points for randomized incremental insertion.
	random_shuffle(vertices.begin(), vertices.end());
}


/** Compute the convex hull of PTS. FILE_PREFIX :*/
ConvexHull::ConvexHull(vector3d &pts, std::string file_prefix) : current(0), initFromFile(false), outPrefix(file_prefix) {
	for(int i =0; i< pts.size(); i++) {
		Vertex::Ptr v(new Vertex(i));
		v->pt.reset(new Eigen::Vector3d(pts[i]));
		vertices.push_back(v);
	}
	//random_shuffle(vertices.begin(), vertices.end());
}


/** Add the next vertex to the convex hull. */
bool ConvexHull::insertNext() {
	if (current >= vertices.size()) false;
	if (current == 0) {
		initTetraAndConflicts();
		return true;
	} else if (newFaces.size() == 0) {
		stepA();
		stepB();
		return true;
	} else {
		stepC();
		return true;
	}
}


/** To begin the convex hull algorithm, we create a tetrahedron from
 *  the first four vertices in the point cloud. */
void ConvexHull::initTetraAndConflicts() {

	assert (("Convex Hull requires at least, 4 points.", vertices.size()>=4));
	Vertex::Ptr v1 = vertices[0];
	Vertex::Ptr v2 = vertices[1];
	Vertex::Ptr v3 = vertices[2];
	Vertex::Ptr v4 = vertices[3];

	Face::Ptr f1(new Face(v1, v2, v3, v4));
	f1->initEdges();

	Face::Ptr f2(new Face(v1, v3, v4, v2));
	f2->initEdges();

	Face::Ptr f3(new Face(v1, v2, v4, v3));
	f3->initEdges();

	Face::Ptr f4(new Face(v2, v3, v4, v1));
	f4->initEdges();


	faces.clear();
	faces.push_back(f1);
	faces.push_back(f2);
	faces.push_back(f3);
	faces.push_back(f4);

	f1->connect(f2, v1, v3);
	f1->connect(f3, v1, v2);
	f1->connect(f4, v2, v3);
	f2->connect(f3, v1, v4);
	f2->connect(f4, v3, v4);
	f3->connect(f4, v2, v4);

	current = 4;
	/** initialize conflicts.*/
	for (int i=4; i < vertices.size(); i++) {
		if (!f1->isBehind(vertices[i])) addConflict(f1, vertices[i]);
		if (!f2->isBehind(vertices[i])) addConflict(f2, vertices[i]);
		if (!f3->isBehind(vertices[i])) addConflict(f3, vertices[i]);
		if (!f4->isBehind(vertices[i])) addConflict(f4, vertices[i]);
	}

	for (int i=0; i < 4; i++)
		vertices[i]->visible = false;
}

/** StepA begins an incremental step of the algorithm.
 * <ul>
 * <li>Identify the next vertex v.  O(1)
 * <li>Identify all facets visible to v.  O(F(v))
 * <li>Find the list of horizon edges for v.  O(F(v))
 * </ul>
 * F(v) refers to the facets visible to vertex v.*/
void ConvexHull::stepA() {
	if (current >= vertices.size()) return;

	newFaces.clear();
	horizonEdges.clear();
	conflictFaces.clear();

	/*Get list of visible facets for v.*/
	Vertex::Ptr v = vertices[current];
	v->cList->getFaces(conflictFaces);

	/*If v is already inside the convex hull, try the next point.*/
	if (conflictFaces.size() == 0) {
		v->visible = false;
		current += 1;
		stepA();
		return;
	}

	/* Flag visible facets. */
	for (int i=0; i< conflictFaces.size(); i++) {
		conflictFaces[i]->toDelete = true;
	}

	/*Find horizon edges*/
	for (int i=0; i< conflictFaces.size(); i++) {
		HalfEdge::Ptr e = conflictFaces[i]->getHorizonEdge();
		if (e) {
			e->findHorizon(horizonEdges);
			break;
		}
	}
}


/** StepB continues the incremental step by connecting vertex v to
 *  each edge of the horizon. */
void ConvexHull::stepB() {
	cout << "STEP B :"<<endl;

	if (current >= vertices.size()) return;
	Vertex::Ptr v = vertices[current];

	/** Create new facets to connect to the horizon  O(v). */
	Face::Ptr old, last, first;
	for (int i=0; i<horizonEdges.size(); i++) {
		HalfEdge::Ptr e = horizonEdges[i];
		old = e->twin->face;

		/** Create a new facet. */
		Face::Ptr f(new Face(v, e->dst, e->org));
		f->initEdges();
		faces.push_back(f);
		newFaces.push_back(f);
		//f.setFilled(false);

		/** Connect it to the hull. */
		f->connect(e);
		if (last) f->connect(last, v, e->org);
		last = f;
		if (!first) first = f;

		/** Update conflict graph for the new facet*/
		addConflicts(f, old, e->face);
	}

	if (last && first) {
		last->connect(first, v, first->edges[1]->dst);
	}
}


/** StepC cleans up the process started in steps A and B by removing
 *  all of the previously visible facets (including the corresponding
 *  nodes and edges in the conflict graph). */
void ConvexHull::stepC() {
	/** Hide the just-processed vertex. */
	vertices[current]->visible = false;

	/** Remove all previously visible facets. */
	for (int i=0; i < conflictFaces.size(); i++) {
		Face::Ptr f = conflictFaces[i];
		removeConflicts(f);
		removeFace(f);
	}

	/** Fill in all newly created facets. */
	for (int i=0; i< newFaces.size(); i++) {
		//newFaces[i]->setFilled(true);
	}

	newFaces.clear();
	current++;
}


/** Register a conflict in the conflict graph. */
void ConvexHull::addConflict(Face::Ptr f, Vertex::Ptr v) {
	ConflictEdge::Ptr e(new ConflictEdge(f, v));
	f->cList->add(e);
	v->cList->add(e);
}


/** Remove all conflicts for the given facet. */
void ConvexHull::removeConflicts(Face::Ptr f) {
	f->cList->clear();
}


/** Test all conflicts for the existing facet with the new facet.
 *  Add conflict arc for all vertices that can now see the new facet.*/
void ConvexHull::addConflicts(Face::Ptr f, Face::Ptr old1, Face::Ptr old2) {
	std::vector<Vertex::Ptr> l1;
	std::vector<Vertex::Ptr> l;

	old1->cList->getVertices(l);
	old2->cList->getVertices(l1);

	for(int i=0; i<l1.size(); i++)
		l.push_back(l1[i]);

	// de-duplicate
//	std::sort(l.begin(), l.end());
//	std::vector<Vertex::Ptr>::iterator it = std::unique(l.begin(), l.end());
//	l.resize( std::distance(l.begin(),it) );

	for (int i=0; i < l.size(); i++) {
		Vertex::Ptr v = l[i];
		if (!f->isBehind(v)) addConflict(f, v);
	}
}
