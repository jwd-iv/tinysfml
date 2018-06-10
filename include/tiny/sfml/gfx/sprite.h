#pragma once

#include "texture.h"

struct vec3f
{
  float x, y, z;
};

struct SFMLSprite : public tiny::component
{
  vec3f pos, rot, scl = { 1.f, 1.f, 1.f };
  sfmlTexture::handle tex;

  rkMetaHandle(SFMLSprite);
};
