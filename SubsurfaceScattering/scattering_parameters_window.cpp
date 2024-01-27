#include "scattering_parameters_window.h"

ScatteringParametersWindow::ScatteringParametersWindow(
	ScatteringParameters &parameters)
	: parameters(parameters) {
	name = "Parameters";
}

void ScatteringParametersWindow::build() {
	ImGui::Begin(get_name());

	ImGui::SeparatorText("Light");
	ImGui::SliderFloat3("Position", parameters.light.position.data(), -3.0f,
						3.0f);
	ImGui::ColorEdit3("Color", parameters.light.color.data());
	ImGui::SliderFloat("Ambient", &parameters.light.ambient, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse", &parameters.light.diffuse, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular", &parameters.light.specular, 0.0f, 1.0f);
	ImGui::SliderFloat("m", &parameters.light.m, 0.0f, 100.0f);
	ImGui::SliderFloat("Wrap", &parameters.wrap, 0.0f, 1.0f);
	ImGui::SliderFloat("Scatter power", &parameters.scatter_power, 0.0f, 1.0f);
	ImGui::SliderFloat("Scatter width", &parameters.scatter_width, 0.0f, 1.0f);
	ImGui::ColorEdit3("Scatter color", parameters.scatter_color.data());
	ImGui::SliderInt("Scatter falloff power", &parameters.scatter_falloff, 0,
					 3);
	ImGui::Checkbox("Scatter depends on angle", &parameters.angle_scatter);
	ImGui::SliderFloat("Translucency", &parameters.translucency, 0.0f, 1.0f);
	ImGui::SliderFloat("sigma_t", &parameters.sigma_t, 0.0f, 5.0f);

	ImGui::SeparatorText("Depth map");
	ImGui::SliderFloat("Grow", &parameters.grow, 0.0f, 0.1f);

	ImGui::SeparatorText("Display");
	ImGui::Combo("Mesh", &parameters.rendered_mesh_idx,
				 "Cube\0Salt Lamp\0Head");

	ImGui::End();
}
