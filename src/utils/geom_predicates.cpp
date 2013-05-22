#include "geom_predicates.h"

// Shewchuk's robust predicates
extern "C" {
#include "predicates.c"
}

using namespace Eigen;

float initializedExact = false;

void inline initExact() {
	if (!initializedExact) {
		initializedExact = true;
		exactinit();
	}
}


/** Returns true iff the points are collinear.
 *  From Shewchuk's robust predicates notes.*/
bool collinear3d(Vector3d pa, Vector3d pb, Vector3d pc) {
	initExact();
	double cross_x = orient2d(Vector2d(pa.y(), pa.z()),
			                    Vector2d(pb.y(), pb.z()),
			                    Vector2d(pc.y(), pc.z()));
	if (cross_x != 0.0) return false;

	double cross_y = orient2d(Vector2d(pa.z(), pa.x()),
			                    Vector2d(pb.z(), pb.x()),
			                    Vector2d(pc.z(), pc.x()));
	if (cross_y != 0.0) return false;

	double cross_z = orient2d(Vector2d(pa.x(), pa.y()),
			                    Vector2d(pb.x(), pb.y()),
			                    Vector2d(pc.x(), pc.y()));
	if (cross_z != 0.0) return false;

	return true;
}

double orient2d (Vector2d pa, Vector2d pb, Vector2d pc) {
	initExact();
	return (double) orient2d(pa.data(), pb.data(), pc.data());
}

double orient2d (Vector2f pa, Vector2f pb, Vector2f pc) {
	initExact();
	Vector2d v1((double) pa[0], (double) pa[1]);
	Vector2d v2((double) pb[0], (double) pb[1]);
	Vector2d v3((double) pc[0], (double) pc[1]);
	return (double) orient2d(v1.data(), v2.data(), v3.data());
}

double orient3d (Vector3d pa, Vector3d pb, Vector3d pc, Vector3d pd) {
	initExact();
	return (double) orient3d(pa.data(), pb.data(), pc.data(), pd.data());
}

double incircle (Eigen::Vector2d pa, Eigen::Vector2d pb,
		Eigen::Vector2d pc, Eigen::Vector2d pd) {
	initExact();
	return incircle(pa.data(), pb.data(), pc.data(), pd.data());
}

double incircle (Eigen::Vector2f pa, Eigen::Vector2f pb,
		Eigen::Vector2f pc, Eigen::Vector2f pd) {
	initExact();
	Vector2d v1((double) pa[0], (double) pa[1]);
	Vector2d v2((double) pb[0], (double) pb[1]);
	Vector2d v3((double) pc[0], (double) pc[1]);
	Vector2d v4((double) pd[0], (double) pd[1]);
	return incircle(v1.data(), v2.data(), v3.data(), v4.data());
}

bool ccw(Vector2d pa, Vector2d pb, Vector2d pc) {
	return (bool) (orient2d(pa, pb, pc)> 0.0);
}

bool ccw(Vector2f pa, Vector2f pb, Vector2f pc) {
	return (bool)(orient2d(pa, pb, pc) > 0.0);
}

bool cw(Vector2d pa, Vector2d pb, Vector2d pc) {
	return (bool) (orient2d(pa, pb, pc) < 0.0);
}
bool cw(Vector2f pa, Vector2f pb, Vector2f pc) {
	return (bool) (orient2d(pa, pb, pc) < 0.0);
}
