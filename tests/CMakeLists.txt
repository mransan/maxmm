include(FindPkgConfig)

include_directories   (${maxmm_SOURCE_DIRECTORY}/include)
link_directories      (${maxmm_SOURCE_DIRECTORY})
add_definitions       (-g)

#
# cpp unit library
#
pkg_search_module(cppunit cppunit)
include_directories   (${cppunit_INCLUDE}      )
link_directories      (${cppunit_LIBRARY_DIRS} )
add_definitions       (${cppunit_CFLAGS}       )




include_directories   (./include     )
aux_source_directory  (./src TEST_SRC)
add_library           (maxmmTest SHARED ${TEST_SRC})

set(BOOST_LIB     boost_thread boost_filesystem )
set(LINK_LIBS     ${BOOST_LIB} ${cppunit_LIBRARIES} ${apr_LIBRARIES} dl maxmm maxmmTest)

add_executable        (test main.cpp)
target_link_libraries (test ${LINK_LIBS})


