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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BASEINTERPOLATOR_H
#define BASEINTERPOLATOR_H

// Tgs
#include <tgs/SharedPtr.h>

#include "Interpolator.h"

namespace Tgs
{
class HilbertRTree;

class BaseInterpolator : public Interpolator
{
public:
  BaseInterpolator();

  virtual ~BaseInterpolator() {}

  virtual double estimateError();

  virtual void readInterpolator(QIODevice& is);

  virtual void setData(const boost::shared_ptr<const DataFrame>& df);

  virtual void setDependentColumns(const vector<string>& labels);

  virtual void setIndependentColumns(const vector<string>& labels);

  virtual void writeInterpolator(QIODevice& os) const;

protected:
  vector<int> _indColumns;
  vector<string> _indColumnsLabels;
  mutable std::auto_ptr<HilbertRTree> _index;
  vector<int> _depColumns;
  vector<string> _depColumnsLabels;
  // A temp variable used to return the result of interpolate()
  mutable vector<double> _result;
 boost::shared_ptr<const DataFrame> _df;

  virtual void _buildModel() = 0;

  virtual void _checkRebuild();

  /**
   * Hold one data sample back for the data frame to estimate the error of the record at index.
   */
  virtual double _estimateError(unsigned int index) const = 0;

  HilbertRTree* _getIndex() const;

  /**
   * To be implemented by child classes to serialize class specific info.
   * The data frame and columns are serialized automatically and it can be assumed that they've
   * been read before _readInterpolator is called.
   */
  virtual void _readInterpolator(QIODevice& is) = 0;

  /**
   * To be implemented by child classes to serialize class specific info.
   * The data frame and columns are serialized automatically.
   */
  virtual void _writeInterpolator(QIODevice& os) const = 0;

};

}

#endif // BASEINTERPOLATOR_H
