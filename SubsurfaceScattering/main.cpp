#include "scattering_view_window.h"
#include "gl_application.h"
#include "fullscreen_window.h"
#include "scattering_parameters_window.h"

int main(int, char**)
{
	GlApplication app(1280, 720, "Subsurface scattering", ImVec4(0.27f, 0.33f, 0.36f, 1.00f), true);

	auto fullscreen = make_window<FullscreenWindow>();
	auto view = make_window<ScatteringViewWindow>();
	auto parameters = make_window<ScatteringParametersWindow>();

	auto& dockspace = fullscreen->get_dockspace();
	auto split = dockspace.split(ImGuiDir_Left, 0.2f);
	split.first->dock(*parameters);
	split.second->dock(*view);

	app.add_window(fullscreen);
	app.add_window(view);
	app.add_window(parameters);

	app.get_io().Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);

	app.run();

	app.dispose();

	return 0;
}
