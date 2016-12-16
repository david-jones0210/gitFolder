#include "foodweb.h"
#include "species.h"
#include "eval.h"
#include <iostream>
#include <math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
//#include <gsl/gsl_blas.h>			// linear algebra with vectors and matrices
#include <gsl/gsl_sort_vector.h>		// vector operations
#include <gsl/gsl_linalg.h>			// linear algebra for TL (average)-calculation 

using namespace std;

Foodweb::Foodweb() : dim(0), maxdim(1)
{
  return;
}

Foodweb::~Foodweb()
{
  for(int i = 0; i < maxdim; i++)
    delete[] adjacency_matrix[i];
  delete[] adjacency_matrix;
  delete[] trophical_level;
  for(int i = 0; i < maxdim; i++)
    delete[] alpha[i];
  delete[] alpha;
  delete[] competition;
  delete[] competitors;
  delete[] survival;
  delete[] species;
  return;
}

void Foodweb::init(Species* resource, int id, int maximum_size)
{
  
  maxdim = maximum_size + 1;
  adjacency_matrix = new int*[maxdim];
  for(int i = 0; i < maxdim; i++)
    adjacency_matrix[i] = new int[maxdim];
  trophical_level = new double[maxdim];
  
  alpha = new double*[maxdim];
  for(int i = 0; i < maxdim; i++)
    alpha[i] = new double[maxdim];
  competition = new double[maxdim];
  competitors = new double[maxdim];
  survival = new double[maxdim];
  appearance_time = new long long[maxdim];
  species = new Species*[maxdim];
    
  web_id = id;
  dim = 1;
  species[0] = resource;
  adjacency_matrix[0][0] = 0;
  appearance_time[0] = 0;
}

void Foodweb::calculate(double x, double c, double d)
{
  calc_k();
  calc_c(c);
  calc_TL();
  calc_alpha(x);
  calc_surv(d);
  return;
}

double Foodweb::get_TL(int i)
{
  if(i < dim)
    return trophical_level[i];
  cout << "Foodweb::get_TL() - Fehler: Indexfehler" << endl;
  return 0.0;
}

double Foodweb::get_survival(int i)
{
  if(i < dim)
    return survival[i];
  cout << "Foodweb::get_survival() - Fehler: Indexfehler" << endl;
  return 0.0;
}

Species* Foodweb::get_species(int i)
{
  if(i < maxdim)
    return species[i];
  cout << "Foodweb::get_species() - Fehler: Indexfehler" << endl;
  return NULL;
}

long long Foodweb::get_appearance_time(int i)
{
  if(i < maxdim)
    return appearance_time[i];
  cout << "Foodweb::get_appearance_time() - Fehler: Indexfehler" << endl;
  return -1;
}


double Foodweb::get_maxbm()
{
  double res = 0.0;
  for(int i = 0; i < dim; i++)
  {
    if(species[i]->biomass > res)
    {
      res = species[i]->biomass;
    }
  }
  return res;
}

double Foodweb::get_minbm()
{
  double res = 1e20;
  for(int i = 1; i < dim; i++) // Resource ausgenommen.
  {
    if(species[i]->biomass < res)
    {
      res = species[i]->biomass;
    }
  }
  return res;
}

double Foodweb::get_avgbm()
{
  Eval ev;
  for(int i = 0; i < dim; i++)
  {
    ev << species[i]->biomass;
  }
  return ev.get_mean();
}

double Foodweb::get_maxTL()
{
  double res = 0.0;
  for(int i = 0; i < dim; i++)
  {
    if(trophical_level[i] > res)
    {
      res = trophical_level[i];
    }
  }
  return res;
}

double Foodweb::get_avgTL()
{
  Eval ev;
  for(int i = 0; i < dim; i++)
  {
    ev << trophical_level[i];
  }
  return ev.get_mean();
}

double Foodweb::get_connectance()
{
  double l = 0.0;
  
  if(dim <= 1)
  {
    return l = 0;
  }
  else
  {
    if(dim == 0 || dim == 1)
    {
      cout << "dim = " << dim;
    }
    for(int i = 0; i < dim; i++)
    {
      for(int j = 0; j < dim; j++)
      {
	l += adjacency_matrix[i][j];
      }
    }
    return l/(dim * (dim - 1.0));
  }   
}

int Foodweb::get_prey_count(Species* s)
{
  int res = 0;
  for(int i = 0; i < dim; i++)
  {
    if((species[i]->biomass < s->feedingcenter + s->feedingrange) && (species[i]->biomass > s->feedingcenter - s->feedingrange))
      res++;
  }
  return res;
}

