struct SFMLRenderer : public tiny::renderer
{
  // Inherited via renderer
  virtual void initialize();
  virtual void update(float dt);
  virtual void close();
  virtual void render(float);


  rkMetaHook(SFMLRenderer);
};
