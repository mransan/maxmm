include_directories   (${maxutils_SOURCE_DIRECTORY}/include)
link_directories      (${maxutils_SOURCE_DIRECTORY})

include_directories   (./)

#
# List of Test executables.
#


ADD_EXECUTABLE        (testTime_1 testTime_1.cpp)
TARGET_LINK_LIBRARIES (testTime_1 maxutils boost_thread)

ADD_EXECUTABLE        (testThread_1 testThread_1.cpp)
TARGET_LINK_LIBRARIES (testThread_1 maxutils boost_thread)

ADD_EXECUTABLE        (testThread_2 testThread_2.cpp)
TARGET_LINK_LIBRARIES (testThread_2 maxutils boost_thread)

ADD_EXECUTABLE        (testThread_3 TestCommon.cpp testThread_3.cpp) 
TARGET_LINK_LIBRARIES (testThread_3 maxutils boost_thread)

ADD_EXECUTABLE        (testCondition_1 testCondition_1.cpp)
TARGET_LINK_LIBRARIES (testCondition_1 maxutils boost_thread)

ADD_EXECUTABLE        (testMTWrapper_1 TestCommon.cpp testMTWrapper_1.cpp)
TARGET_LINK_LIBRARIES (testMTWrapper_1 maxutils boost_thread)

