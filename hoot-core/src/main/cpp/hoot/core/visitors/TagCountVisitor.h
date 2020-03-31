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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TAGCOUNTVISITOR_H
#define TAGCOUNTVISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/info/NumericStatistic.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

/**
 * A visitor for counting element tags.  It distinguishes between metadata and information tags,
 * and both are included in the total count.  Debug tags are not included in the total count.
 */
class TagCountVisitor : public ConstElementVisitor, public NumericStatistic
{
public:

  static std::string className() { return "hoot::TagCountVisitor"; }

  TagCountVisitor();

  virtual void visit(const ConstElementPtr& e) override;

  virtual QString getDescription() const override { return "Calculates tag count statistics"; }

  virtual QString getInitStatusMessage() const override
  { return "Calculating tag count statistics..."; }

  virtual QString getCompletedStatusMessage() const override
  { return "Calculated tag count statistics for " + QString::number(_numAffected) + " elements"; }

  virtual long numWithStat() const override { return _numAffected; }
  virtual double getStat() const override { return _totalCount; }
  virtual double getMin() const override { return _smallestCount; }
  virtual double getMax() const override { return _largestCount; }
  virtual double getAverage() const override
  {
    const double average = _numAffected == 0 ? 0.0 : _totalCount / _numAffected;
    return average;
  }

  long numWithInformationStat() const { return _numInformationAffected; }
  virtual long getInformationCount() const { return _informationCount; }
  virtual long getInformationMin() const { return _smallestInformationCount; }
  virtual long getInformationMax() const { return _largestInformationCount; }
  virtual double getInformationAverage() const
  {
    const double average =
      _numInformationAffected == 0 ? 0.0 :
        (double)_informationCount / (double)_numInformationAffected;
    return average;
  }

  virtual std::string getClassName() const { return className(); }

private:

  long _totalCount;
  long _smallestCount;
  long _largestCount;

  long _numInformationAffected;
  long _informationCount;
  long _smallestInformationCount;
  long _largestInformationCount;
};

}

#endif // TAGCOUNTVISITOR_H
