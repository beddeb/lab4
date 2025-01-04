#pragma once

#include "../../include/sequence.hpp"
#include "../../include/array_sequence.hpp"
#include "../../include/list_sequence.hpp"


void chronoAddInt(size_t num_elements);

void chronoGetIndexInt(size_t num_elements, size_t step);

void chronoIterateInt(size_t num_elements);

void chronoRemoveInt(size_t num_elements);

void chronoSequence(size_t num_elements, size_t step);

void chronoListSequence(size_t num_elements, size_t step);

void chronoArraySequence(size_t num_elements, size_t step);