#pragma once

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef DEBUG
    #define GLCall(x) do { GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__)) } while (0);
#else
    #define GLCall(x) x
#endif