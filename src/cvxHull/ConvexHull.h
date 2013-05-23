#ifndef __CVX_HULL_H__
#define __CVX_HULL_H__

#include "geom/Face.h"
#include "geom/HalfEdge.h"
#include "geom/Vertex.h"
#include "geom/SubD.h"
#include "geom/ConflictGraph.h"

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <Eigen/AlignedVector>


/** ConvexHull is a 3D polytope which implements the randomized
 *  incremental algorithm for constructing a convex hull from a point
 *  cloud.*/
class ConvexHull : public SubDivision {
public:

	typedef boost::shared_ptr<ConvexHull> Ptr;

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
	ConvexHull(std::string fname, std::string outname);

	/** Compute the convex hull of PTS. FILE_PREFIX :*/
	ConvexHull(vector3d &pts, std::string file_prefix);

	/** Add the next vertex to the convex hull. */
	bool insertNext();
	void computeHull();

	/** To begin the convex hull algorithm, we create a tetrahedron from
	 *  the first four vertices in the point cloud. */
	void initTetraAndConflicts();
	/** StepA begins an incremental step of the algorithm.
	 * <ul>
	 * <li>Identify the next vertex v.  O(1)
	 * <li>Identify all facets visible to v.  O(F(v))
	 * <li>Find the list of horizon edges for v.  O(F(v))
	 * </ul>
	 * F(v) refers to the facets visible to vertex v.*/
	void stepA();


	/** StepB continues the incremental step by connecting vertex v to
	 *  each edge of the horizon. */
	void stepB();


	/** StepC cleans up the process started in steps A and B by removing
	 *  all of the previously visible facets (including the corresponding
	 *  nodes and edges in the conflict graph). */
	void stepC();

	/** Register a conflict in the conflict graph. */
	void addConflict(Face::Ptr f, Vertex::Ptr v);

	/** Remove all conflicts for the given facet. */
	void removeConflicts(Face::Ptr f);

	/** Test all conflicts for the existing facet with the new facet.
	 *  Add conflict arc for all vertices that can now see the new facet.*/
	void addConflicts(Face::Ptr f, Face::Ptr old1, Face::Ptr old2);
};

#endif
