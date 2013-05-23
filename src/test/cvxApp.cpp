#include <iostream>
#include "cvxHull/ConvexHull.h"

#include "osgviewer/simplescene.h"
#include "osgviewer/plotting.h"
#include "osgviewer/util.h"

#include <utils/misc.h>


using namespace std;
using namespace Eigen;

class CustomScene : public Scene{};

string welcome =
		"*******************************************************\n\n"
		"                    3D CONVEX HULLS                   \n\n"
		"                      Ankush Gupta                    \n\n"
		"*******************************************************\n\n"
		"Implements the 3D convex hull algorithm, as described in"
		"the Dutch book. The subdivision is represented using a DCEL.\n"
		"It uses Jonathan Shewchuck's robust geometric predicates.\n"
		"The file format for input/output is specified at : \n"
		"    http://www.cs.cmu.edu/~quake/triangle.node.html\n\n"
		"Please use Shewchuk's \'ShowMe\' program for visualization.\n"
		"It can be found at :\n"
		"    http://www.cs.cmu.edu/~quake/triangle.html\n\n"
		"This program was written by Ankush Gupta\n"
		"Use \n    ./cvxApp -h\nfor help on how to use this program.\n"
		"*******************************************************\n\n";

string usage =
		"./cvxApp -i input_file_name [-o output_file_name] [-A or V] [-T]\n"
		"    -i input_file_name  : path to .node file\n"
		"    -o output_file_name : path to .ele file [optional].\n"
		"                          If not specified the .ele file is\n"
		"                          stored in the same directory as .node file.\n"
		"    -A or -V            : switch between alternating [default] or vertical cuts.\n"
		"    -T                  : print the time taken for the algorithm to run.\n"
		"                          File I/O time is excluded.\n\n";

void printUsageAndExit() {
	cout << usage;
	exit(0);
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
	plotTriangles->m_defaultColor = osg::Vec4d(1,0,0,0.8);
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

int main(int argc, char** argv) {
	if (argc == 1) {
		cout << welcome;
		exit(0);
	}
	string input, output;

	bool vertical       = false;
	bool time_algorithm = false;
	for ( int i = 1; i < argc; ++i ) {
		std::string arg( argv[i] );
		if ( arg == "--help" || arg == "-h" ) {
			printUsageAndExit();
		} else if ( arg == "-i") {
			if ( i == argc-1 ) {
				printUsageAndExit();
			}
			input = argv[++i];
		} else if ( arg == "-o" ) {
			output = argv[++i];
		} else if ( arg == "-V" ) {
			vertical = true;
		} else if (arg == "-T") {
			time_algorithm = true;
		} else if (arg == "-A") {
			continue;
		} else {
			cout << "Unknown option: '" << arg << "'\n";
			printUsageAndExit();
		}
	}

	if(!input.length())
		printUsageAndExit();


	ConvexHull chull(input, "out");
	chull.computeHull();

	// draw the convex hull
	CustomScene s;
	Eigen::Affine3f tf = Eigen::Affine3f::Identity();
	util::drawAxes(tf, 0.5, s.env);
	drawConvexHull(chull, s);
	s.run();
}



