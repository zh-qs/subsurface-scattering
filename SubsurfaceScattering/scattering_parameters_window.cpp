#include "scattering_parameters_window.h"

ScatteringParametersWindow::ScatteringParametersWindow()
{
	name = "Parameters";
}

void ScatteringParametersWindow::build()
{
	ImGui::Begin(get_name());

	ImGui::SeparatorText("dupa");
	ImGui::Text("dupa");

	ImGui::End();
}
