#include "controller.h"
#include <iostream>
#include <math.h>

using namespace std;

Controller::Controller() : x(0.1), c(0.1), d(0.0001), t(0)
{
  return;
}

Controller::~Controller()
{
  delete webs;
  for(int i = 0; i < maxS; i++)
  {
    if(species_count[i] > 0)
      delete global_species[i];
  }
  delete[] global_species;
  delete[] species_count;
  gsl_rng_free(r);	
  return;
}

void Controller::init(int seed, int columns, int rows, double migration_proportion,double death_proportion, double custom_feedingrange_min, double custom_feedingrange_var, int scenario, int maximum_species, bool fdo)
{
  //counter initialisieren
  full_die = fdo;
  counter_all_webs = 0;
  counter_all_webs_S1=0;
  counter_inbound = 0;
  counter_inbound_S1 = 0;
  
  success_mutation = 0;
  success_migration = 0;
  
  
  number_of_columns = columns;
  
  patches = columns*rows;		//Anzahl der Patches bestimmen
   
  last_output_time = 0;
   
  //Random Number Generator initialisieren:
  const gsl_rng_type *T;						
  gsl_rng_env_setup();   			
  T = gsl_rng_default;   		// default random number generator (so called mt19937)
  gsl_rng_default_seed = seed;		// Startwert fuer den RNG
  r = gsl_rng_alloc(T);
    
  
  
  
  maxS = maximum_species + 1;
  
  mig_prop = migration_proportion;
  death_prop = death_proportion;
  
  feedingrange_min = custom_feedingrange_min;
  
  feedingrange_var = custom_feedingrange_var;
  
  target_web_id = -1;
  chosen_id = -1;
  
  //Speicher für spezies reservieren:
  global_species = new Species*[maxS];
  species_count = new int[maxS];
  manager = new list<long long>[maxS];
    
  for(int i = 0; i < maxS; i++)
  {
    global_species[i] = NULL;
    species_count[i] = 0;
  }
 
  
  //Resource und erste Spezies erstellen:
  S = 2;
  species_count[0] = patches;
  global_species[0] = new Species(0.0, 0.0, 0.0);
  global_species[0]->set_index(0);
  
  species_count[1] = patches;
  global_species[1] = new Species(2.0, 0.0, 0.5);
  global_species[1]->set_index(1);
  
  species_pos = 2;
  
  abs_connectance = 0.0;
    
  //Nahrungsnetze erstellen:
  webs = new Foodweb*[patches];
  for(int i = 0; i < patches; i++)
  {
    webs[i] = new Foodweb();
    webs[i]->init(global_species[0], i, 63);
    webs[i]->add_species(global_species[1], 0);
    abs_connectance += webs[i]->get_connectance();			//startwert der absoluten Verbundenheit berechnen
  }
  
  absS = patches;
  
  
  spn = 1;
  nps = patches;

  return;
   
}


bool Controller::find_chosen_and_do_action(Output* out)
{
  //cout << t << " : find_chosen_and_do_action" << endl;
  
  t++;												//Zeitschritt auch bei fehlgeschlagener Mutation/Migration machen
  
  target_web_id = -1;
  chosen_id = -1;
  
  
  
  //Finde Netz
  //int weights = new int[maxS];								//Gewichte, entsprechen nach aktueller Methode der Dimension des Netztes
  int sum1 = 0; 
  int sum2 = 0;

  sum1 = absS*gsl_rng_uniform(r);
  
  for(int i = 0; i < patches; i++)
  {
    sum2+= (webs[i]->get_dimension()-1);
    if(sum1 < sum2)
    {
      target_web_id = i;									//Gewählte Mutter/Sterbende
      break;
    }
  }
  
  if(((target_web_id < 0)||(target_web_id > patches-1)))
  {					//Gibt false zurück, wenn kein Netz gefunden wurde
    cout << t << " : Controller::find_chosen_and_do_action(): finde target_web_id ---- target_web_id = " << target_web_id << endl;
    t = 1000*t+1000;
    return false;
  }
  
  //Finde Erwählten
  chosen_id = webs[target_web_id]->get_species((int)((webs[target_web_id]->get_dimension()-1)*gsl_rng_uniform(r)+1))->get_index();
  
  if(((chosen_id < 1)||(chosen_id > maxS)))
  {					//Gibt false zurück, wenn kein passender Erwählter gefunden wurde
    cout << t << " : Controller::find_chosen_and_do_action(): finde chosen_id" << endl;
    t = 1000*t+1000;
    return false;
  }
  
  if(species_count[chosen_id] < 1)
  {					//Gibt false zurück, wenn kein passender Erwählter gefunden wurde
    cout << t << " : Controller::find_chosen_and_do_action(): finde chosen_id" << endl;
    t = 1000*t+1000;
    return false;
  }
  
  
  
  
  //Wähle Aktion
  
  double random = gsl_rng_uniform(r);
  
  if((mig_prop + death_prop + 1.0)*random < mig_prop)
  {
    if(!migrate())
      return false;
    success_migration++;
    
  } else
    if((mig_prop + death_prop + 1.0)*random < (mig_prop + death_prop))
    {
      spontaneous_death(out);
    }
    else
    {       
      if(!mutate())
	return false;
      success_mutation++;   
    
    }

  return true; 

}


