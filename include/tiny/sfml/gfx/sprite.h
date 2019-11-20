#pragma once

#include "texture.h"
#include "core/transform.h"

struct SFMLSprite : public tiny::transform
{
  glm::vec3 t, s = { 1.f, 1.f, 1.f };
  float r;
  sfmlTexture::handle tex;

  rkMetaHandle(SFMLSprite);

  // Inherited via transform
  virtual glm::vec3 pos() const;
  virtual glm::vec3 scl() const;
  virtual float rot() const;
  virtual bool set_pos(glm::vec3);
  virtual bool set_scl(glm::vec3);
  virtual bool set_rot(float);
};
