# CMake build examples

message( STATUS "Runtimes are stored in ${CMAKE_CURRENT_BINARY_DIR}" )

set(run_examples_list
  example1
  example2
  example3
  example4
  example5
  example6
)

foreach(example_name ${run_examples_list})
  build_source_file(${example_name})
  set_target_properties(${example_name} PROPERTIES FOLDER "run examples")
endforeach(example_name)