bool Controller::mutate()
{
  
  //cout << t << " : mutate" << endl;
  
  webs[target_web_id]->calculate(x, c, d);
  
  //Fehler abfangen
  if(webs[target_web_id]->get_dimension() < 2)
  {
    cout << "Controller::mutate(" << target_web_id << ") - Fehler: Keine Mutterspezies im Nahrungsnetz " << endl;
    cout << "t = " << t << endl;
    t = 1000*t + 1000;
    return false;
  }
  
  
  //Mutieren:
  double mutant_bm = global_species[chosen_id]->biomass + 2.0*log10(5.0/*2.0*/)*(gsl_rng_uniform(r)-0.5);
  Species* mutant = new Species(mutant_bm, mutant_bm - 1.0 - 2.0*gsl_rng_uniform(r), feedingrange_min + feedingrange_var*gsl_rng_uniform(r));

  
  // Update-Regel: Hat der Mutant was zu fressen?
  if(webs[target_web_id]->get_prey_count(mutant) == 0)
  {
    //Mutant nicht lebensfähig:
    delete mutant;
    //cout << "Mutant nicht lebensfähig" << endl;
    return false;
  }
  
  double old_connectance = webs[target_web_id]->get_connectance();	//Verbundenheit zwischenspeichern, vor Veränderung des Netzes
  
  // Update-Regel: Ist der Mutant lebensfähig? surv(Mutant) > 1?
  // Zum Netz hinzufügen:
  int mutant_index = webs[target_web_id]->add_species(mutant,t);
  //cout << t << " : target_web_id: " << target_web_id << endl;
  //cout << t << " : webs_size: " << webs[target_web_id]->get_dimension() << endl;
  if(mutant_index < 0) // Fehler aus Foodweb!
  {
    t *= 2;
    mutant_index = 0;
  }
 
  webs[target_web_id]->calculate(x, c, d);
  if(webs[target_web_id]->get_survival(mutant_index) < 1.0)
  {
    //Mutant könnte nicht überleben:
    webs[target_web_id]->remove_species(mutant_index);
    delete mutant;
    //cout << "Mutant nicht überlebensfähig" << endl;
    return false;
  }
  
  //Spezies ist (zunächst) lebensfähig:
  
  //Wo kann die neue Spezies gespeichert werden?
  
  
  bool ok = true;
  if(species_pos == maxS)
  {
    species_pos = 0;
    ok = false;
  }
  while(species_count[species_pos] != 0)
  {
    species_pos++;
    if(species_pos == maxS)
    {
      species_pos = 0;
      if(ok)
      {
	ok = false;
      }
      else
      {
	cout << t << " : Controller::mutate() - Fehler: Maximale globale Speziesanzahl (" << maxS << ") überschritten!" << endl;
	t = 1000*t+1000;
	webs[target_web_id]->remove_species(mutant_index);
	delete mutant;
	return false;
      }
    }
  }
  
  mutant->set_index(species_pos);

  global_species[species_pos] = mutant;
  species_count[species_pos] = 1;
  
  if(manager[species_pos].size() != 0)
  {
    cout << "Fehler - manager[" << species_pos << "] = " << manager[species_pos].size() << endl;
    t= 1000 * t + 1000;
  }
  

  //Anpassen der absoluten Speziesanzahl
  absS++;
  
  //Anpassen der absoluten Verbundenheit
  abs_connectance = abs_connectance - old_connectance + webs[target_web_id]->get_connectance();

  //Anpassen von SpN und NpS
  spn += 1./((double)patches);
  nps = (nps*(((double)S)-1.) + 1.)/((double)S);
  
  S++;
  species_pos++;

  return true;
}


