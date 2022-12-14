#pragma once

#include <iostream>

#include <rttr/type>
#include <rttr/registration_friend>

#if defined(__REFLECTION_PARSER__)
#   define Meta(...) __attribute__((annotate(#__VA_ARGS__)))
#else
#   define Meta(...)
#endif

#include "Slider.h"
#include "Range.h"
#include "Control.h"
#include "Item.h"
