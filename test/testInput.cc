#include <stdio.h>

#if defined(linux) || defined(__APPLE__)
# define DL_EXPORT
#else
# define DL_EXPORT __declspec(dllexport)
#endif

extern "C"
{

DL_EXPORT int add_ints(int v1, int v2)
{
   return v1 + v2;
}

DL_EXPORT void as_string(char *buffer, int v)
{
   sprintf(buffer, "%d", v);
}

}

