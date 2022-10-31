#ifndef FRX_PLUGIN_H
#define FRX_PLUGIN_H

#include "DynamicLibrary.hpp"


namespace frx
{
	/** Wrapper class for Dynamic linked libraries (DLL)
	*/
	class _DllExport Plugin
	{
	public:

		/** Plugin type as a bitmask as a plugin can contain more than one interface
		*/
		enum ePluginType
		{
			PluginType_Renderer = 1 << 0,
			PluginType_GUI = 1 << 1,
			PluginType_Effect = 1 << 2,
			PluginType_MeshTranslator = 1 << 3,
			PluginType_SceneTranslator = 1 << 4,
			PluginType_Physics = 1 << 5,
			PluginType_Addon = 1 << 14,
			PluginType_Other = 1 << 15,
		};

		/** Enumerator for common plugin functions
		*/
		enum ePluginFunction
		{
			PluginFunction_GetInfo,
			PluginFunction_Activate,
			PluginFunction_Deactivate,
		};

		/** Version number
		*/
		struct Version
		{
			UInt16 major = 0;
			UInt16 minor = 0;
			UInt16 patch = 0;
			UInt16 commit = 0;
		};

		/** Structure containing information about the oaded plugin
		*/
		struct Info
		{
			Version version; ///< Version & Build number of plugin
			std::string_view author; ///< Author of the plugin
			std::string_view title; ///< name/title of the plugin
			std::string_view description; ///< Detailed description of the plugin
			//frx::DateTime dateBuilt; ///< Date & time the plugin was created
			//UInt16 type; ///< Bitmask defining type of plugin e.g. renderer, effect etc.. 
		};

		typedef const Info& (*getPluginInfo_t) ();
		typedef bool(*activatePlugin_t) ();
		typedef void (*dectivatePlugin_t) ();

	public:

		/** Constructor takes the filename of a dynamic link library to load
			\param[in] filename The relative or full path to the plugin
		*/
		Plugin(const String& filename);

		/* virtual destructor
		*/
		virtual ~Plugin();

		/** Get information about the library
		*/
		const Plugin::Info& info() const
		{
			return *m_info;
		}

		/** get the filename for the plugin
		*/
		const String& filename() const
		{
			return m_filename;
		}

		/** Returns whether the plugin is loaded
		*/
		bool isLoaded() const
		{
			return m_loaded;
		}

		/** Returns whether the plugin is started or stopped
		*/
		bool isRunning() const
		{
			return m_running;
		}

		/** Load the library
		*/
		virtual bool load() = 0;

		/** Unload the library
		*/
		virtual bool unload() = 0;

		/** Run the start process
		*/
		virtual bool start() = 0;

		/** Run the stop process
		*/
		virtual bool stop() = 0;

		/** Get a proces address by name
		*/
		virtual void* getSymbolAddress(const String& name) const = 0;

		/** Get a proces address by its common function definition
		*/
		void* getSymbolAddress(const ePluginFunction commonFunc) const;

	private:
		Impl impl_; //< Platform implementation

		const Plugin::Info* m_info; ///< Plugin info
		String m_filename; ///< The filename of the plugin
		bool m_running; ///< has the start been called?
		bool m_loaded; ///< has the plugin loaded okay?


#if 0

		iterator begin()
		{
			return m_constructors.begin();
		}

		iterator begin(plugin::eType type)
		{
			iterator iPlugin = m_constructors.begin();
			while (iPlugin != end() && iPlugin->first.type != type)
				++iPlugin;
			return iPlugin;
		}

		iterator next(iterator iPlugin, plugin::eType type)
		{
			++iPlugin;
			while (iPlugin != end() && iPlugin->first.type != type)
				++iPlugin;
			return iPlugin;
		}

		iterator end()
		{
			return m_constructors.end();
		}

		//TODO; probably not needed?
		//void* newPluginInstance( const String& id, plugin::eType type );
		//bool deletePluginInstance( void* instance );

		/**
			\brief Register a constructor for a Renderer/FileTranslator/SceneNode
			\remarks Normally called from within a plugin. This registers an implementation it can create
			\par IConstructor contains a 'type()' field for identifying what the constructor creates
			\param[in] id The id is used to identify the type of renderer to construct for a call to newRenderer
		*/
		bool registerConstructor(const String& id, plugin::IConstructor* constructor);
		bool unregisterConstructor(const String& id, plugin::IConstructor* constructor);
		ConstructorLookup m_constructors; //, map of all the constructors for tyopes that can eb created
		ConstructedInstanceLookup m_constructedInstances; //, Set of instances and their IConstructor* so they can be unloaded cleanly
#endif
	};

} //END: frx

#endif