#include <iostream>
#include "qedge/ConvexHull.h"

#include "osgviewer/simplescene.h"
#include "osgviewer/plotting.h"
#include "osgviewer/util.h"


using namespace std;
using namespace Eigen;

class CustomScene : public Scene{};


vector3f to3d(const vector2f &pts) {
	vector3f pts3d(pts.size());
	for(int i =0 ; i < pts.size(); i++ ) {
		Vector2f pt = pts[i];
		pts3d[i] = Vector3f(pt.x(), pt.y(), 0);
	}

	return pts3d;
}


vector3f toFloat(vector3d & pts) {
	vector3f opts(pts.size());
	cout << "----"<<endl;
	for (int i=0; i<pts.size();i++) {
		Vector3f vec((float) pts[i].x(), (float) pts[i].y(), (float) pts[i].z());
		cout << vec.transpose()<<endl;
		opts.push_back(vec);
	}
	return opts;
}




int main (int argc, char* argv[]) {
	CustomScene s;
	Eigen::Affine3f tf = Eigen::Affine3f::Identity();
	util::drawAxes(tf, 0.5, s.env);

	vector3d cube_pts;
	MatrixXd::Random(1,3);
	cout << MatrixXd::Random(3,1)<<endl;

	cube_pts.push_back(Vector3d(1.434,1,1)+MatrixXd::Random(3,1));
	cube_pts.push_back(Vector3d(0,0.2,1)+MatrixXd::Random(3,1));
	cube_pts.push_back(Vector3d(1.43,0,1.32)+MatrixXd::Random(3,1));
	cube_pts.push_back(Vector3d(0,0,0)+MatrixXd::Random(3,1));
	cube_pts.push_back(Vector3d(0,1.43,0)+MatrixXd::Random(3,1));
	cube_pts.push_back(Vector3d(1,1,0.454)+MatrixXd::Random(3,1));
	cube_pts.push_back(Vector3d(1,2,0.323)+MatrixXd::Random(3,1));
	cube_pts.push_back(Vector3d(0,3,1)+MatrixXd::Random(3,1));

	ConvexHull chull(cube_pts,"cube");
	chull.computeHull();

	vector<vector3d> tris;
	vector<QuadEdge::Ptr> qedges;
	chull.getTriangles(tris, qedges);


	// plot the triangular faces.
	PlotTriangles::Ptr plotTriangles(new PlotTriangles);
	plotTriangles->m_defaultColor = osg::Vec4d(1,0,0,1);
	s.env->add(plotTriangles);
	for(int i=0; i < tris.size(); i++) {
		plotTriangles->addTriangle(toFloat(tris[i]));
	}

	//plot the edges.
	s.run();
}
