if (BUILD_TESTS)
   add_executable(main
      main.cpp
   )

   target_link_libraries(main PRIVATE mzrlib)
   target_include_directories(main PRIVATE ${pubinclude})
endif()
