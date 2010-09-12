
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

set(LINK_LIBS   maxmmnnet
                maxmmutl
                ${Boost_LIBRARIES} 
                ${cppunit_LIBRARIES} 
                ${apr_LIBRARIES}
                ${libxml++_LIBRARIES})

set(TEST_SRC    nnettest.main.cpp
       
                NeuronTest)

include_directories   (./)
add_executable        (nnettest.tsk ${TEST_SRC})
target_link_libraries (nnettest.tsk ${LINK_LIBS})




