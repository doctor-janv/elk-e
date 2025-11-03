#include "elke_core/FrameworkCore.h"

int main(const int argc, char** argv)
{
  elke::FrameworkCore::initialize(argc, argv);
  return elke::FrameworkCore::execute();
}
