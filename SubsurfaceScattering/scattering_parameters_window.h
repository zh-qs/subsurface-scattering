#pragma once

#include "window.h"
#include "scattering_parameters.h"

class ScatteringParametersWindow : public Window {
	ScatteringParameters &parameters;

  public:
	ScatteringParametersWindow(ScatteringParameters& parameters);
	virtual void build() override;
};
