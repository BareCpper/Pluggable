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

#include "PluginInstance.h"
#include "PluginRegistry.h"

namespace kismo
{
	ePluginType IPluginInstance::getType() const
	{ return m_type; }
	
	const String& IPluginInstance::getID() const
	{ return m_id; }

	IPluginInstance::IPluginInstance( ePluginType type, const String& id ) :
		m_type(type),
		m_id(id)
	{ regiterPluginInstance(); }

	IPluginInstance::~IPluginInstance()
	{ unregiterPluginInstance(); }

	bool IPluginInstance::regiterPluginInstance()
	{
		//TODO: Register this instance with the PluginRegistry
		PluginRegistry::getSingleton().registerPluginInstance(this);
		return false;
	}
	
	bool IPluginInstance::unregiterPluginInstance()
	{
		//TODO: Register this instance with the PluginRegistry
		PluginRegistry::getSingleton().unregisterPluginInstance(this);
		return false;
	}
} //END: kismo