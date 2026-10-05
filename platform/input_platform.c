#include "../input.h"
#include "input_platform.h"
#ifdef _WIN32
  #include "input_windows.h"
#elif defined(__linux__)
   #include "input_linux.h"
#elif defined(__APPLE__)
   #include "input_macos.h"
#endif



InputResult start_platform_input(void)
{
#ifdef _WIN32
  return windows_start_platform_input();
#elif defined(__linux__)
  return linux_start_platform_input();
#elif defined(__APPLE__)
  return macos_start_platform_input();
#else
  return create_event(KEY_ESCAPE, 0, 0, 0, 0);
#endif
}