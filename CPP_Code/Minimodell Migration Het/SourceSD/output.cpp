#include "output.h"
#include "foodweb.h"
#include <math.h>

Output::Output() : mute_flags(0)
{
  init_params();
}

Output::~Output()
{
  int i = 0;
  while(ev_params[i] != NULL)
  {
    delete ev_params[i];
    i++;
  }
  delete ev_params;
}

void Output::init_params()
{
  const int param_count = 7;		//Anzahl der Auszuwertenden Größen, ergibt sich aus den Festlegungen in collect_params()
  ev_params = new Eval*[param_count+1];
  for(int i = 0 ; i < param_count; i++)
  {
    ev_params[i] = new Eval();
  }
  ev_params[param_count] = NULL;
}

void Output::write_params(string path, int scenario, int seed, double x, double c, double d)
{
  string name = path + string("/mm_par.out");
  file[0].open(name.c_str(), ios::out | ios::app);
  
  file[0] << scenario << "\t" << x << "\t" << c << "\t" << d << "\t" << seed;
  file[0] << "\t" << ev_params[0]->get_n();
  int i = 0;
  while(ev_params[i] != NULL)
  {
    file[0] << "\t" << ev_params[i]->get_mean() << "\t" << sqrt(ev_params[i]->get_variance());
    i++;
  }
  file[0] << endl;
  
  file[0].close();
}

void Output::collect_params(void* foodweb)
{
  Foodweb* web = (Foodweb*)foodweb;
  
  *ev_params[0] << web->get_dimension();	// Anzahl der Spezies S+1
  *ev_params[1] << web->get_maxbm();		// Maximale Körpergröße einer Spezies im Netz
  *ev_params[2] << web->get_minbm();		// Minimale Körpergröße einer Spezies im Netz mit ausnahme der Resource
  *ev_params[3] << web->get_avgbm();		// Durchschnittliche Körpergrößer der Spezies (incl. Resourc)
  *ev_params[4] << web->get_maxTL();		// analog bm
  *ev_params[5] << web->get_avgTL();		// analog bm
  *ev_params[6] << web->get_connectance();	// Connectance des Netzwerk, = #links / dim^2
}


void Output::open_files(string path)
{
  string c[] = {"par", "bm", "c", "s", "tropos", "alpha", "trophdistri", "die", "spn_nps", "mc_md", "species", "steps"};
  for(int i = 1; i < OUT_FILE_COUNT; i++) // i = 1, da die par datei nicht mit geöffnet wird, diese wird gesondert über write_params() beschrieben.
  {
    string name = path + string("/mm_") + c[i] + string(".out");
    file[i].open(name.c_str());
  }
}

void Output::close_files()
{
  for(int i = 1; i < OUT_FILE_COUNT; i++)
  {
    file[i].close();
  }
}

void Output::mute(resfile_type f)
{
  mute_flags |= (0x01 << (unsigned int)f);
}

void Output::unmute(resfile_type f)
{
  if(mute_flags & (0x01 << (unsigned int)f))
    mute_flags -= (0x01 << (unsigned int)f);
}

void Output::print_line(resfile_type f, int dim, long long time, int patch, int array_size, void* data)
{
  // Testen, ob die Ausgabe unterdrückt wurde:
  if(mute_flags & (0x01 << (unsigned int)f))
    return;
  
  switch(f)
  {
    case OUT_PAR:
      collect_params(data);
      break;

    case OUT_BM:
    case OUT_C:
    case OUT_S:
    {
      Species** s = (Species**)data;
      file[f] << dim << "\t" << time << "\t" << patch;
      for(int i = 0; i < array_size; i++)
      {
	if(s[i])
	{
	  file[f] << "\t";
	  switch(f)
	  {
	    case OUT_BM:
	      file[OUT_BM] << s[i]->biomass;
	      break;
	    case OUT_C:
	      file[OUT_C] << s[i]->feedingcenter;
	      break;
	    case OUT_S:
	      file[OUT_S] << s[i]->feedingrange;
	      break;
	  }
	}
      }
      file[f] << endl;
      break;
    }
    case OUT_TROPOS:
    {
      double* tl = (double*)data;
      file[OUT_TROPOS] << dim << "\t" << time << "\t" << patch;
      for(int i = 0; i < array_size; i++)
      {
	file[OUT_TROPOS] << "\t" << tl[i];
      }
      file[OUT_TROPOS] << endl;
      break;
    }
    case OUT_ALPHA:
    {
      double** a = (double**)data;
      file[OUT_ALPHA] << dim << "\t" << time << "\t" << patch;
      for(int i = 0; i < array_size; i++)
      {
	for(int j = 0; j < array_size; j++)
	{
	  file[OUT_ALPHA] << "\t" << a[i][j];
	}
      }
      file[OUT_ALPHA] << endl;
      break;
    }
    case OUT_TROPHDISTRI:
    {
      int* d = (int*)data;
      file[OUT_TROPHDISTRI] << dim << "\t" << time << "\t" << patch;
      for(int i = 0; i < array_size; i++)
      {
	file[OUT_TROPHDISTRI] << "\t" << d[i];
      }
      file[OUT_TROPHDISTRI] << endl;
      break;
    }
    case OUT_DIE: //Sollte vom Foodweb direkt aufgerufen werden
    {
      Foodweb* web = (Foodweb*)data;
      file[f] << patch << "\t" << web->get_appearance_time(dim) << "\t" << time << "\t" << web->get_species(dim)->biomass << "\t" << web->get_species(dim)->feedingcenter << "\t" << web->get_species(dim)->feedingrange << endl;
      break;
    }
    default:
      cout << "Output::print_line() - Fehler!!!" << endl;
  }
  return;
}

void Output::print_line(resfile_type f, long long time, double info1, double info2)
{
  // Testen, ob die Ausgabe unterdrückt wurde:
  if(mute_flags & (0x01 << (unsigned int)f))
    return;
  
  file[f] << time << "\t" << info1 << "\t" << info2 << endl;
  
  return;  
}

void Output::print_line(resfile_type f, double bm, double fc, double fr, list<long long>* manager)
{
  // Testen, ob die Ausgabe unterdrückt wurde:
  if(mute_flags & (0x01 << (unsigned int)f))
    return;
  
  file[f] << bm << "\t" << fc << "\t" << fr;

  
  while(manager->size() > 0)
  {
    file[f] << "\t" << manager->front();
    manager->pop_front();
  }
  
   file[f] << endl;
  
  return;  
}

void Output::print_line(resfile_type f,long long time, int patch, double bm, double fc, double fr)
{
  // Testen, ob die Ausgabe unterdrückt wurde:
  if(mute_flags & (0x01 << (unsigned int)f))
    return;
  
  file[f] << time << "\t" << patch << "\t" << bm << "\t" << fc << "\t" << fr << endl;
  
  return;  
}
  