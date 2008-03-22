PROJECT(maxutils)
 
SET(INCLUDE_FILES ./include/maxutils/Condition.h
				  ./include/maxutils/Mutex.h
				  ./include/maxutils/Time.h
   				  ./include/maxutils/Thread.h
				  ./include/maxutils/TimedThread.h
				  ./include/maxutils/MTWrapper.h
				  ./include/maxutils/Logger.h
                  ./include/maxutils/all.h

)


INCLUDE_DIRECTORIES (./include/)

aux_source_directory (./src SRC)
add_library          (maxutils SHARED ${SRC})

add_subdirectory     (tests)

CONFIGURE_FILE(${maxutils_SOURCE_DIR}/maxutils.pc.cmake 
			   ${maxutils_BINARY_DIR}/maxutils.pc @ONLY)

INSTALL(TARGETS maxutils
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

INSTALL(FILES       ${INCLUDE_FILES}
		DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maxutils/)

INSTALL(FILES       ${maxutils_SOURCE_DIR}/maxutils.pc 
		DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