bool Controller::migrate()
{
  //cout << t << " : migrate" << endl;
 
  //Prüfe ob Migrant auf allen Netzen existiert.
  if(species_count[chosen_id] == patches)
  {
    counter_inbound++;
    counter_all_webs++;
    if(chosen_id == 1)
      if(global_species[chosen_id]->biomass == 2.0)
	if(global_species[chosen_id]->feedingcenter == 0.0)
	{
	  counter_inbound_S1++;
	  counter_all_webs_S1++;
	}
  }
  
  
  switch((int)(4*gsl_rng_uniform(r)))
  {
    case 0:
      target_web_id = (target_web_id + patches - number_of_columns) % patches;		//nördliches Netz
      break;
      
    case 1:
      if((target_web_id + 1) % number_of_columns == 0)					//östliches Netz
	target_web_id = target_web_id + 1 - number_of_columns;					//wenn am östlichen Rand
      else  
	target_web_id = target_web_id + 1;
      break;
      
    case 2:
      target_web_id = (target_web_id + number_of_columns) % patches;			//südliches Netz
      break;
      
    case 3:
      if(target_web_id % number_of_columns == 0)					//westliches Netz
	target_web_id = target_web_id - 1 + number_of_columns;					//wenn am westlichen Rand
      else  
	target_web_id = target_web_id - 1; 
      break;
      
    default:
       t = 1000*t + 1000;
       cout << t << " : Controller::migrate() - Fehler beim Finden des Zielnetzes!" << endl;
       break;
  }
  

  
  //Prüfe ob Migrant auf Zielnetz bereits existiert.
  for(int i = 1; i < webs[target_web_id]->get_dimension(); i++)
    if(webs[target_web_id]->get_species(i)->get_index() == chosen_id)
    {
      counter_inbound++;
      if(chosen_id == 1)
	if(global_species[chosen_id]->biomass == 2.0)
	  if(global_species[chosen_id]->feedingcenter == 0.0)
	    counter_inbound_S1++;
      return false;
    }
  
  
  webs[target_web_id]->calculate(x, c, d);
    
  //Migrant erstellen
  Species* migrant = global_species[chosen_id];
  
      
  // Update-Regel: Hat der Migrant was zu fressen?
  if(webs[target_web_id]->get_prey_count(migrant) == 0)
  {
    //Migrant nicht lebensfähig:
    return false;
  }
  
  double old_connectance = webs[target_web_id]->get_connectance();	//Verbundenheit zwischenspeichern, vor Veränderung des Netzes
  
  // Update-Regel: Ist der Migrant lebensfähig? surv(Migrant) > 1?
  // Zum Netz hinzufügen:
  int new_migrant_index = webs[target_web_id]->add_species(migrant,t);
  
  if(new_migrant_index < 0) // Fehler aus Foodweb!
  {
    t = 1000*t + 1000;
    new_migrant_index = 0;
    cout << t << " : migrate() - Fehler aus Foodweb!" << endl;
  }
  
  
  webs[target_web_id]->calculate(x, c, d);
  
  if(webs[target_web_id]->get_survival(new_migrant_index) < 1.0)
  {
    //Migrant könnte nicht überleben:
    webs[target_web_id]->remove_species(new_migrant_index);
    return false;
  }
    
  
  //Spezies ist (zunächst) lebensfähig:
  
  //Anpassen des species_count und der absoluten Spezieszahl
  species_count[chosen_id]++;
  absS++;
  
  //Anpassen der absoluten Verbundenheit
  abs_connectance = abs_connectance - old_connectance + webs[target_web_id]->get_connectance();
  
  //Anpassen von SpN und NpS
  spn += 1./((double)patches);
  nps += 1./(((double)S)-1.); 
    
    
  return true;
}



