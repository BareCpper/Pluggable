#include "OpenGLRenderer.h"

#include "Framework/Plugin.h"
#include "Framework/Vector.h"
#include "Core/Core.h"

namespace gfx
{
	const frx::Plugin::Info info =
	{
		{ 0, 1, 0 }, //Version & Build number of plugin
		_T("Craig Hutchinson"), //Author of the plugin
		_T("RAGE OpenGL Renderer"), //name/title of the plugin
		_T("Implimentation of an OpenGL renderer for the RAGE (Realtime Annihlation of Geometric environments)"), //Detailed description of the plugin
		//frx::DateTime(), //Date & time the plugin was created
		//frx::Plugin::PluginType_GUI, //Bitmask defining type of plugin e.g. renderer, effect etc..  			
	};

	extern "C"
	{
		const frx::Plugin::Info* plugin_GetInfo(void)
		{
			return &info;
		}


		// @todo ANDROID: Use JNI_OnLoad()
		// @todo WIN32: use DllMain()
		// @todo LINUX: __attribute__((constructor)) void dllLoad(); and __attribute__((destructor)) void dllUnload();
		Bool plugin_Activate(void) throw()
		{
			if (!frx::isValidPointer(oglRenderPlugin))
				oglRenderPlugin = new OpenGLRenderer();

			_Core.renderer = oglRenderPlugin;

			return frx::isValidPointer(oglRenderPlugin);
		}

		void plugin_Deactivate(void)
		{
			frx::safeDelete(oglRenderPlugin);
		}
	}



	namespace kismo {
		namespace gfx {
			namespace openGL
			{

				class OpenGLRendererPluginInstance : public IRendererPluginInstance
				{
				public:
					OpenGLRendererPluginInstance() : IRendererPluginInstance(_T("OpenGLRenderer")) {}

					virtual IRenderer* newRenderer(Log* log)
					{
						return new OpenGLRenderer(log);
					}

					virtual void deleteRenderer(IRenderer* renderer)
					{
						OpenGLRenderer* openGLRenderer = reinterpret_cast<OpenGLRenderer*>(renderer);
						Assert(openGLRenderer);
						delete openGLRenderer;
					}

				};

				static OpenGLRendererPluginInstance g_openGLRendererPluginInstance; ///< Instantiate the plugin instance

			} /* END: openGL */
		}  /* END: gfx */
	}/* END: kismo */


} // END: ogl
