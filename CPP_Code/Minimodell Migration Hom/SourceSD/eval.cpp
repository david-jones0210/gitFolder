#include "eval.h"

Eval::Eval() : n(0), sum(0.0), sum_sqr(0.0)
{
  return;
}

Eval::~Eval()
{
  return;
}

double Eval::get_mean()
{
  if(n > 0)
    return sum/n;
  return 0.0;
}

double Eval::get_variance()
{
  if(n > 1)
    return (sum_sqr - sum*sum/n)/(n-1);
  return 0.0;
}

Eval& Eval::operator<<(const double d)
{
  n++;
  sum += d;
  sum_sqr += d*d;
  return *this;
}