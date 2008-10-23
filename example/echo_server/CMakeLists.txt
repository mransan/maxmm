
include_directories   (${maxmm_SOURCE_DIRECTORY}/include)
link_directories      (${maxmm_SOURCE_DIRECTORY})

set(LINK_LIBS     ${Boost_LIBRARIES} ${apr_LIBRARIES} dl maxmm maxmm_echo_server)


include_directories   (./include     )
aux_source_directory  (./src TEST_SRC)
add_library           (maxmm_echo_server SHARED ${TEST_SRC})
target_link_libraries (maxmm_echo_server ${LINK_LIBS})

add_executable        (single_thread_server single_thread_server_main.cpp)
target_link_libraries (single_thread_server  ${LINK_LIBS})

add_executable        (multi_process_server multi_process_server_main.cpp)
target_link_libraries (multi_process_server  ${LINK_LIBS})

add_executable        (simple_client simple_client_main.cpp)
target_link_libraries (simple_client  ${LINK_LIBS})

