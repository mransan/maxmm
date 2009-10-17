include_directories   (${maxmm_SOURCE_DIRECTORY}/include)
link_directories      (${maxmm_SOURCE_DIRECTORY})

set(LINK_LIBS     dl maxmm)


add_executable        (interest_rates interest_rates_main.cpp)
target_link_libraries (interest_rates ${LINK_LIBS})
