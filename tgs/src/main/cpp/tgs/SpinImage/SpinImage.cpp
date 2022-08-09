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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */

#include "SpinImage.h"

// Standard Includes
#include <algorithm>
#include <assert.h>
#include <iostream>
using namespace std;

#include "../StreamUtils.h"
#include "../TgsException.h"
#include "../Xml/XmlHelper.h"
#include "Points.h"

namespace Tgs
{
  double atanh(double x)
  {
    if (fabs(x) >= 1)
    {
      if (x >= 1.0 && x - 1.0 < 1e-3)
      {
        // atanh(.9999999999)
        return 17.61636;
      }
      if (x <= -1.0 && fabs(x) - 1.0 < 1e-3)
      {
        // atanh(-.9999999999)
        return -17.61636;
      }
      cout << x << endl;
      throw Exception("fabs(x) must be < 1");
    }
    return .5 * (log((1 + x) / (1 - x)) / log(2.71828183));
  }

  SpinImage::SpinImage()
  {
    clear();
  }

  void SpinImage::addValue(double alpha, double beta)
  {
    Point2d bounds[4];

    if (_ignoreReflection)
    {
      beta = fabs(beta);
    }

    if (alpha > _maxAlpha || beta > _maxBeta || beta < _minBeta || alpha < 0)
    {
      return;
    }

    double alphaBinSize = _maxAlpha / _alphaBins;
    double betaBinSize = (_maxBeta - _minBeta) / _betaBins;
    
    // round the alpha value down to the center of the nearest lower bin
    double alphaDown = _calculateAlphaCenter(_calculateLowerAlpha(alpha));
    // add one bin to get the center of the next upper bin
    double alphaUp = _calculateAlphaCenter(_calculateLowerAlpha(alpha) + 1);

    // round the beta value down
    double betaDown = _calculateBetaCenter(_calculateLowerBeta(beta));
    // add one bin to get the center of the next upper bin
    double betaUp = _calculateBetaCenter(_calculateLowerBeta(beta) + 1);

    bounds[0] = Point2d(alphaDown, betaDown);
    bounds[1] = Point2d(alphaDown, betaUp);
    bounds[2] = Point2d(alphaUp, betaDown);
    bounds[3] = Point2d(alphaUp, betaUp);

    // we're using inverse distance weighting rather than bilinear interpolation requested by the
    // paper. bilinear doesn't make sense to me.
    Point2d p(alpha, beta);
    double weight = 0.0;
    double distance[4];
    for (int i = 0; i < 4; i++)
    {
      double d = _distance(p, bounds[i]);
      distance[i] = d;
      weight += 1 / d;
      // if we're right on top of one of the bins, put all the goods in that one bin
      if (d < 1e-6)
      {
        _bins[_calculateIndex(alpha, beta)] += 1.0;
        return;
      }
    }

    for (int i = 0; i < 4; i++)
    {
      int row = (int)(bounds[i].p1 / alphaBinSize);
      int col = (int)((bounds[i].p2 - _minBeta) / betaBinSize);
      if (row >= 0 && row < _alphaBins && col >= 0 && col < _betaBins)
      {
        bin(row, col) += (float)(1 / distance[i] / weight);
      }
    }
  }

  double SpinImage::_calculateAlphaCenter(int alphaCell) const
  {
    double alphaBinSize = _maxAlpha / _alphaBins;
    return alphaCell * alphaBinSize + alphaBinSize / 2.0;
  }

  double SpinImage::_calculateBetaCenter(int betaCell) const
  {
    double betaBinSize = _maxBeta / _betaBins;
    return betaCell * betaBinSize + betaBinSize / 2.0;
  }

  int SpinImage::_calculateIndex(double alpha, double beta) const
  {
    assert(alpha <= _maxAlpha && alpha >= 0.0 && beta >= _minBeta && beta <= _maxBeta);
    int aBin = (int)(alpha / _maxAlpha * (double)_alphaBins);
    int bBin = (int)((beta - _minBeta) / (_maxBeta - _minBeta) * (double)_betaBins);
    int result = aBin + bBin * _alphaBins;
    assert(result >= 0 && result < (int)_bins.size());
    return result;
  }

  int SpinImage::_calculateLowerAlpha(double alpha) const
  {
    double alphaBinSize = _maxAlpha / _alphaBins;
    // round the alpha value down to the center of the nearest lower bin
    int col = (int)(floor((alpha  - alphaBinSize / 2.0) / _maxAlpha * (double)_alphaBins) + .5);
    if (col < 0)
    {
      col = 0;
    }
    return col;
  }

  int SpinImage::_calculateLowerBeta(double beta) const
  {
    double betaBinSize = _maxBeta / _betaBins;
    // round the beta value down to the center of the nearest lower bin
    int row = (int)(floor((beta  - betaBinSize / 2.0) / (_maxBeta - _minBeta) * 
      (double)_betaBins) + .5);
    if (row < 0)
    {
      row = 0;
    }
    return row;
  }

