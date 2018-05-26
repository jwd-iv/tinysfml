#include "tiny.h"
#include "SFML\Graphics.hpp"
#include "SFML\OpenGL.hpp"
#include "tiny\sfml\gfx\texture.h"

std::string sfmlTexture::extensions() const
{
  return "png jpg bmp tif";
}

bool sfmlTexture::load(tiny::file f)
{
  tex = new sf::Texture;

  if (!tex->loadFromFile(f->filename()))
  {
    delete tex;
    tex = NULL;
  }

  return tex != NULL;
}

bool sfmlTexture::unload()
{
  if (tex != NULL)
  {
    delete tex;
    tex = NULL;
  }

  return tex == NULL;
}
