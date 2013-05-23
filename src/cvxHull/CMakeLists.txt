

add_library(cvxHull ConvexHull.cpp io_utils.cpp)
target_link_libraries(cvxHull geom utils)