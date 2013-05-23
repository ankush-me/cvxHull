/*******************************
 * Author: Ankush Gupta
 *
 * Utility functions for file I/O.
 **********************************/
#ifndef __IO_UTILS_H__
#define __IO_UTILS_H__

#include <vector>
#include <string>
#include <Eigen/AlignedVector>
#include "ConvexHull.h"

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#define STRINGIFY(x) #x
#define EXPAND(x) STRINGIFY(x)


/** Reads a .node file specifying 3-dimension points
 *  into a vector of Eigen::Vector3d points
 *
 *  Also stores against the pointer of each point, its
 *  index in the node file. This is useful in writing the .ele
 *  file later. */
void readNodeFile(const std::string &fname, ConvexHull *hull);

//
///** Writes an .ele and its corresponding .node file,
// *  for a given delaunay triangulation.
// *  The name of the output file are {fname.ele, fname.node}. */
//void writeSubdivision(const std::string &fname, ConvexHull* sub);
//
//
///** Returns a vector of triangular faces of the convex hull subdivision.
// *  Also returns pointers to all the quad-edges in the subdivision. */
//void getAllTriangles(std::vector<vector3d>& otris, std::vector<QuadEdge::Ptr> &qedges, ConvexHull* subD);

#endif //__IO_UTILS_H__
