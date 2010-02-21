#ifndef PLUGINMANAGER_P_H
#define PLUGINMANAGER_P_H

class PluginManager;

class PluginManagerPrivate
{
  public:
    PluginManagerPrivate(PluginManager *pluginManager);

    void loadPlugins();

  private:
    PluginManager *q;
};

#endif // PLUGINMANAGER_P_H
