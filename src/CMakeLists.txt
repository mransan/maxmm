include_directories  (${Urgen_SOURCE_DIRECTORY}/include)
aux_source_directory (./ utils)
add_library          (urbotUtils SHARED ${utils})
