//
// datum-ui - codegen
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include "context.h"
#include <string>

///////////////////////// codegen_ui ////////////////////////////////////////
void codegen_ui(std::string const &filename, std::string const &name, uint8_t const *bytecode, size_t length);
