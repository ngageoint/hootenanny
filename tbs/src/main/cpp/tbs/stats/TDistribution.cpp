/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */

#include "TDistribution.h"

// Boost
#include <boost/math/special_functions/digamma.hpp>
#include <boost/math/special_functions/gamma.hpp>

// Standard
#include <iostream>
#include <math.h>

// tbs
#include <tbs/optimization/GoldenSectionSearch.h>

using namespace cv;
using namespace std;

namespace tbs
{

class CostT : public LineSearch::Function
{
public:

  CostT(TDistribution* td, const Mat& /*m*/, const vector<double>& EH, const vector<double>& ELogH) :
    _td(td),
    _EH(EH),
    _ELogH(ELogH)
  {

  }

  double operator()(double x) override
  {
    double y = _td->_calculateTCost(x, _EH, _ELogH);
    return y;
  }

private:
  TDistribution* _td;
  const vector<double>& _EH;
  const vector<double>& _ELogH;
};

TDistribution::TDistribution(const Mat& m)
{
  initialize(m);
}

double TDistribution::_calculateDataLogLikelihood(const Mat& m, double v) const
{
  vector<double> delta(m.rows);

  double deltaSum = 0.0;
  for (int i = 0; i < m.rows; i++)
  {
    Mat d = (m.row(i) - _mu) * _sigma.inv() * (m.row(i) - _mu).t();
    delta[i] = d.at<double>(0);

    deltaSum += log1p(delta[i] / v) / 2.0;
  }
  double D = m.cols;

  Mat I = Mat::eye(_sigma.rows, _sigma.cols, CV_64F);
  Mat l1 = I * boost::math::lgamma((v + D) / 2.0);
  Mat l2 = I * D * log(v * M_PI) / 2.0;
  Mat l3 = I * log(determinant(_sigma)) / 2.0;
  Mat l4 = I * boost::math::lgamma(v / 2.0);
  double l5 = (v + D) * deltaSum;

  Mat L = l1 - l2 - l3 - l4 - l5;

  return L.at<double>(0);
}

void TDistribution::_calculateNewMuAndSigma(const vector<double>& EH, const Mat& m)
{
  Mat sumMuNum = Mat::zeros(_mu.rows, _mu.cols, _mu.type());
  Mat sumSigmaNum = Mat::zeros(_sigma.rows, _sigma.cols, _sigma.type());
  double sumDen = EH[0];

  for (int i = 0; i < m.rows; i++)
  {
    sumMuNum += EH[i] * m.row(i);
    sumDen += EH[i];
  }

  _mu = sumMuNum / sumDen;

  for (int i = 0; i < m.rows; i++)
  {
    sumSigmaNum += EH[i] * (m.row(i) - _mu).t() * (m.row(i) - _mu);
  }

  _sigma = sumSigmaNum / sumDen;
}

void TDistribution::_calculateNewV(const Mat& m, const vector<double>& EH, const vector<double>& ELogH)
{
  // Use Line Search to search for the best v.
  CostT ct(this, m, EH, ELogH);
  GoldenSectionSearch gss(0.1);
  _v = gss.argmin(ct, 0, 10000);
}

double TDistribution::_calculateTCost(double v, const vector<double>& EH,
  const vector<double>& ELogH) const
{
// This gives results much closer to expected.
//  return _calculateDataLogLikelihood(m, v);

  double sum = 0.0;

  for (size_t i = 0; i < EH.size(); i++)
  {
    double d1 = v / 2.0 * log(v / 2.0);
    double d2 = boost::math::lgamma(v / 2.0);
    double d3 = ((v / 2.0) - 1.0) * ELogH[i];
    double d4 = v / 2.0 * EH[i];

    d3 = pow(d3, 4);

    double d = d1 + d2 - d3 + d4;

    sum += d;
  }

  //cout << "tCost(" << v << "): " << -sum << endl;

  return -sum;
}

double TDistribution::getLikelihood(const Mat& p) const
{
  return exp(getLogLikelihood(p));
}

double TDistribution::getLogLikelihood(const Mat& p) const
{
  assert(_D == p.cols);
  assert(p.rows == 1);
  // Converted the operation to log likelihood to avoid overflows when v is large.
  double t1 = boost::math::lgamma((_v + _D) / 2.0);
  double t2 = log(pow(_v * M_PI, _D / 2.0) * pow(determinant(_sigma), 0.5));
  double t3 = boost::math::lgamma(_v / 2.0);
  Mat n2 = (p - _mu) * _sigma.inv() * (p - _mu).t();
  double t4 = log(pow(1 + n2.at<double>(0) / _v, -(_v + _D) / 2.0));

  double result = t1 - t2 - t3 + t4;

  return result;
}

void TDistribution::initialize(const Mat& m)
{
  // This algorithm is layed out in [2], page 12
  // Further explanation is in [1], page 86
  // intialize sigma and mu to reasonable values.
  calcCovarMatrix(m, _sigma, _mu, COVAR_NORMAL | COVAR_ROWS | COVAR_SCALE, CV_64F);
  // intialize v to 1000
  _v = 1000;
  _D = m.cols;

  bool done = false;

  vector<double> delta(m.rows);

  vector<double> EH(m.rows);
  vector<double> ELogH(m.rows);

  int count = 0;

  double oldL = 1e6;

  while (!done && count < 100)
  {
    // Expectation Step
    for (int i = 0; i < m.rows; i++)
    {
      Mat t = (m.row(i) - _mu).t();
      // possible problem, had to swap the order of transpose to work w/ multiple dimensions.
      Mat d = (m.row(i) - _mu) * _sigma.inv() * (m.row(i) - _mu).t();
      delta[i] = d.at<double>(0);
      EH[i] = (_v + _D) / (_v + delta[i]);
      ELogH[i] = boost::math::digamma<double>(_v / 2.0 + _D / 2.0) - log(_v / 2.0 + delta[i] / 2.0);
    }

    // Maximization Step

    // calculate new mu and sigma
    _calculateNewMuAndSigma(EH, m);
    // calculate new v
    _calculateNewV(m, EH, ELogH);

    // Compute data log likelihood
    double L = _calculateDataLogLikelihood(m, _v);
    if (abs(oldL - L) < 0.1)
    {
      done = true;
    }
    oldL = L;
    count++;
  }
}

Mat TDistribution::_log(const Mat& m) const
{
  Mat result;
  log(m, result);
  return result;
}

}
