#include "DynamicLibrary.h"

#include "Framework/Log.h"

frx::Plugin* frx::Plugin::create( const Char* filename )
{ return new win32::DynamicLibrary( filename ); }

namespace win32
{
	//Construction only from the PluginManager
	DynamicLibrary::DynamicLibrary( const String& filename ) :
		frx::Plugin( filename ),
		hwnd_( 0 )
	{
	
	}

	//Destruction only from the PluginManager
	DynamicLibrary::~DynamicLibrary()
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


	bool DynamicLibrary::isOpen() const
	{
#if _WIN32
		return hwnd_ != 0;
#else
		//TODO:
#endif
	}

	//Load the library
	Bool DynamicLibrary::load()
	{			
		if ( isOpen() )
			return true;

		if ( !m_filename )
		{
			logError( _T("DynamicLibrary filename was not specified!") );
			return false;
		}

#if _WIN32
		//TODO: See \\CrogServer\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Plugin.cpp
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

#else
	return dlopen((libName + ".so").c_str(), RTLD_LAZY);
#endif

		if ( isOpen() )
		{
			//Get information about the plugin
			getPluginInfo_t pluginGetInfo = (getPluginInfo_t)Plugin::getSymbolAddress( PluginFunction_GetInfo );  

			if ( pluginGetInfo ) //Check the infor function was found
				m_info = pluginGetInfo(); //Get a pointer to the information structure
			else
			{
				logMessage( "No plugin 'GetInfo' was found." );
				m_info = 0;
			}

			logSuccess( _T("DynamicLibrary \"") << m_filename.c_str() << _T("\" loaded successfully.") /* DEBUG_ONLY( << _T('\n') << *m_info )*/ );
			return true;
		}
		else
		{
			logError( _T("DynamicLibrary \"") << m_filename.c_str() << _T("\" failed to load. Detailed error:\n") << getLastError().c_str() );
			return false;
		}

	}			

	//Unload the library
	Bool DynamicLibrary::unload()
	{
		m_info = 0;

		if ( isLoaded() )
		{
			stop(); //Stop the plugin

			//We must make sure that any log message locations don't become invalidated  when the plughin is unloaded
			_GlobalLog.pollMessageQueue();

			bool b = (FreeLibrary( hwnd_ ) != 0);
			hwnd_ = 0;
			m_loaded = false;
			return b;
		}

		hwnd_ = 0;
		return true;
	}


	void* DynamicLibrary::getSymbolAddress( const String& name ) const
	{
#ifdef UNICODE
		frx::String8 charString = name.asString8();
		return GetProcAddress( hwnd_, charString.c_str() );
#else
		return GetProcAddress( hwnd_, name.c_str() );
#endif			
	}


	String DynamicLibrary::getLastError() 
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