if (CONFIG_LIB_OSTENTUS)
  zephyr_include_directories(include)
  zephyr_library_sources(libostentus.c)
endif()
