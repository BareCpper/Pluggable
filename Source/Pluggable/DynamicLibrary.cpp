#include "Win32Plugin.h"

#include "Framework/Log.h"

frx::Plugin* frx::Plugin::create( const Char* filename )
{ return new win32::Win32Plugin( filename ); }

namespace win32
{
	//Construction only from the PluginManager
	Win32Plugin::Win32Plugin( const String& filename ) :
		frx::Plugin( filename ),
		m_hwnd( 0 )
	{
	
	}

	//Destruction only from the PluginManager
	Win32Plugin::~Win32Plugin()
	{
		unload();
	}

#if 0
	bool printLibError()
	{
#if _WIN32
		std::cerr << GetLastError() << std::endl;
#else
		std::cerr << dlerror() << std::endl;
#endif
		return false;
	}

	void* openLib(const std::string& libName)
	{
#if _WIN32
		return LoadLibrary((libName + ".dll").c_str());
#else
		return dlopen((libName + ".so").c_str(), RTLD_LAZY);
#endif
	}

	int closeLib(void* libHandle)
	{
#if _WIN32
		return FreeLibrary((HMODULE)libHandle);
#else
		return dlclose(libHandle);
#endif
	}

	void* loadSymbol(void* libHandle, const char* sym)
	{
#if _WIN32
		return (void*)GetProcAddress((HMODULE)libHandle, sym);
#else
		return dlsym(libHandle, sym);
#endif
	}
#endif
	//Load the library
	Bool Win32Plugin::load()
	{	
		m_loaded = ( m_hwnd!= 0 );
		
		if ( m_loaded )
			return true;

		m_running = false;
		
		if ( !m_filename )
		{
			logError( _T("Win32Plugin filename was not specified!") );
			return false;
		}

		//TODO: See \\CrogServer\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Plugin.cpp
#if 0 

		char buff[512];
		GetFullPathName(m_filename.c_str(), 512, buff, 0);
		String pluginDirectory = Path(buff).directory();

		//Set the dll search directory to the path fo the dll
		SetDllDirectory(pluginDirectory.c_str());

		//Set the default error mode so we get messages for missing dependencies
		SetErrorMode(0);

		// Load DLL file
		m_hinstLib = LoadLibraryEx(m_filename.c_str(), 0, LOAD_WITH_ALTERED_SEARCH_PATH);

		//Restore the dll search directory to the default
		SetDllDirectory(0);
#endif

		m_hwnd = LoadLibrary( m_filename.c_str() );
#else
return dlopen((libName + ".so").c_str(), RTLD_LAZY);
#endif

		m_loaded = frx::isValidHandle( m_hwnd );
		if ( m_loaded )
		{
			//Get information about the plugin
			getPluginInfo_t plugin_GetInfo = (getPluginInfo_t)Plugin::getSymbolAddress( PluginFunction_GetInfo );  

			if ( plugin_GetInfo ) //Check the infor function was found
				m_info = plugin_GetInfo(); //Get a pointer to the information structure
			else
			{
				logMessage( "No plugin 'GetInfo' was found." );
				m_info = 0;
			}

			logSuccess( _T("Win32Plugin \"") << m_filename.c_str() << _T("\" loaded successfully.") /* DEBUG_ONLY( << _T('\n') << *m_info )*/ );
			return true;
		}
		else
		{
			logError( _T("Win32Plugin \"") << m_filename.c_str() << _T("\" failed to load. Detailed error:\n") << getLastError().c_str() );
			return false;
		}

	}			

	//Unload the library
	Bool Win32Plugin::unload()
	{
		m_info = 0;

		if ( isLoaded() )
		{
			stop(); //Stop the plugin

			//We must make sure that any log message locations don't become invalidated  when the plughin is unloaded
			_GlobalLog.pollMessageQueue();

			bool b = (FreeLibrary( m_hwnd ) != 0);
			m_hwnd = 0;
			m_loaded = false;
			return b;
		}

		m_hwnd = 0;
		return true;
	}

	//Run the start process
	Bool Win32Plugin::start()
	{
		if ( !isLoaded() )
		{
			if ( !load() )
				return false;
		}

		if ( !isRunning() )
		{
			m_running = true;


			activatePlugin_t process = (Plugin::activatePlugin_t)Plugin::getSymbolAddress( Plugin::PluginFunction_Activate );
			if ( frx::isValidPointer( process ) )
			{
				bool ret = process();
				if ( ret )
				{
					logSuccess( _T("Win32Plugin \"") << m_filename.c_str() << _T("\" started successfully.") );
				}
				else
				{
					logError( _T("Win32Plugin \"") << m_filename.c_str() << _T("\" returned failure from start().")  );
				}
				return ret;
			}	
			logError( _T("Win32Plugin \"") << m_filename.c_str() << _T("\" has no plugin_Activate() method!") );

		}
		return false;
	}

	//Run the start process
	Bool Win32Plugin::stop()
	{
		if ( isLoaded() && isRunning() )
		{
			dectivatePlugin_t process = (Plugin::dectivatePlugin_t)Plugin::getSymbolAddress( Plugin::PluginFunction_Unregister );
			if ( frx::isValidPointer( process ) )
			{
				process();
			}
			else
			{
				logWarning( _T("Win32Plugin \"") << m_filename.c_str() << _T("\" has no plugin_Deactivate() method!") );
			}


			m_running = false;
		}
		return true;
	}

	void* Win32Plugin::getSymbolAddress( const String& name ) const
	{
#ifdef UNICODE
		frx::String8 charString = name.asString8();
		return GetProcAddress( m_hwnd, charString.c_str() );
#else
		return GetProcAddress( m_hwnd, name.c_str() );
#endif			
	}


	String Win32Plugin::getLastError() 
	{
		LPVOID lpMsgBuf; 
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, 
			GetLastError(), 
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), 
			(LPTSTR) &lpMsgBuf, 0, NULL ); 

		String ret( (Char*)lpMsgBuf );

		// Free the buffer.
		LocalFree( lpMsgBuf );
		return ret;
	}

} //END: win32