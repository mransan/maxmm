project( maxmm )

cmake_minimum_required( VERSION 2.6 )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${maxmm_SOURCE_DIR}/lib)

include(FindBoost)
include(FindPkgConfig)

find_package( Boost COMPONENTS thread system REQUIRED)

#
# Compiler flag.
#
add_definitions (${CMAKE_CXX_FLAGS_DEBUG})
include_directories (${Boost_INCLUDE_DIR})

#
# apache portable environment. 
#
pkg_search_module(apr apr-1)
include_directories   (${apr_INCLUDE}      )
link_directories      (${apr_LIBRARY_DIRS} )
add_definitions       (${apr_CFLAGS}       )

pkg_search_module(libxml++ libxml++-2.6)
include_directories   (${libxml++_INCLUDE}      )
link_directories      (${libxml++_LIBRARY_DIRS} )
add_definitions       (${libxml++_CFLAGS}       )


#
# maxmm include , source and target library.
#
include_directories  ( include )
aux_source_directory ( ./src SRC )
add_library          ( maxmm SHARED ${SRC} )
target_link_libraries ( maxmm 
                        ${apr_LIBRARIES} 
                        ${Boost_LIBRARIES}
                        ${libxml++_LIBRARIES} )

#
# project hierarchy ... only one subdir.
#
add_subdirectory     (tests EXCLUDE_FROM_ALL)
add_subdirectory     (example)
add_subdirectory     (src)


#
# Install commands ( pkg , files , lib. )
#
CONFIGURE_FILE(${maxmm_SOURCE_DIR}/maxmm.pc.cmake 
			   ${maxmm_BINARY_DIR}/maxmm.pc @ONLY)


set(INCLUDE_FILES   ./include/maxmm/Condition.h
				    ./include/maxmm/Mutex.h
				    ./include/maxmm/Time.h
   				    ./include/maxmm/Thread.h
				    ./include/maxmm/TimedThread.h
				    ./include/maxmm/MTWrapper.h
				    ./include/maxmm/Logger.h
				    ./include/maxmm/Observer.h
				    ./include/maxmm/Observable.h
                    ./include/maxmm/all.h
)

INSTALL(TARGETS maxmm
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

INSTALL(FILES       ${INCLUDE_FILES}
		DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maxmm/)

INSTALL(FILES       ${maxmm_SOURCE_DIR}/maxmm.pc 
		DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
