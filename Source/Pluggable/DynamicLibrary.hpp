#ifndef KISMOENGINE_PLUGGABLE_DYNAMICLIBRARY_H
#define KISMOENGINE_PLUGGABLE_DYNAMICLIBRARY_H

#ifdef WIN32
//#include "os/DynamicLibrary_win32.hpp"
#elif __APPLE__
//#include "os/DynamicLibrary_darwin.hpp"
#elif __linux__
//#include "os/DynamicLibrary_linux.hpp"
#endif

namespace win32
{
	/** Win32 implementation of frx::Plugin
	*/
	class DynamicLibrary
	{
	public:

		/** Load the library RAII
		*/
		DynamicLibrary( const std::string_view& filename );

		/* \copydoc core::Plugin::~Plugin
		*/
		~DynamicLibrary();

		bool isOpen() const;

		/** \copydoc core::Plugin::load
		*/
		bool load();			

		/** \copydoc core::Plugin::unload
		*/
		bool unload();

		/**  Get a symbols address from the current process
		*/
		void* getSymbolAddress( const String& name ) const;

		template<typename CFunctor>
		CFunctor getSymbolAddressT(const String& name) const
		{ return reinterpret_cast<Functor>(getSymbolAddress(name)); }


	protected:

		String getLastError();

	protected:

#ifdef WIN32
		HMODULE hwnd_; ///< Handle to the plugin/Dll module
#elif __APPLE__
		//TODO:
#elif __linux__
		//TODO:
#endif

	};

} //END: win32

#endif