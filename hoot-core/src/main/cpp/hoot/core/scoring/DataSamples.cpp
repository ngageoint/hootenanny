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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

#include "DataSamples.h"

#include <hoot/core/conflate/matching/MatchType.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

vector<string> DataSamples::getUniqueLabels() const
{
  vector<string> labels;

  set<QString> labelSet;

  for (size_t i = 0; i < size(); i++)
  {
    for (Sample::const_iterator it = get(i).begin(); it != get(i).end(); ++it)
    {
      if (it->first != "class")
      {
        labelSet.insert(it->first);
      }
    }
  }

  for (set<QString>::const_iterator it = labelSet.begin(); it != labelSet.end(); ++it)
  {
    labels.push_back((*it).toStdString());
  }

  return labels;
}

std::shared_ptr<DataFrame> DataSamples::toDataFrame(double nullValue) const
{
  std::shared_ptr<DataFrame> result(new DataFrame);
  vector<string> labels = getUniqueLabels();
  result->setFactorLabels(labels);

  vector<double> record;
  record.resize(labels.size());

  for (size_t i = 0; i < size(); i++)
  {
    const Sample& s = get(i);

    MatchType type = MatchType(s.find("class")->second);
    string classLabel = type.toString().toLower().toStdString();

    for (size_t j = 0; j < labels.size(); j++)
    {
      Sample::const_iterator it = get(i).find(QString::fromStdString(labels[j]));
      if (it != get(i).end())
      {
        record[j] = it->second;
      }
      else
      {
        record[j] = nullValue;
      }
    }
    result->addDataVector(classLabel, record);
  }

  return result;
}


}