  double SpinImage::_calculateCorrelation(const SpinImage& si, int& n) const
  {
    n = 0; // count overlap
    float muX = 0.0;
    float muY = 0.0;
    for (unsigned int i = 0; i < _bins.size(); i++)
    {
      float x = _bins[i];
      float y = si._bins[i];

      switch (_compareAlgo)
      {
      case CompareAllCells:
        muX += x;
        muY += y;
        n++;
      	break;
      case CompareNonZeroCells:
        if (x > 0 && y > 0)
        {
          muX += x;
          muY += y;
          n++;
        }
        break;
      case CompareThisNonZeroCells:
        // [1] says to only compare points that are non-zero to reduce problems from clutter. It 
        // seems to me that since the training data is "clean" we should compare all points that
        // are non-zero in the training data (*this).
        if (x > 0)
        {
          muX += x;
          muY += y;
          n++;
        }
        break;
      }
    }

    if (n == 0)
    {
      return 0;
    }

    muX /= n;
    muY /= n;

    float numerator = 0;
    float denominator1 = 0, denominator2 = 0;
    for (unsigned int i = 0; i < _bins.size(); i++)
    {
      float x = _bins[i];
      float y = si._bins[i];

      switch (_compareAlgo)
      {
      case CompareAllCells:
        numerator += (x - muX) * (y - muY);
        denominator1 += (x - muX) * (x - muX);
        denominator2 += (y - muY) * (y - muY);
        break;
      case CompareNonZeroCells:
        if (x > 0 && y > 0)
        {
          numerator += (x - muX) * (y - muY);
          denominator1 += (x - muX) * (x - muX);
          denominator2 += (y - muY) * (y - muY);
        }
        break;
      case CompareThisNonZeroCells:
        // [1] says to only compare points that are non-zero to reduce problems from clutter. It 
        // seems to me that since the training data is "clean" we should compare all points that
        // are non-zero in the training data (*this).
        if (x > 0)
        {
          numerator += (x - muX) * (y - muY);
          denominator1 += (x - muX) * (x - muX);
          denominator2 += (y - muY) * (y - muY);
        }
        break;
      }
    }
    double correlation = numerator / sqrt(denominator1 * denominator2);

    return correlation;
  }

  void SpinImage::clear()
  {
    _compareAlgo = CompareThisNonZeroCells;
    _ignoreReflection = true;
    _alphaBins = 0;
    _betaBins = 0;
    resize(0, 0);
  }

  double SpinImage::compare(const SpinImage& si, int minUseful) const
  {
    if (si.getAlphaBins() != getAlphaBins() || si.getBetaBins() != getBetaBins())
    {
      return -1;
    }

    double lambda = 3;
    int n;
    double correlation = _calculateCorrelation(si, n);

    if (n == 0)
    {
      return 0;
    }
    
    double at = atanh(correlation);
    double score = at * at - lambda * (1 / (n - 3));
    if (n < minUseful)
    {
      score = 0;
    }
//     if (score > 200)
//     {
//       printf("Unlikely comparison score: %g corr: %g at: %g lambda: %g n: %d\n", score, correlation, at, lambda, n);
//     }
    return score;
  }

  double SpinImage::_distance(const Point2d& p1, const Point2d& p2) const
  {
    double dx = p1.p1 - p2.p1;
    double dy = p1.p2 - p2.p2;
    return sqrt(dx * dx + dy * dy);
  }

  void SpinImage::exportImage(std::ostream& s, std::string tabDepth) const
  {
    s << tabDepth << "<SpinImage"
      << " compareAlgo=" << _compareAlgo
      << " alphaBins=" << _alphaBins
      << " betaBins=" << _betaBins
      << " maxAlpha=" << _maxAlpha
      << " minBeta=" << _minBeta
      << " maxBeta=" << _maxBeta
      << " ignoreReflection=" << _ignoreReflection
      << ">";
    s.precision(8);
    s << scientific;
    for (unsigned int i = 0; i < _bins.size(); i++)
    {
      if (i % getAlphaBins() == 0)
      {
        s << endl << tabDepth;
      }
      s  << " " << _bins[i];
    }
    s << endl << tabDepth << "</SpinImage>" << endl;
  }

  void SpinImage::importImage(std::istream& s)
  {
    string buffer;
    getline(s, buffer);

    buffer = XmlHelper::trim(buffer);

    if (buffer == "<SpinImage/>")
    {
      clear();
      return;
    }

    HashMap<string, string> m = XmlHelper::parseAttributes(buffer);
    if (m.size() != 7)
    {
      throw Exception("Error while parsing. Wrong numer of attributes.");
    }
    _compareAlgo = conv<int>(m["compareAlgo"]);
    _alphaBins = conv<int>(m["alphaBins"]);
    _betaBins = conv<int>(m["betaBins"]);
    _maxAlpha = conv<double>(m["maxAlpha"]);
    _minBeta = conv<double>(m["minBeta"]);
    _maxBeta = conv<double>(m["maxBeta"]);
    _ignoreReflection = conv<bool>(m["ignoreReflection"]);

    int size = _alphaBins * _betaBins;
    _bins.resize(0);
    _bins.reserve(size);
    getline(s, buffer);
    char str[2 << 16];
    buffer = XmlHelper::trim(buffer);
    while(buffer != "</SpinImage>")
    {
      strcpy(str, buffer.c_str());
      char* token = strtok(str, " ");
      while (token)
      {
        if ((int)_bins.size() + 1 > size)
        {
          cout << _bins.size() << " " << size << endl;
          throw Exception("Error while parsing. SpinImage has too many values.");
        }
        _bins.push_back((float)atof(token));
        token = strtok(NULL, " ");
      }
      getline(s, buffer);
      buffer = XmlHelper::trim(buffer);
    }
  }

  float SpinImage::getValue(double alpha, double beta) const
  {
    return _bins[_calculateIndex(alpha, beta)];
  }

  void SpinImage::resize(int alphaBins, int betaBins)
  {
    _alphaBins = alphaBins;
    _betaBins = betaBins;
    _bins.resize(_alphaBins * _betaBins, 0.0);
  }

  void SpinImage::setBounds(double maxAlpha, double minBeta, double maxBeta)
  {
    _maxAlpha = maxAlpha;
    if (_ignoreReflection == true)
    {
      _minBeta = 0.0;
    }
    else
    {
      _minBeta = minBeta;
    }
    _maxBeta = maxBeta;
  }
}
