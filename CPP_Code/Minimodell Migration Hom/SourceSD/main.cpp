#include <iostream>      
#include <cstdlib>
#include <ctime>
#include "species.h"
#include "foodweb.h"
#include "controller.h"
#include "output.h"
#include <sys/stat.h>
#include <vector>

using namespace std;



int main(int argc, char** argv)
{
  if(argc != 15)
  {
    cout << "Falsche Parameteranzahl! Aufruf mit:" << endl << "./working ausgabeordner seed x c d col row prop prop_d frM frV steps numberOfMutationsPerPatch full_die.out" << endl;
    return 0;
  }
  char* path = argv[1];
  int seed = atoi(argv[2]);
  double x = atof(argv[3])/1e6;
  double c = atof(argv[4])/1e6;
  double d = atof(argv[5])/1e6;
  int columns = atoi(argv[6]);
  int rows = atoi(argv[7]);
  double migration_proportion = atof(argv[8])/1e3;
  double death_proportion = atof(argv[9])/1e3;
  double feedingrange_min = atof(argv[10])/1e3;
  double feedingrange_var = atof(argv[11])/1e3;
  int steps = atoi(argv[12]);
  long long nompp = atoll(argv[13]);
  int fdo = atoi(argv[14]);
  
  long long runtime = nompp*columns*rows*(1 + migration_proportion + death_proportion);
  
  long long stepsize = 0;
  
  if(steps < 1)
    stepsize = -1;
  else
  {
    if(steps < runtime)
      stepsize = runtime/steps;
    else
      stepsize = 1;
  }
  long long curr = -1;
  
  
  const int scenario = 2*3*5*7;
  
  time_t start_time = clock();
  time_t timer = clock();
  cout << endl;
  cout << "=========================================================" << endl;
  cout << "Programmstart mit Parametern:" << endl;
  cout << "path = " << path << endl;
  cout << "seed = " << seed << endl;
  cout << "x = " << x << " | c = " << c << " | d = " << d << endl;
  cout << "col = " << columns << " | row = " << rows << endl;
  cout << "prop = " << migration_proportion << endl;
  cout << "prop_d = " << death_proportion << endl;
  cout << "frM = " << feedingrange_min << endl;
  cout << "frV = " << feedingrange_var << endl;
  cout << "steps = " << steps << endl;
  cout << "numberOfMutationsPerPatch = " << nompp << endl;
  cout << "runtime = " << runtime << endl;
  cout << "stepsize = " << stepsize << endl;
  
  
  
  Output* out = new Output();
  
  const int dir_err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  if(-1 == dir_err)
    cout << "Error creating directory!" << endl;
    
  
  out->open_files(path);
  
  
//   out->mute(Output::OUT_DIE);
//   out->mute(Output::OUT_PAR);
//   out->mute(Output::OUT_S);
//   out->mute(Output::OUT_TROPHDISTRI);
//   out->mute(Output::OUT_C);
//   out->mute(Output::OUT_ALPHA);
//   out->mute(Output::OUT_BM);
//   out->mute(Output::OUT_TROPOS);
  
  bool full_die = false;
  if(fdo==1)
    full_die = true;
  
  
  Controller* ctrl = new Controller();
  ctrl->init(seed, columns, rows, migration_proportion, death_proportion, feedingrange_min, feedingrange_var, 0 , 20 * columns * rows + 1000, full_die);
  ctrl->set_x(x);
  ctrl->set_c(c);
  ctrl->set_d(d);
  
  while(ctrl->get_t() < runtime)
  {
    curr++;
    if(curr == stepsize)
    {
      ctrl->print(out);
      curr = 0;
    }
    
    if(ctrl->find_chosen_and_do_action(out))	
    {
      while(ctrl->die(out)) continue;
          
      /*ctrl->print(out);*/
      
      if((double)(clock()-timer)/CLOCKS_PER_SEC > 60.0)
      {
	if((double)(clock()-start_time)/CLOCKS_PER_SEC < 3660.0)
	{
	  timer = clock();
	  cout << "t = " << ctrl->get_t() << " (" << (double)(clock()-start_time)/CLOCKS_PER_SEC << " s)" << endl;
	}
	else if((double)(clock()-timer)/CLOCKS_PER_SEC > 1800.0)
	{
	  timer = clock();
	  cout << "t = " << ctrl->get_t() << " (" << ((double)(clock()-start_time)/CLOCKS_PER_SEC)/3600.0 << " h)" << endl;
	}
      }
    }
  }
  
  int success_migration = ctrl->get_success_migration(); 
  int success_mutation = ctrl->get_success_mutation(); 
  int success = success_migration + success_mutation;
  int all_webs = ctrl->get_counter_all_webs();
  int all_webs_S1 = ctrl->get_counter_all_webs_S1();
  int inbound = ctrl->get_counter_inbound();
  int inbound_S1 = ctrl->get_counter_inbound_S1();
  
  
  ctrl->print(out);
  ctrl->clean_up(out);
  
  delete ctrl;
  
  
  out->write_params(path, scenario, seed, x, c, d);		//gesammelte Daten über mehrere Läufe
  out->close_files();
  
  cout << "Programmende! Laufzeit: " << (double)(clock()-start_time)/CLOCKS_PER_SEC << " s" << endl;
  cout << endl;
  cout << "Es gab " << success_migration << " erfolgreiche Migrationen (" << 100*(double)((double)success_migration/(double)runtime) << "%)" << endl;
  cout << "und " << success_mutation << " erfolgreiche Mutationen (" << 100*(double)((double)success_mutation/(double)runtime) << "%)." << endl;
  cout << "Damit wurde effektiv in " << 100*(double)success_migration/(success) << "% der Fälle migriert," << endl;
  cout << "im Vergleich zu den gegebenen " << 100*migration_proportion/(1+migration_proportion) << "%." << endl;
  cout << endl;
  cout << "Die Migration scheiterte in " << inbound << " (" << 100*(double)((double)inbound/(double)runtime*(double)(1.+migration_proportion + death_proportion)/(double)migration_proportion) << "%) Fällen, weil nach innen migriert wurde" << endl;
  cout << "(in " << inbound_S1 << " (" << 100*(double)((double)inbound_S1/((double)inbound)) << "%) Fällen durch die Anfangsspezies verursacht)." << endl;
  cout << "Davon waren " << all_webs << " (" << 100*(double)((double)all_webs/(double)inbound) << "%) Migrationen von Spezies, die auf allen Patches existiert haben" << endl;
  cout << "(in " << all_webs << " (" << 100*(double)((double)all_webs/((double)all_webs)) << "%) Fällen durch die Anfangsspezies verursacht)." << endl;
  cout << "Insgesamt gab es " << success << " erfolgreiche Veränderungen." << endl;
  cout << "Damit lag die Erfolgsrate bei " << 100*(double)((double)success/(double)((double)runtime*(double)(1.+migration_proportion)/(double)(1.+migration_proportion + death_proportion))) << "%." << endl;
  cout << "=========================================================" << endl;
  cout << endl;
  
  delete out;

  return 0;                                        
}
