#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

#include <rttr/registration.h>
#include <rttr/registration_friend>

#if defined(__REFLECTION_PARSER__)
#   define Meta(...) __attribute__((annotate(#__VA_ARGS__)))
#else
#   define Meta(...)
#endif