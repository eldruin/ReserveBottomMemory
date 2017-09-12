/*
Copyright 2017 Diego Barrios Romero. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef HELPER_H__
#define HELPER_H__

#include <ReserveBottomMemory.h>

class MemoryAddressTest
{
  virtual void* allocate() = 0;
  virtual void  free() = 0;

public:
  int run()
  {
    int returnCode = 0;
    ReserveBottomMemory();

    void* address = allocate();

    const size_t LOW_MEM_LINE = 0x100000000LL;
    if (reinterpret_cast<size_t>(address) < LOW_MEM_LINE)
    {
      returnCode = 1;
    }

    free();

    FreeBottomMemory();

    return returnCode;
  }
};

#endif // HELPER_H__

