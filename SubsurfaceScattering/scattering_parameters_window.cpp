#include "scattering_parameters_window.h"

ScatteringParametersWindow::ScatteringParametersWindow(
	ScatteringParameters &parameters)
	: parameters(parameters) {
	name = "Parameters";
}

void ScatteringParametersWindow::build() {
	ImGui::Begin(get_name());

	ImGui::SeparatorText("Light");
	ImGui::SliderFloat3("Position", parameters.light.position.data(), -10.0f, 10.0f);
	ImGui::ColorEdit3("Color", parameters.light.color.data());
	ImGui::SliderFloat("Ambient", &parameters.light.ambient, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse", &parameters.light.diffuse, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular", &parameters.light.specular, 0.0f, 1.0f);
	ImGui::SliderFloat("m", &parameters.light.m, 0.0f, 100.0f);
	ImGui::SliderFloat("Wrap", &parameters.wrap, 0.0f, 1.0f);
	ImGui::SliderFloat("Scatter width", &parameters.scatter_width, 0.0f, 1.0f);
	ImGui::ColorEdit3("Scatter color", parameters.scatter_color.data());

	ImGui::SeparatorText("Display");
	ImGui::Combo("Mesh", &parameters.rendered_mesh_idx, "Cube\0Salt Lamp\0Head");

	ImGui::End();
}
