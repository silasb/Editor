#include "PluginManager.h"
#include "PluginManager_p.h"

#include "lua.hpp"

#include "luna.h"

PluginManager *PluginManager::m_instance = 0;

PluginManager *PluginManager::instance()
{
  return m_instance;
}

PluginManager::~PluginManager()
{
  delete d;
  d = 0;
}

PluginManager::PluginManager() : d(new PluginManagerPrivate(this))
{
  m_instance = this;
}

void PluginManager::loadPlugins()
{
  return d->loadPlugins();
}


// Private stuff //

PluginManagerPrivate::PluginManagerPrivate(PluginManager *pluginManager)
  : q(pluginManager)
{
  lua_State *L = lua_open();

  luaL_openlibs(L);

  //lua_register(L, "register_callback", register_lua_callback);

  int lErr = 0;
  if((lErr = luaL_dofile(L, "plugins/test.lub")) == 0)
  {
    //lua_cpcall(L, &test_call, 0);
  }
  else
  {
    //cout << "error opeing file\n";
  }

  lua_close(L);
}

void PluginManagerPrivate::loadPlugins()
{

}
