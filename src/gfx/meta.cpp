#include "tiny.h"
#include "tiny\sfml\gfx\camera.h"
#include "tiny\sfml\gfx\renderer.h"
#include "tiny\sfml\gfx\sprite.h"
#include "tiny\sfml\gfx\texture.h"

rkExportLib(tinySFMLgfx)

rkType(SFMLRenderer, rkParent(tiny::renderer) rkDefaultFactory)

rkType(sfmlTexture, rkParent(tiny::resource) rkDefaultFactory)

rkType(SFMLSprite, rkParent(tiny::component) rkDefaultFactory
  rkMember(pos) rkMember(rot) rkMember(scl) rkMember(tex)
)

rkType(vec3f, rkDefaultFactory
  rkMember(x) rkMember(y) rkMember(z)
)
