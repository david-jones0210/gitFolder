// Spezies Klasse
#ifndef SPECIES_H
#define SPECIES_H


class Species
{
private:
  int index;				// Index für den globalen Speicherplatz

public:
  Species();				//constructor
  Species(double _biomass, double _feedingcenter = 0.0, double _feedingrange = 1.0);
  ~Species();				//destructor
  
  double biomass;			// Log der Körpergröße
  double feedingcenter;			// Log des Feedingcenters
  double feedingrange;			// Feedingrange auf der logarthmischen Skala
  
  void set_index(int i){index = i;};	// Setter und Getter
  int get_index(){return index;};

};

#endif