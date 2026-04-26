#pragma once

#if NIMONSPOLY_ENABLE_RAYLIB
#define Color RaylibColor
#include <raylib.h>
#undef Color
#ifdef RED
#undef RED
#endif
#ifdef PINK
#undef PINK
#endif
#ifdef BROWN
#undef BROWN
#endif
#ifdef ORANGE
#undef ORANGE
#endif
#ifdef YELLOW
#undef YELLOW
#endif
#ifdef GREEN
#undef GREEN
#endif
#ifdef GRAY
#undef GRAY
#endif

inline constexpr RaylibColor RL_WHITE{255, 255, 255, 255};
inline constexpr RaylibColor RL_BLACK{0, 0, 0, 255};
inline constexpr RaylibColor RL_BLANK{0, 0, 0, 0};
#endif
