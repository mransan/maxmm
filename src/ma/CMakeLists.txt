include_directories   (${maxmm_SOURCE_DIRECTORY}/include)
link_directories      (${maxmm_SOURCE_DIRECTORY})
add_definitions       (-g)


set(    LINK_LIBS     
        ${Boost_LIBRARIES} 
        ${apr_LIBRARIES}
        ${libxml++_LIBRARIES} 
        dl 
        maxmm )


aux_source_directory  (./ MAXMM_MA_SRC)
add_library           (maxmmma SHARED ${MAXMM_MA_SRC})
target_link_libraries (maxmmma ${LINK_LIBS})

