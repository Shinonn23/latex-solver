# Test executables

add_executable(test_lexer test_lexer.cpp)
target_link_libraries(test_lexer PRIVATE latex_core)
add_test(NAME test_lexer COMMAND test_lexer)

add_executable(test_arithmetic test_arithmetic.cpp)
target_link_libraries(test_arithmetic PRIVATE latex_core)
add_test(NAME test_arithmetic COMMAND test_arithmetic)

add_executable(test_errors test_errors.cpp)
target_link_libraries(test_errors PRIVATE latex_core)
add_test(NAME test_errors COMMAND test_errors)

add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser PRIVATE latex_core)
add_test(NAME test_parser COMMAND test_parser)

add_executable(test_variables test_variables.cpp)
target_link_libraries(test_variables PRIVATE latex_core)
add_test(NAME test_variables COMMAND test_variables)
