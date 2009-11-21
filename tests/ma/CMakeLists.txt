
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

set(LINK_LIBS   maxmmma
                maxmm
                ${Boost_LIBRARIES} 
                ${cppunit_LIBRARIES} 
                ${apr_LIBRARIES}
                ${libxml++_LIBRARIES})

set(TEST_SRC    matest.main.cpp
       
                ABTMessageTest.cpp
                ABTNoGoodMessageTest.cpp
                ABTOkMessageTest.cpp
                ABTProcessorTest.cpp
                AgentTest.cpp)

include_directories   (./)
add_executable        (matest.tsk ${TEST_SRC})
target_link_libraries (matest.tsk ${LINK_LIBS})




