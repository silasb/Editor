#include <QtGui/QApplication>
#include <QStringList>
#include <QDir>

#include "PluginManager.h"

#include "mainwindow.h"

#include <iostream>

using namespace std;

#include "lua.hpp"

#include "luna.h"

int register_lua_callback(lua_State *L)
{
  int argc = lua_gettop(L);

  for(int i = 1; i<=argc; ++i) {
    cout << "-- argument " << i << ": " << lua_tostring(L, i) << endl;
  }

  lua_pushnumber(L, true); // return value back to lua
  return 1;
}

static int test_call(lua_State *L)
{
  lua_settop(L, 0);
  lua_pushliteral(L, "_TRACEBACK");
  lua_rawget(L, LUA_GLOBALSINDEX);
  int tb = lua_gettop(L);

  lua_pushliteral(L, "my_function");
  lua_gettable(L, LUA_GLOBALSINDEX);
  lua_pcall(L, 0, 1, tb);
  return 0;
}

static inline QStringList getPluginsPath()
{
  QStringList rc;
  QDir rootDir = QApplication::applicationDirPath();
  rootDir.cdUp();
  const QString rootDirPath = rootDir.canonicalPath();
  QString pluginPath = rootDirPath;
  pluginPath += "/";
  pluginPath += "plugins";
  rc.push_back(pluginPath);
  return rc;
}

int main(int argc, char *argv[])
{

  PluginManager pluginManager;
  //pluginManager.setFileExtension("pluginspec");
  // get plugin path
  const QStringList pluginPaths = getPluginsPath();
  // pluginManager.setPluginPaths(pluginPaths);

  //pluginManager.loadPlugins();

  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  //a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
  return a.exec();
}
