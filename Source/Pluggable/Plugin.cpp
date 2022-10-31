#include "Framework/Plugin.h"

namespace frx
{
	Plugin::Plugin(const String& filename) :
		m_filename(filename),
		m_info(0),
		m_running(false),
		m_loaded(false)
	{

	}

	//Destruction only from the PluginManager
	Plugin::~Plugin()
	{
		assert(!isLoaded() && !isRunning());
	}


	void* Plugin::getSymbolAddress(const ePluginFunction commonFunc) const
	{
		switch (commonFunc)
		{
		case PluginFunction_GetInfo:
			return getSymbolAddress(_T("plugin_GetInfo"));
			break;
		case PluginFunction_Activate:
			return getSymbolAddress(_T("plugin_Activate"));
			break;
		case PluginFunction_Unregister:
			return getSymbolAddress(_T("plugin_Deactivate"));
			break;
		default:
			assert(_T("Common function ID not valid") == 0);
			break;
		}
		return 0;
	}


} //END: frx