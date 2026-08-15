function(link_include target)
   target_link_libraries(${target} PRIVATE mzrlib)
   target_include_directories(${target} PRIVATE ${pubinclude})
endfunction()

if (BUILD_TESTS)
   add_executable(main
      main.cpp
   )

   add_executable(multi_window
      multi_window.cpp
   )

   add_executable(static_drawing
      static_drawing.cpp
   )

   add_executable(batch_drawing
      batch_drawing.cpp
   )

   link_include(main)
   link_include(multi_window)
   link_include(static_drawing)
   link_include(batch_drawing)
endif()
