/** Provides various geometric predicates like orientation tests.*/

#pragma once
#include <Eigen/Dense>

/** Returns true iff the points are collinear.
 *  From Shewchuk's robust predicates notes.*/
bool collinear3d(Eigen::Vector3d pa, Eigen::Vector3d pb, Eigen::Vector3d pc);

double orient2d (Eigen::Vector2d pa, Eigen::Vector2d pb, Eigen::Vector2d pc);
double orient2d (Eigen::Vector2f pa, Eigen::Vector2f pb, Eigen::Vector2f pc);

double orient3d (Eigen::Vector3d pa, Eigen::Vector3d pb,
		           Eigen::Vector3d pc, Eigen::Vector3d pd);

double incircle (Eigen::Vector2d pa, Eigen::Vector2d pb, Eigen::Vector2d pc, Eigen::Vector2d pd);
double incircle (Eigen::Vector2f pa, Eigen::Vector2f pb, Eigen::Vector2f pc, Eigen::Vector2f pd);


// pc is tested wrt to the line-segment pa-pb
bool ccw(Eigen::Vector2d pa, Eigen::Vector2d pb, Eigen::Vector2d pc) ;
bool ccw(Eigen::Vector2f pa, Eigen::Vector2f pb, Eigen::Vector2f pc);

bool cw(Eigen::Vector2d pa, Eigen::Vector2d pb, Eigen::Vector2d pc) ;
bool cw(Eigen::Vector2f pa, Eigen::Vector2f pb, Eigen::Vector2f pc);