void Controller::spontaneous_death(Output* out)
{
  
  //cout << t << " : spontaneous_death" << endl;
  
  webs[target_web_id]->calculate(x, c, d);
  
  //Fehler abfangen
  if(webs[target_web_id]->get_dimension() < 2)
  {
    cout << "Controller::spontaneous_death(" << target_web_id << ") - Fehler: Keine Sterbende im Nahrungsnetz " << endl;
    cout << "t = " << t << endl;
    t = 1000*t+1000;
    return;
  }
  
  double old_connectance = webs[target_web_id]->get_connectance();	//Verbundenheit zwischenspeichern, vor Veränderung des Netzes
  
  
  // Spezies im Netz finden und sterben lassen
  for(int i = 1; i < webs[target_web_id]->get_dimension() + 1; i++)
  {
   
    if(i == webs[target_web_id]->get_dimension())
    {
    cout << "Controller::spontaneous_death(" << target_web_id << ") - Fehler: Die Sterbende existiert nicht im Nahrungsnetz " << endl;
    cout << "t = " << t << endl;
    t = 1000*t+1000;
    return;
    }
    
    if(chosen_id == webs[target_web_id]->get_species(i)->get_index()) 
    {
      manager[chosen_id].push_back(target_web_id);					//einlesen Patch
      if(full_die)
	  manager[chosen_id].push_back(webs[target_web_id]->remove_species(i,t,out));			//einlesen Birth, sterben im Netz
	else
	  manager[chosen_id].push_back(webs[target_web_id]->remove_species(i));		//einlesen Birth, sterben im Netz
      manager[chosen_id].push_back(t);							//einlesen Death
      break;
    }
  }

  species_count[chosen_id]--; 			// Spezies lebt auf einem Netz weniger
  
  //Anpassen der absoluten Speziesanzahl
  absS--;
  
  //Anpassen der absoluten Verbundenheit
  abs_connectance = abs_connectance - old_connectance + webs[target_web_id]->get_connectance();
  
  // Anpassen von SpN und NpS
  spn -= 1./((double)patches);
  nps -= 1./(((double)S)-1.);
  
      
  if(species_count[chosen_id] == 0)	// Falls die Spezies in keinem Nahrungsnetz mehr lebt:
  {
    out->print_line(Output::OUT_SPECIES, global_species[chosen_id]->biomass, global_species[chosen_id]->feedingcenter, global_species[chosen_id]->feedingrange, &manager[chosen_id]);
    
    
    delete global_species[chosen_id];
    //Erneutes Anpassen von NpS
    nps += 1./(((double)S)-1.);
    nps = (nps*(((double)S)-1.)-1.)/(((double)S)-2.);
      
    S--;
    global_species[chosen_id] = NULL;
  }
  
  return;
}





int Controller::die(Output* out) 
{
 
  //cout << t << " : die" << endl;
 
  if(target_web_id < 0)
  {
    cout << "ctrl->die() trotz target_web_id < 0" << endl;
    t = 1000*t+1000;
    return 0;
  }
 
  webs[target_web_id]->calculate(x, c, d);
  int old_dim = webs[target_web_id]->get_dimension();
  double old_connectance = webs[target_web_id]->get_connectance();	//Verbundenheit zwischenspeichern, vor Veränderung des Netzes
 
  double min1_fitness = 1.0; // Kleinste Fitness
  double min2_fitness = 1.0; // Zweitkleinste Fitness
  for(int i = 1; i < webs[target_web_id]->get_dimension(); i++) // Resource zählt nicht; Finde kleinste und zweitkleinste Fitness
  {
    if(webs[target_web_id]->get_survival(i) < min1_fitness)
    {
      min2_fitness = min1_fitness;
      min1_fitness = webs[target_web_id]->get_survival(i);
    }
    else if(webs[target_web_id]->get_survival(i) < min2_fitness)
    {
      if(webs[target_web_id]->get_survival(i) > min1_fitness)
	min2_fitness = webs[target_web_id]->get_survival(i);
    }
  }
  
 
  
  //Wenn mehrere Spezies gleich schwach, dann zufällige Spezies aus diesen wählen
  int die_index = 0;
  int removed_species_index = -1;
  for(int i = 1; i < webs[target_web_id]->get_dimension(); i++)
  {
    if(webs[target_web_id]->get_survival(i) < (min1_fitness+min2_fitness)/2.0)
    {
      die_index++;
    }
  }
  //if(min1_fitness < 1)
    //cout << t << " : real_die_index = " << die_index << endl;
  die_index *= gsl_rng_uniform(r);
  //if(min1_fitness < 1)
    //cout << t << " : real_die_index = " << die_index << endl;
  
  for(int i = 1; i < webs[target_web_id]->get_dimension(); i++)
  {
    if(webs[target_web_id]->get_survival(i) < (min1_fitness+min2_fitness)/2.0)
    {
      die_index--;
      if(die_index < 0)
      {
	
	removed_species_index = webs[target_web_id]->get_species(i)->get_index();
	
	manager[removed_species_index].push_back(target_web_id);					//einlesen Patch
	if(full_die)
	  manager[removed_species_index].push_back(webs[target_web_id]->remove_species(i,t,out));			//einlesen Birth, sterben im Netz
	else
	  manager[removed_species_index].push_back(webs[target_web_id]->remove_species(i));		//einlesen Birth, sterben im Netz
	manager[removed_species_index].push_back(t);							//einlesen Death
	
	
	break;
      }
    }
  }
  

  
  
  if(removed_species_index > -1)
  {
    species_count[removed_species_index]--; 			//Spezies lebt auf einem Netz weniger
    
      
    //Anpassen der absoluten Speziesanzahl
    absS--;
  
    //Anpassen der absoluten Verbundenheit
    abs_connectance = abs_connectance - old_connectance + webs[target_web_id]->get_connectance();
    
    //Anpassen von SpN und NpS
    spn -= 1./((double)patches);
    nps -= 1./(((double)S)-1.);
    
   if(species_count[removed_species_index] == 0)	//Spezies lebt in keinem Nahrungsnetz mehr:
    {
      out->print_line(Output::OUT_SPECIES, global_species[removed_species_index]->biomass, global_species[removed_species_index]->feedingcenter, global_species[removed_species_index]->feedingrange, &manager[removed_species_index]);
    
      delete global_species[removed_species_index];
      //Erneutes Anpassen von NpS
      nps += 1./(((double)S)-1.);
      nps = (nps*(((double)S)-1.)-1.)/(((double)S)-2.);
      
      S--;
      global_species[removed_species_index] = NULL;
    }
  }
  
  return old_dim - webs[target_web_id]->get_dimension();	//gibt die Anzahl der ausgestorbenen Spezies zurück,
								//stirbt also keine aus wird 0 zurückgegeben
}

