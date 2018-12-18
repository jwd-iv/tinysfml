#include "tiny.h"
#include "tiny\sfml\system\window.h"
#include "SFML\System.hpp"
#include "SFML\Window.hpp"
#include "SFML\OpenGL.hpp"
#include "tiny\editor\inspector.h"

static sf::Window* g_sWindow = NULL;

void SFMLWindow::initialize()
{
  if (g_sWindow != NULL)
    return;
  // Request a 24-bits depth buffer when creating the window
  sf::ContextSettings contextSettings;
  contextSettings.depthBits = 24;
  contextSettings.sRgbCapable = false;
  // Create the main window
  g_sWindow = new sf::Window(sf::VideoMode(1600, 900), "SFML window with OpenGL", sf::Style::Default, contextSettings);
  //g_sWindow = new sf::Window(sf::VideoMode::getDesktopMode(), "SFML window with OpenGL", sf::Style::None, contextSettings);
  g_sWindow->setVerticalSyncEnabled(true);

  // Make the window the active target for OpenGL calls
  // Note: If using sf::Texture or sf::Shader with OpenGL,
  // be sure to call sf::Texture::getMaximumSize() and/or
  // sf::Shader::isAvailable() at least once before calling
  // setActive(), as those functions will cause a context switch
  g_sWindow->setActive();

  // Enable Z-buffer read and write
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glClearDepth(1.f);

  // Disable lighting
  glDisable(GL_LIGHTING);

  // Configure the viewport (the same size as the window)
  glViewport(0, 0, g_sWindow->getSize().x, g_sWindow->getSize().y);

  // Setup a perspective projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLfloat ratio = static_cast<float>(g_sWindow->getSize().x) / g_sWindow->getSize().y;
  glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);
}

void SFMLWindow::update(float dt)
{
  if (g_sWindow == NULL)
    return;

  sf::Event event;
  while (g_sWindow->pollEvent(event))
  {
    if (tiny::systems::get<tiny::inspector>()->receiveWindowEvent(&event))
      continue;

    // close window: exit
    if (event.type == sf::Event::Closed)
      tiny::engine::get().stop();

    // Escape key: exit
    if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
      tiny::engine::get().stop();

    // Adjust the viewport when the window is resized
    if (event.type == sf::Event::Resized)
      glViewport(0, 0, event.size.width, event.size.height);
  }

  // Clear the depth buffer
  glClear(GL_DEPTH_BUFFER_BIT);
  g_sWindow->setActive();
}

void SFMLWindow::close()
{
  g_sWindow->close();
  delete g_sWindow;
  g_sWindow = NULL;
}

bool SFMLWindow::is_active() const
{
  return g_sWindow != NULL && g_sWindow->isOpen() && g_sWindow->hasFocus();
}

bool SFMLWindow::show()
{
  if (g_sWindow == NULL)
    return false;

  g_sWindow->requestFocus();
  return g_sWindow->hasFocus();
}

bool SFMLWindow::hide()
{
  if (g_sWindow == NULL)
    return false;

  g_sWindow->setVisible(false);
  return !g_sWindow->hasFocus();
}

void SFMLWindow::render()
{
  if (g_sWindow != NULL)
    g_sWindow->display();
  glClear(GL_COLOR_BUFFER_BIT);
}

unsigned SFMLWindow::width() const
{
  return g_sWindow == NULL ? 0 : g_sWindow->getSize().x;
}

unsigned SFMLWindow::height() const
{
  return g_sWindow == NULL ? 0 : g_sWindow->getSize().y;
}

bool SFMLWindow::is_fullscreen() const
{
  return false;
}

bool SFMLWindow::set_size(unsigned w, unsigned h, bool fullscreen)
{
  g_sWindow->setSize(sf::Vector2u(w, h));
  return width() == w && height() == h && is_fullscreen() == fullscreen;
}
