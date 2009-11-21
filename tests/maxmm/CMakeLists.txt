
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

set(    LINK_LIBS     
        ${Boost_LIBRARIES} 
        ${cppunit_LIBRARIES} 
        ${apr_LIBRARIES}
        ${libxml++_LIBRARIES} 
        dl 
        maxmm)

set(   TEST_SRC
       maxmmtest.main.cpp
       ConditionTest.cpp
       LockFreeQueueTest.cpp
       LockFreeWrapperTest.cpp
       LockFreeWrapperTest_details.cpp
       NullableValueTest.cpp
       RandomUniformTest.cpp
       StringUtilsTest.cpp
       TestCommon.cpp
       ThreadControllerTest.cpp
       ThreadPoolTest.cpp
       ThreadTest.cpp
       TimeTest.cpp
       XmlDecoderTest.cpp
       XmlEncoderTest.cpp)

include_directories   (./)
add_executable        (maxmmtest.tsk ${TEST_SRC})
target_link_libraries (maxmmtest.tsk ${LINK_LIBS})