void Foodweb::calc_k()
{
  //cout << "k: " << endl;
  for(int i = 0; i < dim; i++)
  {
    competitors[i] = 0.0;
    for(int j = 0; j < dim; j++)
    {
      competitors[i] += adjacency_matrix[j][i];
    }
    //cout << competitors[i] << "\t";
  }
  //cout << endl;
  
  return;
}

void Foodweb::calc_c(double c)
{
  //cout << "c:" << endl;
  for(int i = 0; i < dim; i++)
  {
    // Alt: beta = 1
    //competition[i] = c*competitors[i] / (1.0 + c*competitors[i]);
    // Neu: beta = 2
    competition[i] = (c*competitors[i])*(c*competitors[i]) / (1.0 + (c*competitors[i])*(c*competitors[i]));
    //cout << competition[i] << "\t";
  }
  //cout << endl;
  return;
}

void Foodweb::calc_TL() 
{
  int sig = 1;
  // ******************** Loese Gleichung der Struktur (Matrix)Ta*(Vektor)xa=(Vektor)b 
  gsl_vector *b = gsl_vector_calloc(dim);				// Inhomogenitaet 
  gsl_vector *xa = gsl_vector_calloc(dim);				// gesuchte Variablen
  gsl_permutation *p = gsl_permutation_calloc(dim);			// Permutationsvektor
  gsl_matrix *Ta = gsl_matrix_calloc(dim,dim);				// Matrix des Gleichungssystems

  // ******************** Matrix Ta und Vektor b befüllen
  for(int i = 0; i < dim; i++)
  {
    double norm = 0.0;
    for(int j = 0; j < dim; j++)
    {
      norm += adjacency_matrix[i][j];
    }
    if(norm < 0.5)
    {
      for(int j = 0; j < dim; j++)
      {
	gsl_matrix_set(Ta, i, j, adjacency_matrix[i][j]);
      }
    }
    else
    {
      for(int j = 0; j < dim; j++)
      {
	gsl_matrix_set(Ta, i, j, adjacency_matrix[i][j]/norm);
      }
    }
    gsl_matrix_set(Ta, i, i, gsl_matrix_get(Ta,i,i) - 1.0);
  }

  for(int i = 0; i < dim; i++)
  {
    gsl_vector_set(b, i, -1.0);
  }
  //print_matrix(Ta);

  // ******************** Ta zerlegen und System lösen
  gsl_linalg_LU_decomp(Ta,p,&sig);					
  gsl_linalg_LU_solve(Ta,p,b,xa);	
  

// ******************** Ausgabe
  for(int i = 0; i < dim; i++)
  {
    trophical_level[i] = gsl_vector_get(xa, i);
    //cout << "TL(" << i << ") = " << trophical_level[i] << "\t";
  }
  //cout << endl;
		
// ******************** Speicherplatz freigeben	
  gsl_matrix_free(Ta);
  gsl_vector_free(xa);
  gsl_permutation_free(p);
  gsl_vector_free(b);
  return;
}

void Foodweb::calc_alpha(double x)
{
  for(int i = 0; i < dim; i++)
  {
    for(int j = 0; j < dim; j++)
    {
      if(adjacency_matrix[i][j] == 0)
	alpha[i][j] = 0.0;
      else
	alpha[i][j] = pow(x, trophical_level[i]) * (1.0 - competition[j]);
    }
  }
  return;
}

void Foodweb::calc_surv(double d)
{
  for(int i = 0; i < dim; i++)
  {
    double sum1 = 0.0;
    double sum2 = 0.0;
    for(int j = 0; j < dim; j++)
    {
      sum1 += alpha[i][j];
      sum2 += alpha[j][i];
    }
    survival[i] = sum1/(sum2 + d);
  }
  return;
}

void Foodweb::print(Output* out, long long time)
{
  out->print_line(Output::OUT_PAR, dim, time, web_id, 1, (void*)this);
  out->print_line(Output::OUT_BM, dim, time, web_id, dim, (void*)species);
  out->print_line(Output::OUT_C, dim, time, web_id, dim, (void*)species);
  out->print_line(Output::OUT_S, dim, time, web_id, dim, (void*)species);
  out->print_line(Output::OUT_TROPOS, dim, time, web_id, dim, (void*)trophical_level);
  out->print_line(Output::OUT_ALPHA, dim, time, web_id, dim, (void*)alpha);
  int troph_count[11];
  for(int i = 0; i < 11; i++)
    troph_count[i] = 0;
  for(int i = 0; i < dim; i++)
  {
    troph_count[(int)(trophical_level[i]+0.5)]++;
  }
  for(int i = 10; i >= 0; i--)
  {
    if(troph_count[i] > 0)
    {
      out->print_line(Output::OUT_TROPHDISTRI, dim, time, web_id, i+1, (void*)troph_count);
      break;
    }
  }
}

