#set(Boost_USE_STATIC_LIBS ON)
#set(CMAKE_WARN_DEPRECATED FALSE)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

include_directories(${PROJECT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

add_executable(ProgramOptions_test ProgramOptions_test.cpp)
target_link_libraries(ProgramOptions_test  LTEClasses simple-web-server ${Boost_LIBRARIES})
add_test(NAME ProgramOptions_test COMMAND ProgramOptions_test)

add_executable(HuaweiClient_test HuaweiClient_test.cpp)
target_link_libraries(HuaweiClient_test LTEClasses simple-web-server ${Boost_LIBRARIES})
add_test(NAME HuaweiClient_test COMMAND HuaweiClient_test)

add_executable(AngularServer_test AngularServer_test.cpp)
#target_compile_definitions(AngularServer_test PUBLIC "-Wno-deprecated-declarations")
target_link_libraries(AngularServer_test LTEClasses  simple-web-server ${Boost_LIBRARIES})
add_test(NAME AngularServer_test COMMAND AngularServer_test)

