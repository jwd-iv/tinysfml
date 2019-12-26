#include "tiny.h"
#include "SFML\Graphics.hpp"
#include "SFML\OpenGL.hpp"
#include "tiny\sfml\gfx\renderer.h"
#include "tiny\sfml\gfx\camera.h"
#include "tiny\sfml\gfx\sprite.h"
#include "tiny\sfml\gfx\texture.h"
#include "tiny\editor\inspector.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
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

unsigned long long frame = 0;

void UpdateSpriteCalculations(SFMLSprite& sprite, glm::mat4 parent = glm::mat4(1.f));
void UpdateChildCalculations(tiny::entity* ent, glm::mat4 parent);
void Render(SFMLSprite const& sprite);

bool EntityOrParentHasSprite(tiny::entity* ent)
{
  if (ent == NULL)
    return false;

  return (ent->find<SFMLSprite>() != NULL)
    ||
    EntityOrParentHasSprite(ent->parent);
}

void SFMLRenderer::update(float dt)
{
  timeElapsed += dt;
  ++frame;

  for (auto spacename : engine::get().spaces)
  {
    for (auto& comp : spacename.second->get_all(riku::get<DriftComp>()))
    {
      auto& drift = comp.as<DriftComp>();

      if (drift.sprite.data() == NULL)
        drift.sprite = drift.parent->find<SFMLSprite>();

      if (drift.sprite.data() == NULL)
        throw "DriftComp requires an SFMLSprite!";

      drift.sprite->t.x += drift.speed * dt;
    }

    for (auto& comp : spacename.second->get_all(riku::get<WaveComp>()))
    {
      auto& Wave = comp.as<WaveComp>();

      if (Wave.sprite.data() == NULL)
        Wave.sprite = Wave.parent->find<SFMLSprite>();

      if (Wave.sprite.data() == NULL)
        throw "WaveComp requires an SFMLSprite!";

      Wave.sprite->r = -5 * sin(.8 * frame * dt + Wave.sprite->t.x);
    }

    for (auto& comp : spacename.second->get_all(riku::get<SFMLSprite>()))
    {
      if (!inspected)
      {
        systems::get<inspector>()->inspect(comp);
        inspected = true;
      }

      auto& sprite = comp.as<SFMLSprite>();

      if (sprite.frame != frame && !EntityOrParentHasSprite(sprite.parent->parent))
        UpdateSpriteCalculations(sprite);
    }
  }
}

void SFMLRenderer::close()
{
}

void UpdateSpriteCalculations(SFMLSprite& sprite, glm::mat4 parent)
{
  glm::mat4 calc = glm::mat4(1.f);
  sprite.s.z = 1.f;

  calc = glm::translate(calc, sprite.t);
  calc = glm::rotate(calc, glm::radians(sprite.r), glm::vec3(0.f, 0.f, 1.f));
  calc = glm::scale(calc, sprite.s);

  sprite.calc = parent * calc;
  sprite.calcZ = (sprite.calc * glm::vec4(0.f, 0.f, 0.f, 1.f)).z;

  sprite.frame = frame;

  for (auto& siblingEnt : sprite.parent->children)
    UpdateChildCalculations(siblingEnt.to<tiny::entity>(), sprite.calc);
}

void UpdateChildCalculations(tiny::entity* ent, glm::mat4 parent)
{
  if (ent == NULL)
    return;

  auto sprite = ent->find<SFMLSprite>();
  
  if (sprite != NULL)
  {
    sprite->hasParent = true;
    UpdateSpriteCalculations(*sprite, parent);
  }
  else
    for (auto& child : ent->children)
      UpdateChildCalculations(child.to<tiny::entity>(), parent);
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
      [](tiny::cref a, tiny::cref b) -> bool {
        return a.as<SFMLSprite>().calcZ < b.as<SFMLSprite>().calcZ;
      }
    );

    for (auto& spritevar : sprites)
    {
      auto& sprite = spritevar.as<SFMLSprite>();

      Render(sprite);
    }
  }
}

void Render(SFMLSprite const& sprite)
{
  if (sprite.tex.to<sfmlTexture>() == NULL)
    return;

  glPushMatrix();
  glLoadMatrixf(glm::value_ptr(sprite.calc));

  sf::Texture::bind(sprite.tex->tex);
  glColor4fv(glm::value_ptr(sprite.color));
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MULT);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glPopMatrix();
}
