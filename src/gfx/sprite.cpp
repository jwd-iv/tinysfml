#include "tiny.h"
#include "tiny\sfml\gfx\sprite.h"

glm::vec3 SFMLSprite::pos() const
{
  return t;
}

glm::vec3 SFMLSprite::scl() const
{
  return s;
}

float SFMLSprite::rot() const
{
  return r;
}

bool SFMLSprite::set_pos(glm::vec3 pos)
{
  t = pos;
  return true;
}

bool SFMLSprite::set_scl(glm::vec3 scl)
{
  s = scl;
  return true;
}

bool SFMLSprite::set_rot(float rot)
{
  r = rot;
  return true;
}
