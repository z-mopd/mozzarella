function(link_include target)
   target_link_libraries(${target} PRIVATE mzrlib)
   target_include_directories(${target} PRIVATE ${pubinclude})
endfunction()

if (BUILD_TESTS)
   add_executable(main
      main.cpp
   )

   link_include(main)
endif()
