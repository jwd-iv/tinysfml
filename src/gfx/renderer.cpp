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
  -20, -20, 0,  0, 0,
   20, -20, 0,  1, 0,
  -20,  20, 0,  0, 1,
  -20,  20, 0,  0, 1,
   20, -20, 0,  1, 0,
   20,  20, 0,  1, 1
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
  GLfloat ratio = static_cast<float>(systems::get<window>()->width()) / systems::get<window>()->height();
  glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

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
        return a.as<SFMLSprite>().pos.z > b.as<SFMLSprite>().pos.z;
      }
    );

	  for (auto& spritevar : sprites)
	  {
		  auto& sprite = spritevar.as<SFMLSprite>();

		  glPushMatrix();
		  glLoadIdentity();
		  sf::Texture::bind(sprite.tex->tex);

		  glTranslatef(-sprite.pos.x, -sprite.pos.y, -sprite.pos.z);
		  glRotatef(timeElapsed * sprite.rot.x, 1.f, 0.f, 0.f);
		  glRotatef(timeElapsed * sprite.rot.y, 0.f, 1.f, 0.f);
		  glRotatef(timeElapsed * sprite.rot.z, 0.f, 0.f, 1.f);

		  // Draw the cube
		  glDrawArrays(GL_TRIANGLES, 0, 6);

		  glPopMatrix();
	  }
  }
}
