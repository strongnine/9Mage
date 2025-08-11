
file( GLOB APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp )
find_package( OpenCV REQUIRED )

foreach( testsourcefile ${APP_SOURCES} )

    # Used a simple string replace, to cut off .cpp.
    string( REPLACE ".cpp" "" testname ${testsourcefile} )
    add_executable( ${testname} ${testsourcefile} )
    target_link_libraries( ${testname} ${OpenCV_LIBS} )
    # set_target_properties( ${testname} PROPERTIES LINKER_LANGUAGE CXX )

endforeach( testsourcefile ${APP_SOURCES} )
