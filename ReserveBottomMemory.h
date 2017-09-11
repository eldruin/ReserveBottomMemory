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

#ifndef RESERVEBOTTOMMEMORY_H__
#define RESERVEBOTTOMMEMORY_H__

#ifdef _WIN32
#  define RESERVEBOTTOMMEMORY_SYMBOL_EXPORT __declspec(dllexport)
#  define RESERVEBOTTOMMEMORY_SYMBOL_IMPORT __declspec(dllimport)
#else
#  define RESERVEBOTTOMMEMORY_SYMBOL_EXPORT
#  define RESERVEBOTTOMMEMORY_SYMBOL_IMPORT
#endif

#ifndef RESERVEBOTTOMMEMORY_API
#  ifdef RESERVEBOTTOMMEMORY_SOURCE
#    define RESERVEBOTTOMMEMORY_API RESERVEBOTTOMMEMORY_SYMBOL_EXPORT
#  else
#    define RESERVEBOTTOMMEMORY_API RESERVEBOTTOMMEMORY_SYMBOL_IMPORT
#  endif
#endif

void RESERVEBOTTOMMEMORY_API ReserveBottomMemory();
void RESERVEBOTTOMMEMORY_API FreeBottomMemory();

#endif // RESERVEBOTTOMMEMORY_H__

