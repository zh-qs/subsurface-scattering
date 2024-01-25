#include "scattering_parameters_window.h"

ScatteringParametersWindow::ScatteringParametersWindow(
	ScatteringParameters &parameters)
	: parameters(parameters) {
	name = "Parameters";
}

void ScatteringParametersWindow::build() {
	ImGui::Begin(get_name());

	ImGui::SeparatorText("Light");
	ImGui::InputFloat3("Position", parameters.light.position.data());
	ImGui::ColorEdit3("Color", parameters.light.color.data());
	ImGui::SliderFloat("Ambient", &parameters.light.ambient, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse", &parameters.light.diffuse, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular", &parameters.light.specular, 0.0f, 1.0f);
	ImGui::SliderFloat("m", &parameters.light.m, 0.0f, 100.0f);

	ImGui::SeparatorText("Display");
	ImGui::Combo("Mesh", &parameters.rendered_mesh_idx, "Cube\0Salt Lamp\0Head");

	ImGui::End();
}
