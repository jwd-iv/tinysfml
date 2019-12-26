#include "tiny.h"
#include "tiny\sfml\gfx\camera.h"
#include "tiny\sfml\gfx\renderer.h"
#include "tiny\sfml\gfx\sprite.h"
#include "tiny\sfml\gfx\texture.h"

rkExportLib(tinySFMLgfx)

rkType(SFMLRenderer, rkParent(tiny::renderer) rkDefaultFactory)

rkType(sfmlTexture, rkParent(tiny::resource) rkDefaultFactory)

rkType(SFMLSprite, rkParent(tiny::transform) rkDefaultFactory
  rkMember(t) rkMember(r) rkMember(s) rkMember(tex) rkMember(color)
)

rkType(WaveComp, rkParent(tiny::component) rkDefaultFactory
)

rkType(DriftComp, rkParent(tiny::component) rkDefaultFactory
  rkMember(speed)
)
