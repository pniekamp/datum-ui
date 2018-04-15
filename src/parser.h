//
// datum-ui - parser
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include <leap/pathstring.h>
#include <vector>

std::string load_ui(leap::pathstring const &path, std::string const &defines = "");

///////////////////////// parse_ui /////////////////////////////////////////
std::vector<uint8_t> parse_ui(std::string const &src);

