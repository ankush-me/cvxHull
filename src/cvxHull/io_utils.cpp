/*******************************
 * Author: Ankush Gupta
 * SID   : 22388145
 *
 * Utility functions for file I/O.
 **********************************/

#include "io_utils.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <Eigen/Dense>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_set.hpp>
#include "Data.h"

using namespace Eigen;
using namespace std;

/** Reads a .node file specifying 3-dimensional points
 *  into a vector of Eigen::Vector3d points.*/
void readNodeFile(const std::string &fname,
		std::vector<ConflictVertex3d> &pts, std::vector<Vector3dPtr> &point_ptrs) {

	bool readFirstLine        = false;
	unsigned int N           = -1;
	unsigned int dim         = -1;
	unsigned int i           =  0;

	ifstream inpfile(fname.c_str());
	if(!inpfile.is_open()) {
		cout << "Unable to open file : " << fname << endl;
		exit(-1);
	} else {
		while(!inpfile.eof()) {
			string line;
			getline(inpfile,line);
			vector<string> splitline;
			string buf;
			stringstream ss(line);

			while (ss >> buf)  // extract white-space separated strings on a line
				splitline.push_back(buf);

			if (splitline.size()==0)   // skip the blank lines
				continue;
			if(splitline[0][0] == '#') // comments
				continue;

			if (!readFirstLine) {
				assert(("First line of .node should be : "
						"<# of vertices> <dimension (must be 3)> <# of attributes>"
						"<# of boundary markers (0 or 1)>", splitline.size()== 4));
				N   = atoi(splitline[0].c_str());
				dim = atoi(splitline[1].c_str());
				assert(("Dimension of vertices must be 3", dim==3));
				pts.clear(); point_ptrs.clear();
				pts.resize(N); point_ptrs.resize(N+1);
				i = 0;
				readFirstLine = true;
			} else { // read the vertices
				assert(("Insufficient data while reading .node file. "
						"Vertices should be specified in the following format : "
						"<vertex #> <x> <y> <z> [attributes] [boundary marker]", splitline.size() >= 4));
				int index =  boost::lexical_cast<int>(splitline[0].c_str());
				double x  = boost::lexical_cast<double>(splitline[1].c_str());
				double y  = boost::lexical_cast<double>(splitline[2].c_str());
				double z  = boost::lexical_cast<double>(splitline[3].c_str());

				if (i < N) {
					ConflictVertex3d cpoint(index);
					pts[i]            = cpoint;
					point_ptrs[index] = Vector3dPtr(new Vector3d(x,y,z));
					i += 1;
				} else {
					cout << ">>> Expecting "<< N << " points. Found more while reading "
							<< fname << ". Skipping."<< endl; break;}
			}
		}
		inpfile.close();
	}
}


void reportTriangle(Edge::Ptr e, ConvexHull* subD,
		int check_num, std::vector<std::vector<int> >  &tris) {

	if (e->visit_num == check_num) {// not marked
		if (e->Lnext()->Lnext()->Lnext() == e) {// check it forms a triangle

			vector<int> tri(3);
			tri[0] = e->org();
			tri[1] = e->Rnext()->org();
			tri[2] = e->Rnext()->Rnext()->org();

			e->visit_num += 1;
			e->Rnext()->visit_num += 1;
			e->Rnext()->Rnext()->visit_num += 1;

			// add the triangle.
			tris.push_back(tri);
		}
	}
}


void getAllQuadEdges(char check_num, QuadEdge::Ptr qedge,
		vector<QuadEdge::Ptr> &qedges, vector<QuadEdge::Ptr> &toCall) {

	if (qedge->visit_num == check_num) {
		qedge->visit_num += 1;

		qedges.push_back(qedge);
		Edge::Ptr e = qedge->edges[0];
		Edge::Ptr f = e->Onext();

		while (f!=e) {
			toCall.push_back(f->qEdge());
			f = f->Onext();
		}

		f = e->Dnext();
		while (f!=e) {
			toCall.push_back(f->qEdge());
			f = f->Dnext();
		}
	}
}


/** Writes an .ele file, corresponding to the .node file
 *  which was used to construct the triangulation.
 *  The name of the output file is fname.ele. */
void writeSubdivision(const std::string &fname, ConvexHull* subD) {
	vector<vector<int> > tris;
	vector<QuadEdge::Ptr> qedges;
	vector<QuadEdge::Ptr> toCall;


	toCall.push_back(subD->randEdge);
	while (toCall.size() != 0) {
		QuadEdge::Ptr q = toCall.back();
		toCall.pop_back();
		getAllQuadEdges(0, q, qedges, toCall);
	}
	cout << "Number of quadedges : "<<qedges.size()<<endl;

	for(int i=0; i < qedges.size(); i+=1) {
		QuadEdge::Ptr q = qedges[i];
		reportTriangle(q->edges[0], subD, 0, tris);
		reportTriangle(q->edges[0]->Sym(), subD, 0, tris);
	}
	cout << "Number of triangular faces : "<<tris.size()<<endl;

	ofstream outfile;
	outfile.open(fname.c_str(), ios::out);
	outfile << tris.size()<<"\t"<<3<<"\t"<<0<<endl;
	for (int t=0; t<tris.size(); t+=1)
		outfile <<t+1<<"\t"<<tris[t][0]<<"\t"<<tris[t][1]<<"\t"<<tris[t][2]<<endl;
	outfile.close();
	cout << "Wrote "<<fname<<endl;
}


/** Returns a vector of triangular faces of the convex hull subdivision.
 *  Also returns pointers to all the quad-edges in the subdivision. */
void getAllTriangles(vector<vector3d>& otris, vector<QuadEdge::Ptr> &qedges, ConvexHull* subD) {
	vector<vector<int> > tris;
	vector<QuadEdge::Ptr> toCall;

	otris.clear();
	qedges.clear();

	toCall.push_back(subD->randEdge);
	while (toCall.size() != 0) {
		QuadEdge::Ptr q = toCall.back();
		toCall.pop_back();
		getAllQuadEdges(0, q, qedges, toCall);
	}
	cout << "Number of quadedges : "<<qedges.size()<<endl;

	for(int i=0; i < qedges.size(); i+=1) {
		QuadEdge::Ptr q = qedges[i];
		reportTriangle(q->edges[0], subD, 0, tris);
		reportTriangle(q->edges[0]->Sym(), subD, 0, tris);
	}
	cout << "Number of triangular faces : "<<tris.size()<<endl;

	for (int t=0; t<tris.size(); t+=1) {
		vector3d tpts(3);
		tpts[0] = *(subD->point_ptrs[tris[t][0]]);
		tpts[1] = *(subD->point_ptrs[tris[t][1]]);
		tpts[2] = *(subD->point_ptrs[tris[t][2]]);

		otris.push_back(tpts);
	}
}
