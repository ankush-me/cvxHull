#include "geom/Face.h"
#include "geom/HalfEdge.h"
#include "geom/Vertex.h"
#include "geom/ConflictGraph.h"

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <vector>


/** ConvexHull is a 3D polytope which implements the randomized
 *  incremental algorithm for constructing a convex hull from a point
 *  cloud.*/
class ConvexHull : public SubDivision {
public:
	/** Current vertex to add to the hull */
	int  current;

	/** List of newly created facets */
	std::vector<Face::Ptr> newFaces;

	/** List of edges on the horizon */
	std::vector<HalfEdge::Ptr> horizonEdges;

	/** List of facets visible to the current vertex */
	std::vector<Face::Ptr> conflictFaces;

	bool initFromFile;
	std::string outPrefix;


	/** Constructor. */
	ConvexHull(std::string fname, std::string outname) : initFromFile(true) {
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
	ConvexHull::ConvexHull(vector3d &pts, std::string file_prefix) :
					initFromFile(false), outPrefix(file_prefix) {
		for(int i =0; i< pts.size(); i++) {
			Vertex::Ptr v(new Vertex);
			v->pt.reset(new Eigen::Vector3d(pts[i]));
			v->cList.reset(new ConflictList(false));
		}
		random_shuffle(vertices.begin(), vertices.end());
	}

	/** Add the next vertex to the convex hull. */
	void insertNext() {
		if (current == 0) {
			initTetraAndConflicts();
		} else if (newFaces.size() == 0) {
			stepA();
			stepB();
		} else {
			stepC();
		}
	}

	/** To begin the convex hull algorithm, we create a tetrahedron from
	 *  the first four vertices in the point cloud. */
	void initTetraAndConflicts() {
		assert (("Convex Hull requires at least, 4 points.", vertices.size()>=4));
		Vertex::Ptr v1 = vertices[0];
		Vertex::Ptr v2 = vertices[1];
		Vertex::Ptr v3 = vertices[2];
		Vertex::Ptr v4 = vertices[3];

		Face::Ptr f1(new Face(v1, v2, v3, v4));
		Face::Ptr f2(new Face(v1, v3, v4, v2));
		Face::Ptr f3(new Face(v1, v2, v4, v3));
		Face::Ptr f4(new Face(v2, v3, v4, v1));

		faces.clear();
		faces.push_back(f1);
		faces.push_back(f2);
		faces.push_back(f3);
		faces.push_back(f4);

		for (int i=0; i<faces.size(); i++)
			faces[i]->cList.reset(new ConflictList(true));

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

		for (int i=0; i < 4; i++) {
			vertices[i]->visible = false;
		}
	}

	/** StepA begins an incremental step of the algorithm.
	 * <ul>
	 * <li>Identify the next vertex v.  O(1)
	 * <li>Identify all facets visible to v.  O(F(v))
	 * <li>Find the list of horizon edges for v.  O(F(v))
	 * </ul>
	 * F(v) refers to the facets visible to vertex v.
	 */
	void stepA() {
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


	/**
	 * StepB continues the incremental step by connecting vertex v to
	 * each edge of the horizon.
	 */
	void stepB() {

		if (this.current >= getVertexCount()) return;

		Vertex v = getVertex(current);

		/*
		 * Create new facets to connect to the horizon  O(v)
		 */
		Facet old, last = null, first = null;

		for (int i=0; i<horizon.size(); i++) {

			Edge e = (Edge)horizon.get(i);
			old = e.getTwin().getFacet();

			/*
			 * Create a new facet
			 */
			Facet f = new Facet(v, e.getDest(), e.getSource());
			f.setData(new ConflictList(true));
			addFacet(f);
			created.add(f);
			f.setFilled(false);

			/*
			 * Connect it to the hull
			 */
			f.connect(e);
			if (last != null) f.connect(last, v, e.getSource());
			last = f;
			if (first == null) first = f;

			/*
			 * Update conflict graph for the new facet
			 */
			addConflicts(f, old, e.getFacet());
		}

		if (last != null && first != null) {
			last.connect(first, v, first.getEdge(1).getDest());
		}
	}

	/**
	 * StepC cleans up the process started in steps A and B by removing
	 * all of the previously visible facets (including the corresponding
	 * nodes and edges in the conflict graph).
	 */
	void stepC() {

		/*
		 * Hide the just-processed vertex
		 */
		getVertex(current).setVisible(false);

		/*
		 * Remove all previously visible facets
		 */
		Facet f;
		for (int i=0; i<visible.size(); i++) {
			f = (Facet)visible.get(i);
			removeConflicts(f);
			removeFacet(f);
		}

		/*
		 * Fill in all newly created facets
		 */
		for (int i=0; i<created.size(); i++) {
			((Facet)created.get(i)).setFilled(true);
		}
		created.clear();

		this.current++;
	}

	/**
	 * Add an arc to the conflict graph connecting the given facet and
	 * vertex.
	 */
	void addConflict(Face::Ptr f, Vertex::Ptr v) {
		GraphArc arc = new GraphArc(f, v);
		((ConflictList)f.getData()).add(arc);
		((ConflictList)v.getData()).add(arc);
	}


	/**
	 * Remove all conflicts for the given facet.
	 */
	void removeConflicts(Facet f) {
		((ConflictList)f.getData()).clear();
	}


	/**
	 * Test all conflicts for the existing facet with the new facet.  Add
	 * conflict arc for all vertices that can now see the new facet.
	 */
	void addConflicts(Facet f, Facet old1, Facet old2) {

		List l1 = new java.util.ArrayList();
		List l2 = new java.util.ArrayList();
		List l = new java.util.ArrayList();

		((ConflictList)old1.getData()).getVertices(l1);
		((ConflictList)old2.getData()).getVertices(l2);

		Vertex v1, v2;
		int i1 = 0, i2 = 0;

		while (i1 < l1.size() || i2 < l2.size()) {

			if (i1 < l1.size() && i2 < l2.size()) {
				v1 = (Vertex)l1.get(i1);
				v2 = (Vertex)l2.get(i2);
				if (v1.getIndex() == v2.getIndex()) {
					l.add(v1);
					i1++;
					i2++;
				}
				else if (v1.getIndex() > v2.getIndex()) {
					l.add(v1);
					i1++;
				}
				else {
					l.add(v2);
					i2++;
				}
			}
			else if (i1 < l1.size()) {
				l.add(l1.get(i1++));
			}
			else {
				l.add(l2.get(i2++));
			}
		}

		Vertex v;
		for (int i=l.size() - 1; i >= 0; i--) {
			v = (Vertex)l.get(i);
			if (!f.behind(v)) addConflict(f, v);
		}
	}

}
