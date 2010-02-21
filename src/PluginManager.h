#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>

class PluginManagerPrivate;

class PluginManager : public QObject {
  Q_OBJECT

  public:
    static PluginManager *instance(); // singleton
    PluginManager();
    virtual ~PluginManager();

    void loadPlugins();

  private:
    PluginManagerPrivate *d;
    static PluginManager *m_instance;

    friend class PluginManagerPrivate;
};

#endif // PLUGINMANAGER_H
