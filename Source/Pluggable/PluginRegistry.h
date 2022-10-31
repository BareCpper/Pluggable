/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2011 Labyrinth Entertainment Ltd.
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/

#ifndef KISMOFRX_PLUGINREGISTRY_H
#define KISMOFRX_PLUGINREGISTRY_H

#include "KismoFRX/Types.h"
#include "KismoFRX/Common.h"
#include "KismoFRX/Singleton.h"
#include <set>

/** 
	\brief		Macro intiialises the PluginRegistry singleton instance 
	\remarks	Due to difference between compilers and instantiation of statics this must be defined before the end user
				application entry point, i.e above main() in main.cpp when compiling plugins with static/compile-time linking
	\warning	If the plugins are to only be loaded dynamically the singleton must be instantiated prior to loading of the 
				first plug-in
*/
#define INIT_PLUGINREGISTRY kismo::PluginRegistry g_pluginRegistry;

namespace kismo
{
	//forward declarations
	class IPluginInstance;

	/** \brief	Registry of all IPluginInstance classes that have been defined within all statically and dynamically linked
				libraries wihtin the current runnig process.
		\remarks	Is a singleton class which must be instantiated before the entry point of the application which is normally
					main.cpp of any using application
	*/
	class KISMOFRX_EXPORT PluginRegistry : public Singleton<PluginRegistry>
	{
		friend class IPluginInstance;
	public:


		IPluginInstance* findPluginInstance( const Char* pluginID );

		Count size() const
		{ return m_plugins.size(); }

	protected:
		bool registerPluginInstance( IPluginInstance* instance );
		bool unregisterPluginInstance( IPluginInstance* instance );

	private:
		typedef std::set<IPluginInstance*> PluginInstanceLookup;
#pragma warning(disable:4251)
		PluginInstanceLookup m_plugins;
#pragma warning(default:4251)
	};

} //END: kismo

#endif