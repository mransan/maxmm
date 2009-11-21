include_directories   (${maxmm_SOURCE_DIRECTORY}/include)
link_directories      (${maxmm_SOURCE_DIRECTORY})

set(LINK_LIBS     maxmmfinance maxmm dl)


add_executable        (interest_rates.tsk interest_rates.main.cpp)
target_link_libraries (interest_rates.tsk ${LINK_LIBS})
