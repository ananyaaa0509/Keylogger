#include "../input.h"
#include "input_platform.h"
#ifdef _WIN32
  #include "input_windows.h"
#elif defined(__linux__)
   #include "input_linux.h"
#elif defined(__APPLE__)
   #include "input_macos.h"
#endif



KeyEvent get_event(void)
{
#ifdef _WIN32
  return windows_get_event();
#elif defined(__linux__)
  return linux_get_event();
#elif defined(__APPLE__)
  return macos_get_event();
#else
  return create_event(KEY_ESCAPE, 0, 0, 0, 0);
#endif
}