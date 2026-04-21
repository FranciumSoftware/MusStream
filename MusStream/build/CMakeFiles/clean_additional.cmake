# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appMusStream_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appMusStream_autogen.dir\\ParseCache.txt"
  "appMusStream_autogen"
  )
endif()
