# Copyright 2017 Diego Barrios Romero. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required (VERSION 2.8.11)
project (ReserveBottomMemory)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")

add_library(Lib SHARED src/ReserveBottomMemory.cpp include/ReserveBottomMemory.h)
target_include_directories(Lib PUBLIC include)
set_target_properties(Lib PROPERTIES OUTPUT_NAME ReserveBottomMemory)
target_compile_definitions(Lib PRIVATE RESERVEBOTTOMMEMORY_SOURCE)

enable_testing ()
foreach(TESTNAME NewObject NewArray Malloc VirtualAlloc HeapAlloc)
  add_test (NAME ${TESTNAME}Test COMMAND ${TESTNAME}Test)
  add_executable (${TESTNAME}Test tests/${TESTNAME}Test.cpp)
  target_link_libraries (${TESTNAME}Test LINK_PUBLIC Lib)
endforeach(TESTNAME)

