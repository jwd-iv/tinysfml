#pragma once

class SFMLWindow : public tiny::window
{
public:

  // Inherited via Window

  virtual void initialize();
  virtual void update(float);
  virtual void render();
  virtual void close();

  virtual bool is_active() const;
  virtual bool show();
  virtual bool hide();
  virtual unsigned width() const;
  virtual unsigned height() const;
  virtual bool is_fullscreen() const;
  virtual bool set_size(unsigned w, unsigned h, bool fullscreen);

  rkMetaHook(SFMLWindow);
};
