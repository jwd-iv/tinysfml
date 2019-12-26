#pragma once

#include "texture.h"
#include "core/transform.h"

struct SFMLSprite : public tiny::transform
{
  glm::vec3 s = { 1.f, 1.f, 1.f };
  float r;
  glm::vec3 t = { 0.f, 0.f, 0.f };

  glm::mat4 calc;
  float calcZ;
  bool hasParent = false;

  sfmlTexture::handle tex;
  glm::vec4 color = { 1.f, 1.f, 1.f, 1.f };
  unsigned frame = -1;

  rkMetaHandle(SFMLSprite);

  // Inherited via transform
  virtual glm::vec3 pos() const;
  virtual glm::vec3 scl() const;
  virtual float rot() const;
  virtual bool set_pos(glm::vec3);
  virtual bool set_scl(glm::vec3);
  virtual bool set_rot(float);
};

struct WaveComp : public tiny::component
{
  riku::var<SFMLSprite> sprite;

  rkMetaHandle(WaveComp);
};

struct DriftComp : public tiny::component
{
  riku::var<SFMLSprite> sprite;
  float speed = .0f;

  rkMetaHandle(DriftComp);
};
