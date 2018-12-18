#include "tiny.h"
#include "SFML\Graphics.hpp"
#include "SFML\OpenGL.hpp"
#include "tiny\sfml\gfx\renderer.h"
#include "tiny\sfml\gfx\camera.h"
#include "tiny\sfml\gfx\sprite.h"
#include "tiny\sfml\gfx\texture.h"
#include "tiny\editor\inspector.h"
#include <algorithm>

static const GLfloat square[] =
{
  -.5f, 0.f, 0.f, 0.f, 1.f,
   .5f, 0.f, 0.f, 1.f, 1.f,
  -.5f, 1.f, 0.f, 0.f, 0.f,
  -.5f, 1.f, 0.f, 0.f, 0.f,
   .5f, 0.f, 0.f, 1.f, 1.f,
   .5f, 1.f, 0.f, 1.f, 0.f
};

static float timeElapsed = .0f;

using namespace tiny;

void SFMLRenderer::initialize()
{
  // Enable Z-buffer read and write
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glClearDepth(1.f);

  // Disable lighting
  glDisable(GL_LIGHTING);

  // Configure the viewport (the same size as the window)
  glViewport(0, 0, systems::get<window>()->width(), systems::get<window>()->height());

  // Setup a perspective projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLfloat ratio = float(systems::get<window>()->width()) / float(systems::get<window>()->height());
  glOrtho(-ratio * .5f, ratio * .5f, 0.f, 1.f, 0.f, 1000.f);

  // Enable position and texture coordinates vertex components
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), square);
  glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), square + 3);

  // Disable normal and color vertex components
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY); 
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool inspected = false;

void SFMLRenderer::update(float dt)
{
  timeElapsed += dt;

  if(!inspected)
  for (auto spacename : engine::get().spaces)
  {
    for (auto& sprite : spacename.second->get_all(riku::get<SFMLSprite>()))
    {
      systems::get<inspector>()->inspect(sprite);
      inspected = true;
      break;
    }

    if (inspected)
      break;
  }
}

void SFMLRenderer::close()
{
}

void Render(SFMLSprite const& sprite)
{
  glPushMatrix();
  glLoadIdentity();
  sf::Texture::bind(sprite.tex->tex);

  glTranslatef(sprite.t.x, sprite.t.y, -sprite.t.z);
  glRotatef(sprite.r, 0.f, 0.f, 1.f);
  glScalef(sprite.s.x, sprite.s.y, 1.f);

  for (auto const& child : sprite.parent->children())
  {
    auto var = child.as<entity>().find<SFMLSprite>();
    if (var != NULL)
      Render(*var);
  }

  // Draw the cube
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glPopMatrix();
}

void SFMLRenderer::render(float)
{
  glEnable(GL_TEXTURE_2D);

  // Clear the depth buffer
  glClear(GL_DEPTH_BUFFER_BIT);

  // Apply some transformations
  glMatrixMode(GL_MODELVIEW);

  for (auto space : engine::get().spaces)
  {
    if (space.second.data() == NULL || !space.second->active)
      continue;

    auto sprites = space.second->get_all(riku::get<SFMLSprite>()).vec;

    std::sort(sprites.begin(), sprites.end(),
      [](rk::variant const& a, rk::variant const& b) -> bool {
        return a.as<SFMLSprite>().t.z > b.as<SFMLSprite>().t.z;
      }
    );

    for (auto& spritevar : sprites)
    {
      auto& sprite = spritevar.as<SFMLSprite>();

      if (sprite.parent->parent.data() != NULL)
        continue;

      Render(sprite);
    }
  }
}
