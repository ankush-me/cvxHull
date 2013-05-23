add_executable(test_tetra testTetra.cpp)
target_link_libraries(test_tetra  osgviewer utils cvxHull)

add_executable(cvxApp cvxApp.cpp)
target_link_libraries(cvxApp  osgviewer utils cvxHull)