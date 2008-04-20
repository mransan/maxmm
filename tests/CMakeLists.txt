INCLUDE(FindPkgConfig)

include_directories   (${maxutils_SOURCE_DIRECTORY}/include)
link_directories      (${maxutils_SOURCE_DIRECTORY})
add_definitions       (-g)

#
# CPPUNIT_LIB
#
pkg_search_module(cppunit cppunit)
include_directories   (${cppunit_INCLUDE}      )
link_directories      (${cppunit_LIBRARY_DIRS} )
add_definitions       (${cppunit_CFLAGS}       )

include_directories   (./include     )
aux_source_directory  (./src TEST_SRC)
add_library           (maxutilsTest SHARED ${TEST_SRC})

set(BOOST_LIB     boost_thread                )
set(LINK_LIBS     ${BOOST_LIB} ${cppunit_LIBRARIES} dl maxutils maxutilsTest)

add_executable        (test main.cpp)
target_link_libraries (test ${LINK_LIBS})


