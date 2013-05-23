#include <iostream>
#include "cvxHull/ConvexHull.h"

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
	int N=100;
	for (int i=0; i < N; i++) {
		cube_pts.push_back(MatrixXd::Random(3,1));
		cout << cube_pts[i].transpose()<<endl;
	}


	ConvexHull chull(cube_pts,"cube");
	int i=0;
	while(i < N-4) {chull.insertNext(); i++;}

	// plot the triangular faces.
	PlotTriangles::Ptr plotTriangles(new PlotTriangles);
	plotTriangles->m_defaultColor = osg::Vec4d(1,0,0,1);
	s.env->add(plotTriangles);


	cout << "num faces : "<<chull.faces.size()<<endl;

	for(int i=0; i < chull.faces.size(); i++) {
		vector3d tris(3);
		tris[0] = *(chull.faces[i]->vertices[0]->pt);
		tris[1] = *(chull.faces[i]->vertices[1]->pt);
		tris[2] = *(chull.faces[i]->vertices[2]->pt);
		cout << "adding"<<endl;
		plotTriangles->addTriangle(toFloat(tris));
	}

	//plot the edges.
	s.run();
}
