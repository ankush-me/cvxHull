/**
 *  Wrappers for common types of data.
 *  Defined so that an Edge can contain generic data. */
#ifndef __DATAA_H__
#define __DATAA_H__

#include <Eigen/Dense>
#include <boost/shared_ptr.hpp>

// Pointers to Eigen points.
typedef boost::shared_ptr<Eigen::Vector2d> Vector2dPtr;
typedef boost::shared_ptr<Eigen::Vector2f> Vector2fPtr;
typedef boost::shared_ptr<Eigen::Vector3d> Vector3dPtr;
typedef boost::shared_ptr<Eigen::Vector3f> Vector3fPtr;

#endif // __DATAA_H__
