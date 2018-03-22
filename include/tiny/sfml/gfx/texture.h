#pragma once

namespace sf
{
  class Texture;
}

struct sfmlTexture : public tiny::resource
{
  // Inherited via resource
  virtual std::string extensions() const;
  virtual bool load(tiny::file f);
  virtual bool unload();

  sf::Texture* tex = NULL;

  rkMetaHandle(sfmlTexture);
};