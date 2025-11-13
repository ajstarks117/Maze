# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MazeVisualizer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MazeVisualizer_autogen.dir\\ParseCache.txt"
  "MazeVisualizer_autogen"
  )
endif()
