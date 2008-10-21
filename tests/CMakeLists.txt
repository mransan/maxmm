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

set(LINK_LIBS     ${Boost_LIBRARIES} ${cppunit_LIBRARIES} ${apr_LIBRARIES} dl maxmm maxmmTest)


include_directories   (./include     )
aux_source_directory  (./src TEST_SRC)
add_library           (maxmmTest SHARED ${TEST_SRC})
target_link_libraries (maxmmTest ${LINK_LIBS})

add_executable        (test main.cpp)
target_link_libraries (test ${LINK_LIBS})


