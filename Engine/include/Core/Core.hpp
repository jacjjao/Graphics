#pragma once

#ifdef EG_ENABLE_ASSERTS
#define EG_ASSERT(x, ...) { if(!(x)) { EG_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#define EG_CORE_ASSERT(x, ...) { if (!(x)) { EG_CORE_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#else
#define EG_ASSERT(x, ...)
#define EG_CORE_ASSERT(x, ...)
#endif