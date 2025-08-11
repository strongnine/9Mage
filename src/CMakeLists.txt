
file( GLOB APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp )

foreach( testsourcefile ${APP_SOURCES} )

    # Used a simple string replace, to cut off .cpp.
    string( REPLACE ".cpp" "" testname ${testsourcefile} )
    find_package( OpenCV REQUIRED )
    add_executable( ${testname} ${testsourcefile} )
    target_link_libraries( ${testname} ${OpenCV_LIBS} )
    # set_target_properties( ${testname} PROPERTIES LINKER_LANGUAGE CXX )

endforeach( testsourcefile ${APP_SOURCES} )

add_subdirectory( methods )