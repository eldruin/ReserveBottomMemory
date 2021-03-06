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

#include <cstdlib>
#include "helper.h"

struct Object
{
  char c[10];
};

class Test : public MemoryAddressTest
{
public:
  Test()
    : object(NULL)
  { }

private:
  void* allocate()
  {
    object = new Object;
    return object;
  }
  void free()
  {
    delete object;
  }

  Object* object;
};

int main(int, char**)
{
  return Test().run();
}

