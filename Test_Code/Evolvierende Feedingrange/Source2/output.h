// Output Klasse
#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <fstream>
#include <string>
#include "species.h"
#include "eval.h"

class Output
{
public:
  Output();				//constructor
  ~Output();				//destructor
  
  enum resfile_type {OUT_PAR, OUT_BM, OUT_C, OUT_S, OUT_TROPOS, OUT_ALPHA, OUT_TROPHDISTRI, OUT_FILE_COUNT}; // Zum Zugriff auf die einzelnen Ausgabedateien
  void write_params(std::string path, int scenario, int seed, double x, double c, double d);	//Ergänzt eine Zeile in der Ausgabedatei, sollte am ende einer Simulation aufgerufen werden.
  void open_files(std::string path);		
  void close_files();
  void print_line(resfile_type f, int dim, int time, int patch, int array_size, void* data);	//Schreibt in die Datei f eine Zeile entsprechend dem Dateiformat
			// S+1 | t | patch | values...  wobei die anzahl der values durch array_size bestimmt ist und in data_dbl liegt, falls es ungleich NULL ist, ansonsten in data_mat
  void mute(resfile_type f);		// Setzt flags, um die Ausgabe in die Datei f zu unterdrücken.
  void unmute(resfile_type f);		// Widerruft die Auswirkung von mute.
  
  
private:
  std::ofstream file[OUT_FILE_COUNT];
  void collect_params(void* foodweb);
  void init_params();		//Initialisiert Evals für auszuwertende Netzwerkparameter.
  Eval** ev_params;
  unsigned int mute_flags;
  

};

#endif