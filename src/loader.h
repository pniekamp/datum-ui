//
// datum-ui - loader
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include "context.h"
#include <map>

using LoaderString = std::basic_string<char, std::char_traits<char>, StackAllocator<char>>;
using LoaderItemList = std::map<LoaderString, Ui::Item*, std::less<>, std::scoped_allocator_adaptor<StackAllocator<std::pair<const LoaderString, Ui::Item*>>>>;

///////////////////////// create_ui /////////////////////////////////////////
Ui::Item *create_ui(Ui::Context &context, uint8_t const *bytecode, size_t length, Arena &scratchmemory);
Ui::Item *create_ui(Ui::Context &context, uint8_t const *bytecode, size_t length, LoaderItemList *items, Arena &scratchmemory);
