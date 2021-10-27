#ifndef MANTA_PACKER_HPP
#define MANTA_PACKER_HPP

// This is a collection of Manta specific datatypes that are meant to optimize the data stored and read by the engine

#include <string>

//
// Model packing
//
class Model;

void MantaPackModel(Model *model, std::string output);

#endif
