#include "tiny.h"
#include "SFML\Graphics.hpp"
#include "SFML\OpenGL.hpp"
#include "tiny\sfml\gfx\renderer.h"
#include "tiny\sfml\gfx\camera.h"
#include "tiny\sfml\gfx\sprite.h"
#include "tiny\sfml\gfx\texture.h"

// Define a 3D cube (6 faces made of 2 triangles composed by 3 vertices)
static const GLfloat cube[] =
{
  // positions    // texture coordinates
  -20, -20, -20,  0, 0,
  -20,  20, -20,  1, 0,
  -20, -20,  20,  0, 1,
  -20, -20,  20,  0, 1,
  -20,  20, -20,  1, 0,
  -20,  20,  20,  1, 1,

  20, -20, -20,  0, 0,
  20,  20, -20,  1, 0,
  20, -20,  20,  0, 1,
  20, -20,  20,  0, 1,
  20,  20, -20,  1, 0,
  20,  20,  20,  1, 1,

  -20, -20, -20,  0, 0,
   20, -20, -20,  1, 0,
  -20, -20,  20,  0, 1,
  -20, -20,  20,  0, 1,
   20, -20, -20,  1, 0,
   20, -20,  20,  1, 1,

  -20,  20, -20,  0, 0,
   20,  20, -20,  1, 0,
  -20,  20,  20,  0, 1,
  -20,  20,  20,  0, 1,
   20,  20, -20,  1, 0,
   20,  20,  20,  1, 1,

  -20, -20, -20,  0, 0,
   20, -20, -20,  1, 0,
  -20,  20, -20,  0, 1,
  -20,  20, -20,  0, 1,
   20, -20, -20,  1, 0,
   20,  20, -20,  1, 1,

  -20, -20,  20,  0, 0,
   20, -20,  20,  1, 0,
  -20,  20,  20,  0, 1,
  -20,  20,  20,  0, 1,
   20, -20,  20,  1, 0,
   20,  20,  20,  1, 1
};

sf::Texture* tex = NULL;
static float timeElapsed = .0f;

void SFMLRenderer::initialize()
{
  // Load a texture to apply to our 3D cube
  tex = new sf::Texture;
  if (!tex->loadFromFile("texture.jpg"))
    DebugBreak();

  // Enable Z-buffer read and write
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glClearDepth(1.f);

  // Disable lighting
  glDisable(GL_LIGHTING);

  // Configure the viewport (the same size as the window)
  glViewport(0, 0, tiny::systems::get<tiny::window>()->width(), tiny::systems::get<tiny::window>()->height());

  // Setup a perspective projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLfloat ratio = static_cast<float>(tiny::systems::get<tiny::window>()->width()) / tiny::systems::get<tiny::window>()->height();
  glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

  // Enable position and texture coordinates vertex components
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), cube);
  glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), cube + 3);

  // Disable normal and color vertex components
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void SFMLRenderer::update(float dt)
{
  timeElapsed += dt;
}

void SFMLRenderer::close()
{
  delete tex;
}

void SFMLRenderer::render(float)
{
  glEnable(GL_TEXTURE_2D);
  sf::Texture::bind(tex);

  // Clear the depth buffer
  glClear(GL_DEPTH_BUFFER_BIT);

  // Apply some transformations
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -100.f);
  glRotatef(timeElapsed * 50.f, 1.f, 0.f, 0.f);
  glRotatef(timeElapsed * 30.f, 0.f, 1.f, 0.f);
  glRotatef(timeElapsed * 90.f, 0.f, 0.f, 1.f);

  // Draw the cube
  glDrawArrays(GL_TRIANGLES, 0, 36);
}
