#include <iostream>
#include "cvxHull/ConvexHull.h"

#include "osgviewer/simplescene.h"
#include "osgviewer/plotting.h"
#include "osgviewer/util.h"

#include <utils/misc.h>
#include <time.h>

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
	for (int i=0; i<pts.size();i++) {
		Vector3f vec((float) pts[i].x(), (float) pts[i].y(), (float) pts[i].z());
		opts.push_back(vec);
	}
	return opts;
}


void drawConvexHull(ConvexHull &chull, Scene &s) {
	vector3d pts(chull.vertices.size());
	for(int i=0; i<chull.vertices.size(); i++)
		pts[i] = *(chull.vertices[i]->pt);

	PlotPoints::Ptr plotPts(new PlotPoints);
	plotPts->setPoints(toFloat(pts));
	s.env->add(plotPts);

	// plot the triangular faces.
	PlotTriangles::Ptr plotTriangles(new PlotTriangles);
	s.env->add(plotTriangles);
	plotTriangles->m_defaultColor = osg::Vec4d(1,0,0,0.7);
	// draw faces
	for(int i=0; i < chull.faces.size(); i++) {
		vector3d tris(3);
		tris[0] = *(chull.faces[i]->vertices[0]->pt);
		tris[1] = *(chull.faces[i]->vertices[1]->pt);
		tris[2] = *(chull.faces[i]->vertices[2]->pt);
		plotTriangles->addTriangle(toFloat(tris));
	}

	// draw the edges.
	PlotLines::Ptr plotLines(new PlotLines);
	s.env->add(plotLines);
	int c=0;
	vector3d edges(2*3*chull.faces.size());
	for(int i=0; i < chull.faces.size(); i++) {
		for(int k=0; k < 3; k++) {
			edges[c] = *(chull.faces[i]->vertices[k]->pt); c++;
			edges[c] = *(chull.faces[i]->vertices[mod(k+1,3)]->pt); c++;
		}
	}
	plotLines->setPoints(toFloat(edges));

}

int main (int argc, char* argv[]) {
	CustomScene s;
	Eigen::Affine3f tf = Eigen::Affine3f::Identity();
	util::drawAxes(tf, 0.5, s.env);

	vector3d cube_pts;
	MatrixXd::Random(1,3);
	int N=atoi(argv[1]);
	for (int i=0; i < N; i++)
		cube_pts.push_back(MatrixXd::Random(3,1));

	clock_t tim;
	int f;
	tim = clock();

	ConvexHull chull(cube_pts,"cube");
	int i=0; int pc = chull.current;
	while(i < 2*N) {
		chull.insertNext(); i++;
	}
	tim = clock() - tim;
	cout <<">>> "<<((double)tim)/CLOCKS_PER_SEC<< " seconds to compute the convex hull.\n";


	drawConvexHull(chull, s);
	// plot the points
	s.run();
}
