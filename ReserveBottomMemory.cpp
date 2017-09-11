/*
Copyright (c) 2012 Bruce Dawson. All Rights Reserved.
Copyright (c) 2017 Diego Barrios Romero. All Rights Reserved.

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

#include "ReserveBottomMemory.h"
#ifdef _WIN64
#  include <iostream>
#  include <windows.h>
#elif _WIN32
#  pragma message("ReserveBottomMemory is deactivated for 32-bit builds.")
void ReserveBottomMemory() {}
void FreeBottomMemory() {}
#else
#  error "ReserveBottomMemory: This platform is not supported."
#endif

#ifdef _WIN64

static const size_t _NUMBER_OF_MEMORY_HANDLES = 10000;

struct MemoryHandles
{
  MemoryHandles()
    : size(0)
  { }

  void addBlock(void* address)
  {
    static bool warnedLeakage = false;
    if (size >= _NUMBER_OF_MEMORY_HANDLES)
    {
      if (!warnedLeakage)
      {
        OutputDebugStringA("ReserveBottomMemory: Memory address storage capacity exhausted. Memory will be leaked.");
        warnedLeakage = true;
      }
      return;
    }
    handles[size] = address;
    size++;
  }

  void*  handles[_NUMBER_OF_MEMORY_HANDLES];
  size_t size;
};

MemoryHandles* _getVAMemoryHandles()
{
  static MemoryHandles vaHandles;
  return &vaHandles;
}

MemoryHandles* _getHeapMemoryHandles()
{
  static MemoryHandles heapHandles;
  return &heapHandles;
}

MemoryHandles* _getMallocMemoryHandles()
{
  static MemoryHandles mallocHandles;
  return &mallocHandles;
}

bool* isInitialized()
{
  static bool initialized = false;
  return &initialized;
}

void ReserveBottomMemory()
{
  bool* initialized = isInitialized();
  if (*initialized)
    return;
  *initialized = true;

  // Start by reserving large blocks of address space, and then
  // gradually reduce the size in order to capture all of the
  // fragments. Technically we should continue down to 64 KB but
  // stopping at 1 MB is sufficient to keep most allocators out.

  const size_t LOW_MEM_LINE = 0x100000000LL;
  size_t totalReservation = 0;
  size_t numVAllocs = 0;
  size_t numHeapAllocs = 0;
  size_t oneMB = 1024 * 1024;
  MemoryHandles* vaHandles = _getVAMemoryHandles();
  for (size_t size = 256 * oneMB; size >= oneMB; size /= 2)
  {
    for (;;)
    {
      void* p = VirtualAlloc(0, size, MEM_RESERVE, PAGE_NOACCESS);
      if (!p)
        break;

      if ((size_t)p >= LOW_MEM_LINE)
      {
        // We don't need this memory, so release it completely.
        VirtualFree(p, 0, MEM_RELEASE);
        break;
      }

      vaHandles->addBlock(p);
      ++numVAllocs;
      totalReservation += size;
    }
  }

  // Now repeat the same process but making heap allocations, to use up
  // the already reserved heap blocks that are below the 4 GB line.
  HANDLE heap = GetProcessHeap();
  MemoryHandles* heapHandles = _getHeapMemoryHandles();
  for (size_t blockSize = 64 * 1024; blockSize >= 16; blockSize /= 2)
  {
    for (;;)
    {
      void* p = HeapAlloc(heap, 0, blockSize);
      if (!p)
        break;

      if ((size_t)p >= LOW_MEM_LINE)
      {
        // We don't need this memory, so release it completely.
        HeapFree(heap, 0, p);
        break;
      }

      heapHandles->addBlock(p);
      ++numHeapAllocs;
      totalReservation += blockSize;
    }
  }

  // Perversely enough the CRT doesn't use the process heap. Suck up
  // the memory the CRT heap has already reserved.
  MemoryHandles* mallocHandles = _getMallocMemoryHandles();
  for (size_t blockSize = 64 * 1024; blockSize >= 16; blockSize /= 2)
  {
    for (;;)
    {
      void* p = malloc(blockSize);
      if (!p)
        break;

      if ((size_t)p >= LOW_MEM_LINE)
      {
        // We don't need this memory, so release it completely.
        free(p);
        break;
      }

      mallocHandles->addBlock(p);
      ++numHeapAllocs;
      totalReservation += blockSize;
    }
  }

  // Print diagnostics showing how many allocations we had to make in
  // order to reserve all of low memory, typically less than 200.
  char buffer[1000];
  sprintf_s(buffer, "Reserved %1.3f MB (%d vallocs,"
    "%d heap allocs) of low-memory.\n",
    totalReservation / (1024 * 1024.0),
    (int)numVAllocs, (int)(numHeapAllocs));
  OutputDebugStringA(buffer);
  std::cout << buffer << std::endl;
}

void FreeBottomMemory()
{
  MemoryHandles* vaHandles = _getVAMemoryHandles();
  for (size_t i = 0; i < vaHandles->size; ++i)
  {
    VirtualFree(vaHandles->handles[i], 0, MEM_RELEASE);
    vaHandles->handles[i] = NULL;
  }
  vaHandles->size = 0;


  HANDLE heap = GetProcessHeap();
  MemoryHandles* heapHandles = _getHeapMemoryHandles();
  for (size_t i = 0; i < heapHandles->size; ++i)
  {
    HeapFree(heap, 0, heapHandles->handles[i]);
    heapHandles->handles[i] = NULL;
  }
  heapHandles->size = 0;

  MemoryHandles* mallocHandles = _getMallocMemoryHandles();
  for (size_t i = 0; i < mallocHandles->size; ++i)
  {
    free(mallocHandles->handles[i]);
    mallocHandles->handles[i] = NULL;
  }
  mallocHandles->size = 0;

  *isInitialized() = false;
}

#endif // _WIN64

