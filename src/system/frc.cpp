#include "tiny.h"
#include "tiny\sfml\system\frc.h"
#include "SFML\System.hpp"

void SFMLFRC::initialize()
{
  t = 0.0f;
  clock = new sf::Clock();

  //initialize timer
  currentTime = clock->getElapsedTime().asSeconds();
  accumulator = 0.0f;
}

void SFMLFRC::close()
{
  delete clock;
}

void SFMLFRC::begin_frame()
{
  ++frames;
  float newTime = clock->getElapsedTime().asSeconds();
  float frameTime = newTime - currentTime;
  if (frameTime > 0.05f)
    frameTime = 0.05f;
  previousFrame = currentTime;
  currentTime = newTime;
  accumulator += frameTime;
}

bool SFMLFRC::time_left() const
{
  return accumulator >= dt();
}

void SFMLFRC::tick()
{
  t += dt();
  accumulator -= dt();
  ++ticks;
}

float SFMLFRC::dt() const
{
  return tickDuration;
}

float SFMLFRC::df() const
{
  return clock->getElapsedTime().asSeconds() - previousFrame;
}

float SFMLFRC::alpha() const
{
  return accumulator / dt();
}
