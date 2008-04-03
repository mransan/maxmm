INCLUDE(FindPkgConfig)

include_directories   (${maxutils_SOURCE_DIRECTORY}/include)
link_directories      (${maxutils_SOURCE_DIRECTORY})


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

#
# List of Test executables.
#


ADD_EXECUTABLE        (testThread_2 testThread_2.cpp)                                                         
TARGET_LINK_LIBRARIES (testThread_2     ${LINK_LIBS})
                                                                                                              
ADD_EXECUTABLE        (testThread_3 testThread_3.cpp)                                          
TARGET_LINK_LIBRARIES (testThread_3     ${LINK_LIBS}) 
                                                                                                              
ADD_EXECUTABLE        (testMTWrapper_1 testMTWrapper_1.cpp)                                    
TARGET_LINK_LIBRARIES (testMTWrapper_1  ${LINK_LIBS})
                                                                                                              
ADD_EXECUTABLE        (testMTWrapper_2 testMTWrapper_2.cpp)                                    
TARGET_LINK_LIBRARIES (testMTWrapper_2  ${LINK_LIBS})

