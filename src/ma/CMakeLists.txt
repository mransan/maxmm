include_directories   (${maxmm_SOURCE_DIR}/include)

add_definitions       (-g)


set(    LINK_LIBS    
        maxmm
        ${libxml++_LIBRARIES} 
        dl)

aux_source_directory  (./ MAXMM_MA_SRC)
add_library           (maxmmma SHARED ${MAXMM_MA_SRC})
target_link_libraries (maxmmma ${LINK_LIBS})

