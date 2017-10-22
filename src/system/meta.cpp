#include "tiny.h"
#include "tiny\sfml\system\frc.h"
#include "tiny\sfml\system\input.h"
#include "tiny\sfml\system\window.h"

rkExportLib(tinySFMLsys)

rkType(SFMLFRC,      rkParent(tiny::frc) rkStaticFactory)
rkType(SFMLWindow,   rkParent(tiny::window) rkStaticFactory)
//rkType(SFMLKeyboard, rkParent(tiny::input::keyboard) rkStaticFactory)
//rkType(SFMLMouse,    rkParent(tiny::input::mouse) rkStaticFactory)
//rkType(SFMLJoystick, rkParent(tiny::input::device) rkStaticFactory)
