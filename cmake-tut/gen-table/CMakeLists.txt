add_executable(MakeTable MakeTable.cpp)
add_custom_command(
  OUTPUT Table.h
  COMMAND MakeTable ${CMAKE_CURRENT_SOURCE_DIR}/Table.h
  DEPENDS MakeTable
  COMMENT "Generate Table.h"
)
add_custom_target(
  gen-table
  DEPENDS Table.h
)