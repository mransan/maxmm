
include_directories   (${maxmm_SOURCE_DIR}/include)
link_directories      (${maxmm_SOURCE_DIR}/lib)

set(LINK_LIBS   maxmm_echo_server
                maxmm
                ${Boost_LIBRARIES} 
                ${apr_LIBRARIES} 
                dl)


include_directories   (./include     )
aux_source_directory  (./src TEST_SRC)
add_library           (maxmm_echo_server SHARED ${TEST_SRC})
target_link_libraries (maxmm_echo_server ${LINK_LIBS})

add_executable        (single_thread_server.tsk single_thread_server.main.cpp)
target_link_libraries (single_thread_server.tsk  ${LINK_LIBS})

add_executable        (multi_process_server.tsk multi_process_server.main.cpp)
target_link_libraries (multi_process_server.tsk  ${LINK_LIBS})

add_executable        (simple_client.tsk simple_client.main.cpp)
target_link_libraries (simple_client.tsk  ${LINK_LIBS})

