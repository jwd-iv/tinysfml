#include "tiny.h"

rkLink(tiny_core)
rkLink(tinySFMLsys)

int main(void)
{
  tiny::engine::get().initialize();
  tiny::engine::get().run();
  tiny::engine::get().close();
  
  return 0;
}