#pragma once
#include "SFML\System.hpp"

class SFMLFRC : public tiny::frc
{
public:
  float tickDuration = .01f;
  float t, currentTime, previousFrame, accumulator;
  sf::Clock* clock;
  unsigned long long ticks = 0, frames = 0;

  virtual void initialize();
  virtual void close();

  virtual void begin_frame();
  virtual void tick();

  virtual bool time_left() const;
  virtual float dt() const;
  virtual float df() const;
  virtual float alpha() const;

  rkMetaHook(SFMLFRC);
};
