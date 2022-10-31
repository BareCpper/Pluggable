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

#ifndef KISMOFRX_PLUGININSTANCE_H
#define KISMOFRX_PLUGININSTANCE_H

#include "KismoFRX/DateTime.h"
#include "KismoFRX/Common.h"
#include "KismoFRX/SmartPointer.h"


namespace kismo
{
	/** PluginInstance type used to identify the derived implementation supported by a plugin
	*/
	enum ePluginType
	{
		PluginType_ModelTranslator,		///< OBJ, FBX model translators (See KismoFileFormat for IModelTranslator)
		PluginType_ImageTranslator,		///< TGA, BMP, DDS model translators (See KismoFileFormat for IImageTranslator)
		PluginType_Renderer,			///< OpenGL/Direct3D/RayTrace renderers (See KismoGFX for IRenderer)	
		PluginType_Physics,				///< PhysX, ODE, Bullet etc physics implementations (See KismoDYN for IPhysics)			
		PluginType_Audio,				///< OpenAL, DirectSound audio implementations (See KismoAudio for IAudio)
		PluginType_InputDevice,			///< DirectInput, Win32 etc implementations for input devices (See KismoInput for IInputDevice)
		PluginType_EntityController,	///< TODO: user controler library or scripted thing!?
		PluginType_EntityBehaviour,		///< TODO: user controler library or scripted thing!?
		PluginType_ResourceLocation,	///< ZIP, WAD, 7Zip, RAR support could be added as possible resource locations
		//PluginType_GUI,
		//PluginType_Effect,
		//PluginType_SceneTranslator,
		//PluginType_Addon,
		PluginType_Other = 255,
	};	
		
	/** \brief		Base interface for a plugin interface
		\remarks	Multiple IPluginInstances may be constructed by a DLL
		\remarks	Creation of a derived IPluginInstance will automatically register the implementation within the 
					PluginRegistry. A PluginManager can be instantiated which can utilise the registry to load and 
					unload plugin functionalities via the registry in either a static or dynamically linked mode
					depending on build parameters.
	*/
	class KISMOFRX_EXPORT IPluginInstance
	{
	public:

		// Derived implementation should define speciifc functionaliy based on type

		/**
			\brief Obtains the type information for what derived interface this plugin instantiation supports
		*/
		ePluginType getType() const;

		const String& getID() const;

	protected:
		/**
			\brief		Protected constructor to prevent direct instantiation and must be derived and created in the plugin library
			\remarks	On instantiation the construtor automatically registers this with the plugin
						registry without the need of programmer interaction
		*/
		IPluginInstance( ePluginType type, const String& id );

		/**
			\brief		Protected destructor to prevent direct deletion and must be derived and created in the plugin library
			\remarks	On destruction this is automatically unregistered from the plugin
						registry without the need of programmer interaction
		*/
		virtual ~IPluginInstance();

	private:

		bool regiterPluginInstance();
		
		bool unregiterPluginInstance();

	private:
		const ePluginType m_type; //< Type of instance interface i.e Renderer, translator, behaviour etc
		const String m_id; //< Name identifier
	};

} //END: kismo

#endif