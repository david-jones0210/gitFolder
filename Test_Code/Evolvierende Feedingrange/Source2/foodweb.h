// Foodweb Klasse
#ifndef FOODWEB_H
#define FOODWEB_H

#include "species.h"
#include "output.h"
#include <gsl/gsl_rng.h>

class Foodweb
{
private:
  int dim;				//Die Dimension des Netzes, also die Anzahl der Spezies plus eins (Resource)
  int maxdim;
  int web_id;				//Identifizierungsnummer des Netzes
  int** adjacency_matrix;		//Speicherplatz für die Adjazenzmatrix adj[i][j] = adjacency_matrix[i][j]
  double* trophical_level;		//Speicherplatz für die Trophischen Level der Spezies
  double** alpha;			//Speicherplatz für die Linkstärke linkstrength[i][j] = alpha[i][j]
  double* competition;			//Zwischenspeicher zur Berechnung der Competition
  double* competitors;			//Zwischenspeicher, der die Anzahl der Konkurenten enthält
  double* survival;			//Speicherplatz für die Survivalparameter der Spezies
  Species** species;			//Array für die Spezies-Traits und eine Resource am Anfang
  
  void calc_k();			//Berechnet die Competitors k
  void calc_c(double c);		//Berechnet die competition
  void calc_alpha(double x);		//Berechnet die Linkstärken-Matrix
  void calc_surv(double d);		//Berechnet die Survivalparameter

public:
  Foodweb();				//constructor
  ~Foodweb();				//destructor, gibt den Speicherplatz frei
  void init(Species* resource, int id = 0, int maximum_size = 255);	//alloziiert den Speicherplatz für maximum_size Spezies und belegt ein minimalnetz aus Resource
  void calculate(double x, double c, double d);		//Berechnet die Survivalparameter ausgehend von der Adjazenzmatrix.
  void calc_TL();			//Berechnet die trophischen Level der Spezies
  double get_TL(int i);			//Gibt das trophische Level der Spezies mit index i zurück.
  double get_survival(int i);		//Gibt den Survivalparameter der Spezies i zurück.
  Species* get_species(int i);		//Gibt einen Pointer auf die Spezies i zurück.
  int get_dimension();			//Gibt die Dimension = Anzahl der Spezies + 1 zurück
  double get_maxbm();			//Gibt die Körpergröße der größten Spezies im Netz zurück
  double get_minbm();			//Gibt die Körpergröße der kleinsten Spezies im Netz außer der Resource zurück
  double get_avgbm();			//Gibt die durchschnittliche Körpergröße aller Spezies (incl. Resource) zurück
  double get_maxTL();			// analog get_maxbm
  double get_avgTL();			// analog get_avgbm
  double get_connectance();		//Gibt die Connectance des Netzwerks zurück (#Links / dim*dim)
  double get_feedingrange();		//Gibt mittlere feeding range aller Spezies im Netz zurück
  
  int get_prey_count(Species* s);	//gibt die Anzahl der Beuten der Species s anhand des Fressbereich und der Biomassen in diesem Nahrungsnetz zurück
  void print(Output* out, int time);	//Ruft die Ausgaberoutinen für die Dateiausgaben auf.
  int add_species(Species* s);		//Fügt dem Nahrungsnetz die neue Spezies hinzu und berechnet die Adjazenzmatrix, gibt den index der neuen Spezies zurück.
  int remove_species_fitness(double threshold = 1.0);//survival muss schon berechnet sein, ist danach aber kaputt. Adjazenzmatrix wird aktualisiert. Aussterbende kommen nach hinten in der liste!
					//Gibt die Anzahl der Ausgestorbenen Spezies zurück, diese müssen vom Controller nachbereitet werden!!!
  void remove_species(int index);	//Entfernt die Spezies an der Position i aus dem Netz (Bitte niemals die Resource entfernen!!!) und fixt adjacency_matrix (nur diese, survivals etc ist weg!)
  void remove_species_list(int count, int* list);//TODO	Entfernt Spezies mit den indizes in list und fixt adjacency_matrix

};

#endif