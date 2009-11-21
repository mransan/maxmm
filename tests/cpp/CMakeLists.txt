
include(FindPkgConfig)

include_directories   (${maxmm_SOURCE_DIR}/include)
link_directories      (${maxmm_SOURCE_DIR}/lib)

add_definitions       (-g)

#
# cpp unit library
#
pkg_search_module(cppunit cppunit)
include_directories   (${cppunit_INCLUDE}      )
link_directories      (${cppunit_LIBRARY_DIRS} )
add_definitions       (${cppunit_CFLAGS}       )

set(LINK_LIBS maxmm
              ${Boost_LIBRARIES}
              ${cppunit_LIBRARIES})

set(TEST_SRC  cpptest.main.cpp
              
              BoostAsioUnixSocketTest.cpp
              ExceptionTest.cpp
              IomanipTest.cpp
              StdContainerTest.cpp
              TypeTest.cpp)

include_directories   (./)
add_executable        (cpptest.tsk ${TEST_SRC})
target_link_libraries (cpptest.tsk ${LINK_LIBS})





