// Eval Klasse
#ifndef EVAL_H
#define EVAL_H

class Eval
{
private:
  int n;
  double sum;
  double sum_sqr;
  

public:
  Eval();				//constructor
  ~Eval();				//destructor
  
  int get_n(){return n;};		//Gibt den Stichprobenumfang zurück
  double get_mean();			//Gibt das laufende arithmetische Mittel zurück
  double get_variance();		//Gibt die Varianz zurück
  Eval& operator<<(const double d);	//Fügt ein Datenelement hinzu.
};

//Eval& operator<<(Eval& ev, const double& d);	//Fügt ein Datenelement hinzu.

#endif