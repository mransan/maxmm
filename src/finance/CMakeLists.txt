include_directories   (${maxmm_SOURCE_DIRECTORY}/include)

add_definitions       (-g)


aux_source_directory  (./ MAXMM_MA_SRC)
add_library           (maxmmfinance SHARED ${MAXMM_MA_SRC})

