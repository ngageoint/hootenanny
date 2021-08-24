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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef DATASAMPLES_H
#define DATASAMPLES_H

// Tgs
#include <tgs/RandomForest/DataFrame.h>

namespace hoot
{

using Sample = std::map<QString, double>;

/**
 * A very simple construct for storing simple data samples.
 */
class DataSamples : public std::vector<Sample>
{
public:

  DataSamples() = default;
  virtual ~DataSamples() = default;

  std::vector<std::string> getUniqueLabels() const;

  std::shared_ptr<Tgs::DataFrame> toDataFrame(double nullValue) const;

  const Sample& get(size_t i) const { return operator[](i); }

};

}

#endif // DATASAMPLES_H
