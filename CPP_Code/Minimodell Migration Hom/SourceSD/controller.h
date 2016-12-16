// Controller Klasse
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "species.h"
#include "foodweb.h"
#include "output.h"
#include <gsl/gsl_rng.h>
#include <list>

using namespace std;

class Controller
{
private:
  double x;				//Struktur-/Levelparameter x
  double c;				//Konkurrenzparameter c
  double d;				//Sterbeparameter d
  double mig_prop;			//Verhältnis von Migrationen zu Mutationen zu spontanem Aussterben:
  double death_prop;			//1 : mig_prop : death_prop
  long long t;				//Zeit
  Species** global_species;		//Liste aller insgesamt lebenden Species
  int* species_count;			//Liste, auf wie vielen patches die Spezies gerade existiert
  list<long long>* manager;			//Verwaltet alle zur Zeit lebenden Spezies
  
  int species_pos;			//Position, an der man vermutlich als nächstes eine neue Spezies einfügen könnte
  int maxS;				//Maximal mögliche Gesamtspeziesanzahl (incl. Resource)
  int S;				//Aktuelle (global-)Speziesanzahl (incl. einer Resource, die auf global_species[0] liegen soll)
  int absS;				//Aktuelle Gesamtanzahl Spezies (Summe aller Dimensionen - ohne Resource)

  int patches;				//Anzahl der Patches
  int number_of_columns;			//Anzahl der Zeilen bei den Patches
  Foodweb** webs;			//Die Nahrungsnetze
  
  int target_web_id;			//Netz in das Migration/in dem Mutation stattfindet
  int chosen_id;			//Nächste(r) Mutter/Migrant
  
  int counter_all_webs;			//Zählt, wie oft die Migration fehlschlägt, weil die Spezies auf allen Netzen existiert.
  int counter_all_webs_S1;		//Zählt, wie oft die Migration fehlschlägt, weil die Ausgangsspezies auf allen Netzen existiert.
  int counter_inbound;			//Zählt, wie oft die Migration fehlschlägt, weil eine Spezies nach innen migriert. Wird auch erhöht, wenn eine Spezies ausgewählt wurde, die auf allen Patches existiert.
  int counter_inbound_S1;		//Zählt, wie oft die Migration fehlschlägt, weil die Ausgangsspezies nach innen migriert. Wird auch erhöht, wenn die Ausgangsspezies auf allen Patches existiert.
  
  int success_migration;		//Zählt die Anzahl erfolgreicher Migrationen
  int success_mutation;			//Zählt die Anzahl erfolgreicher Mutationen
  
  
  double feedingrange_min;		//Setzt die minimale Feedingrange eines Mutanten.
  double feedingrange_var;		//Variiert die Feedingrange eines Mutanten um maximal diesen Wert nach oben.
  gsl_rng *r;
  
  double spn;				//Zählt durchschnittliche Anzahl Spezies pro Netz
  double nps;				//Zählt durchschnittliche Anzahl Netze pro Spezies, ohne die Ressource
  double abs_connectance;		//Zählt die Summe der Verbundenheiten aller Netze
  
  long long last_output_time;		//Gibt an, wann das letzte mal ein output erzeugt wurde
  
  bool full_die;			//Gibt an, ob die.out komplett ausgegeben wird, oder nur der letzte Zeitpunkt
  

public:
  Controller();				//constructor
  ~Controller();			//destructor, gibt den Speicherplatz frei
  void init(int seed, int columns = 1, int rows = 1, double migration_proportion = 1000.0, double death_proportion = 1000.0 , double custom_feedingrange_min = 0.0, double custom_feedingrange_var = 0.0, int scenario = 0, int maximum_species = 4095, bool fdo = false);	//Initialisiert den Controller: Alloziiert speicher, setzt Resource und Netze damit,
										//initialisiert den Zufallszahlengenerator
  //Getter und Setter:
  void set_x(double _x){x = _x;};
  double get_x(){return x;};
  void set_c(double _c){c = _c;};
  double get_c(){return c;};
  void set_d(double _d){d = _d;};
  double get_d(){return d;};
  long long get_t(){return t;};
  int get_success_migration(){return success_migration;};
  int get_success_mutation(){return success_mutation;};
  
  int get_counter_all_webs(){return counter_all_webs;};
  int get_counter_all_webs_S1(){return counter_all_webs_S1;};
  int get_counter_inbound(){return counter_inbound;};
  int get_counter_inbound_S1(){return counter_inbound_S1;};
    
  bool find_chosen_and_do_action(Output* out);				//Sucht einen Patch und dann eine Spezies auf diesem Patch und wählt aus, was mit ihr passiert
  
  bool mutate();							//Erstellt einen Mutanten und setzt diesen ins Nahrungsnetz, falls er überleben könnte. Gibt true zurück, falls der Mutant erfolgreich ins Netz gesetzt wurde
  bool migrate(); 							//Migriert eine Spezies in ein angrenzendes Netz
  void spontaneous_death(Output* out);					//Entfernt eine Spezies auf einem Netz
  
  int die(Output* out);							//Lässt Spezies aussterben, gibt ggf. entsprechende Speicherbereiche frei und gibt die Anzahl der gestorbenen Spezies zurück.
   
  void print(Output* out);						//Gibt Zwischenbilanz aus
  /*void print_die(Output* out);*/
  
  void clean_up(Output* out);						//Räumt auf; ruft remove_species für alle Spezies in allen Netzen auf.

};

#endif