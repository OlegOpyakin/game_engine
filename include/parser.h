#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "objects.h"
#pragma once

bool LoadFromObjectFile(std::string filename, std::vector<triangle> *tris);
