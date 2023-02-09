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
			return getSymbolAddress(_T("pluginGetInfo"));
			break;
		case PluginFunction_Activate:
			return getSymbolAddress(_T("pluginActivate"));
			break;
		case PluginFunction_Unregister:
			return getSymbolAddress(_T("pluginDeactivate"));
			break;
		default:
			assert(_T("Common function ID not valid") == 0);
			break;
		}
		return 0;
	}


	//Run the start process
	Bool Plugin::start()
	{
		if (!isLoaded())
		{
			if (!load())
				return false;
		}

		if (!isRunning())
		{
			m_running = true;


			activatePlugin_t process = (Plugin::activatePlugin_t)Plugin::getSymbolAddress(Plugin::PluginFunction_Activate);
			if (frx::isValidPointer(process))
			{
				bool ret = process();
				if (ret)
				{
					logSuccess(_T("DynamicLibrary \"") << m_filename.c_str() << _T("\" started successfully."));
				}
				else
				{
					logError(_T("DynamicLibrary \"") << m_filename.c_str() << _T("\" returned failure from start()."));
				}
				return ret;
			}
			logError(_T("DynamicLibrary \"") << m_filename.c_str() << _T("\" has no pluginActivate() method!"));

		}
		return false;
	}

	//Run the start process
	Bool DynamicLibrary::stop()
	{
		if (isLoaded() && isRunning())
		{
			dectivatePlugin_t process = (Plugin::dectivatePlugin_t)Plugin::getSymbolAddress(Plugin::PluginFunction_Unregister);
			if (frx::isValidPointer(process))
			{
				process();
			}
			else
			{
				logWarning(_T("DynamicLibrary \"") << m_filename.c_str() << _T("\" has no pluginDeactivate() method!"));
			}


			m_running = false;
		}
		return true;
	}


} //END: frx