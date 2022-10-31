#ifndef KISMOGFX_IRENDERERPLUGIN_H
#define	KISMOGFX_IRENDERERPLUGIN_H

#include "KismoGFX/Common.h"
#include "KismoFRX/Types.h"
#include "KismoFRX/PluginInstance.h"

namespace kismo
{

	//Forward declarations
	class Log;

namespace gfx
{
	//Forward declarations
	class IRenderer;

	/**
		\brief Renderer plugin instance interface
	*/
	class KISMOGFX_EXPORT IRendererPluginInstance : public IPluginInstance
	{ 
	public:

		/**
			\brief The plugins create method returns a new rendere implementation that is defined in a plugin library
		*/
		virtual IRenderer* newRenderer( Log* log ) = 0;
		
		/**
			\brief		Delete a renderer created by this instance. 
			\wraning	Any memory that is allocated in one module should be deleted in the same module as each module may be
						using different memory managers and allocation
		*/
		virtual void deleteRenderer( IRenderer* renderer ) = 0;

	protected:
		IRendererPluginInstance( const String& id ) : IPluginInstance( PluginType_Renderer, id ) {}
		virtual ~IRendererPluginInstance() {}
	};

} /* END: gfx*/ } /*END: Kismo*/

#endif
