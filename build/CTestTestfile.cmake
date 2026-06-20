# CMake generated Testfile for 
# Source directory: D:/DME
# Build directory: D:/DME/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(RunAllTests "D:/DME/build/Debug/dsa_tests.exe")
  set_tests_properties(RunAllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/DME/CMakeLists.txt;101;add_test;D:/DME/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(RunAllTests "D:/DME/build/Release/dsa_tests.exe")
  set_tests_properties(RunAllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/DME/CMakeLists.txt;101;add_test;D:/DME/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(RunAllTests "D:/DME/build/MinSizeRel/dsa_tests.exe")
  set_tests_properties(RunAllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/DME/CMakeLists.txt;101;add_test;D:/DME/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(RunAllTests "D:/DME/build/RelWithDebInfo/dsa_tests.exe")
  set_tests_properties(RunAllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/DME/CMakeLists.txt;101;add_test;D:/DME/CMakeLists.txt;0;")
else()
  add_test(RunAllTests NOT_AVAILABLE)
endif()
