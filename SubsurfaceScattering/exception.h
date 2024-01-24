#pragma once

#include <stdexcept>
#include <string>

#define THROW_EXCEPTION throw GlApplicationInitException()

class GlApplicationInitException : public std::runtime_error {
  public:
	GlApplicationInitException() : runtime_error("Application init error") {}
};
