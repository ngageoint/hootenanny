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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 1998, 1999 Eibe Frank, Leonard Trigg, Mark Hall
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef LOGISTICREGRESSION_H
#define LOGISTICREGRESSION_H

/**
 * @author Weka
 * @copyright GPLv3
 * http://www.cs.waikato.ac.nz/ml/weka/
 * This code snippet was shamelessly taken from Weka's logistic regression class.
 */

// Qt
#include <QString>

// Standard
#include <map>

namespace hoot
{

/**
 * Yet another logistic regression class. This one does not train on data (rely on Weka for that).
 * You provide a set of values and it will apply Logistic regression to those values.
 */
class LogisticRegression
{
public:
  typedef std::map<QString, double> Coeff;

  LogisticRegression();

  void setCoefficients(const Coeff& coeff) { _coeff = coeff; }

  void setIntercept(double intercept) { _intercept = intercept; }

  double evaluate(std::map<QString, double> sample);

private:
  Coeff _coeff;
  double _intercept;
};

}

#endif // LOGISTICREGRESSION_H
