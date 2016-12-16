#include "species.h"

Species::Species() : biomass(1.0), feedingcenter(0.0), feedingrange(1.0), index(0)
{
  return;
}

Species::Species(double _biomass, double _feedingcenter, double _feedingrange)
  : biomass(_biomass), feedingcenter(_feedingcenter), feedingrange(_feedingrange), index(0)
{
  return;
}

Species::~Species()
{
  return;
}