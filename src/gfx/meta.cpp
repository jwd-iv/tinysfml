#include "tiny.h"
#include "tiny\sfml\gfx\camera.h"
#include "tiny\sfml\gfx\renderer.h"
#include "tiny\sfml\gfx\sprite.h"
#include "tiny\sfml\gfx\texture.h"

rkExportLib(tinySFMLgfx)

rkType(SFMLRenderer, rkParent(tiny::renderer) rkDefaultFactory)

rkType(SFMLSprite, rkParent(tiny::component) rkDefaultFactory)