/*
void Controller::print(Output* out)
{
  //cout << t << " : print" << endl;
  
  if(target_web_id < 0)
  {
    cout << "ctrl->print(out) trotz target_web_id < 0" << endl;
    t = 100*t+100;
    return;
  }
  //_space_ out->print_line(Output::OUT_BM, S, t, -1, maxS, (void*)global_species);
  //_space_ out->print_line(Output::OUT_C, S, t, -1, maxS, (void*)global_species);
  //_space_ out->print_line(Output::OUT_S, S, t, -1, maxS, (void*)global_species);
  //webs[target_web_id]->calculate(x, c, d);							XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  //webs[target_web_id]->print(out, t);								Muss wieder unkommentiert werden,
  //												wenn etwas anders ausgegeben werden soll
  
  out->print_line(Output::OUT_MC_MD, t, abs_connectance/((double)patches), spn);  
  out->print_line(Output::OUT_SPN_NPS, t, spn, nps);
  
  last_output_time = t;
}
*/

void Controller::print(Output* out)
{
  if(last_output_time < t)
    {
      for(int i = 0; i < patches; i++)
	for(int j = webs[i]->get_dimension()-1; j > -1; j--)					//auch Ressource wird ausgegeben!
	    out->print_line(Output::OUT_STEPS, t, i, webs[i]->get_species(j)->biomass, webs[i]->get_species(j)->feedingcenter, webs[i]->get_species(j)->feedingrange);

	    out->print_line(Output::OUT_MC_MD, t, abs_connectance/((double)patches), spn);  
	    out->print_line(Output::OUT_SPN_NPS, t, spn, nps);
    }
  
  last_output_time = t;
  
  
}


void Controller::clean_up(Output* out)
{
  
  //cout << t << " : clean_up" << endl;
  
  int removed_species_index = -1;
  
  for(int i = 0; i < patches; i++)
    for(int j = webs[i]->get_dimension()-1; j > -1; j--)					//auch Ressource wird entfernt!
    {
      
      removed_species_index = webs[i]->get_species(j)->get_index();
      
	
      manager[removed_species_index].push_back(i);						//einlesen Patch
      manager[removed_species_index].push_back(webs[i]->remove_species(j, t + 1, out));		//einlesen Birth, sterben im Netz
      manager[removed_species_index].push_back(t);						//einlesen Death
       
      species_count[removed_species_index]--; 							//Spezies lebt auf einem Netz weniger
            
      if(species_count[removed_species_index] == 0)						//Spezies lebt in keinem Nahrungsnetz mehr:
      {
	out->print_line(Output::OUT_SPECIES, global_species[removed_species_index]->biomass, global_species[removed_species_index]->feedingcenter, global_species[removed_species_index]->feedingrange, &manager[removed_species_index]);
	
	delete global_species[removed_species_index];
	global_species[removed_species_index] = NULL;
      }

      
    }
  
  delete[] manager;
  manager = NULL;
   
}