int Foodweb::add_species(Species* s, long long time)
{
  if(dim >= maxdim)
  {
    cout << "Foodweb::add_species() - Fehler: Maximale Speziesanzahl (" << maxdim << ") im Nahrungsnetz " << web_id << " überschritten!" << endl;
    return -1;
  }
  species[dim] = s;
  appearance_time[dim] = time;
  
  for(int i = 0; i < dim; i++)
  {
    if((s->biomass < species[i]->feedingcenter + species[i]->feedingrange) && (s->biomass > species[i]->feedingcenter - species[i]->feedingrange))
      adjacency_matrix[i][dim] = 1;   //spezies i frisst die neue spezies
    else
      adjacency_matrix[i][dim] = 0;
    if((species[i]->biomass < s->feedingcenter + s->feedingrange) && (species[i]->biomass > s->feedingcenter - s->feedingrange))
      adjacency_matrix[dim][i] = 1;   //spezies i wird gefressen
    else
      adjacency_matrix[dim][i] = 0;
  }
  adjacency_matrix[dim][dim] = 0; //neue spezies friss sich nicht selbst
  dim++;
//   cout << "Adj-Matrix:" << endl;
//   for(int i = 0; i < dim; i++)
//   {
//     for(int j = 0; j < dim; j++)
//     {
//       cout << adjacency_matrix[i][j] << " ";
//     }
//     cout << endl;
//   }
  return dim-1;
}

long long Foodweb::remove_species(int index, long long time, Output* out)
{
  dim--;
  Species* tmp_s = species[index];
  long long tmp_at = appearance_time[index];
  int* tmp_row = adjacency_matrix[index];
  for(int i = index; i < dim; i++)
  {
    species[i] = species[i+1];
    appearance_time[i] = appearance_time[i+1];
    adjacency_matrix[i] = adjacency_matrix[i+1];
  }
  species[dim] = tmp_s;
  appearance_time[dim] = tmp_at;
  adjacency_matrix[dim] = tmp_row;
  for(int j = 0; j <= dim; j++)
  {
    int tmp_col = adjacency_matrix[j][index];
    for(int i = index; i < dim; i++)
    {
      adjacency_matrix[j][i] = adjacency_matrix[j][i+1];
    }
    adjacency_matrix[j][dim] = tmp_col;
  }
  out->print_line(Output::OUT_DIE, dim, time, web_id, 1, (void*)this);
  return tmp_at;
}

long long Foodweb::remove_species(int index)
{
  dim--;
  Species* tmp_s = species[index];
  long long tmp_at = appearance_time[index];
  int* tmp_row = adjacency_matrix[index];
  for(int i = index; i < dim; i++)
  {
    species[i] = species[i+1];
    appearance_time[i] = appearance_time[i+1];
    adjacency_matrix[i] = adjacency_matrix[i+1];
  }
  species[dim] = tmp_s;
  appearance_time[dim] = tmp_at;
  adjacency_matrix[dim] = tmp_row;
  for(int j = 0; j <= dim; j++)
  {
    int tmp_col = adjacency_matrix[j][index];
    for(int i = index; i < dim; i++)
    {
      adjacency_matrix[j][i] = adjacency_matrix[j][i+1];
    }
    adjacency_matrix[j][dim] = tmp_col;
  }
  return tmp_at;
}

/*void Foodweb::remove_species(int index)
{
  dim--;
  //Species* tmp_s = species[index];
  //int* tmp_row = adjacency_matrix[index];
  for(int i = index; i < dim; i++)
  {
    species[i] = species[i+1];
    adjacency_matrix[i] = adjacency_matrix[i+1];
  }
  //species[dim] = tmp_s;
  //adjacency_matrix[dim] = tmp_row;
  for(int j = 0; j <= dim; j++)
  {
    //int tmp_col = adjacency_matrix[j][index];
    for(int i = index; i < dim; i++)
    {
      adjacency_matrix[j][i] = adjacency_matrix[j][i+1];
    }
    //adjacency_matrix[j][dim] = tmp_col;
  }
  return;
}*/

int Foodweb::print_matrix(const gsl_matrix *m)
{
        int status, n = 0;

        for (size_t i = 0; i < m->size1; i++) {
                for (size_t j = 0; j < m->size2; j++) {
                        if ((status = printf( "%g ", gsl_matrix_get(m, i, j))) < 0)
                                return -1;
                        n += status;
                }

                if ((status = printf("\n")) < 0)
                        return -1;
                n += status;
        }

        return n;
}
