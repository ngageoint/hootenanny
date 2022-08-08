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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "DataSummaryTagVisitor.h"

// hoot
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/UuidHelper.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, DataSummaryTagVisitor)

DataSummaryTagVisitor::DataSummaryTagVisitor()
  : _sources(ConfigOptions().getDataSummaryTagSources())
{
}

DataSummaryTagVisitor::DataSummaryTagVisitor(const QString& sources)
  : _sources(sources.split(";"))
{
}

void DataSummaryTagVisitor::setConfiguration(const Settings& conf)
{
  _sources = ConfigOptions(conf).getDataSummaryTagSources();
}

void DataSummaryTagVisitor::visit(const ElementPtr& e)
{
  Tags& tags = e->getTags();
  if (tags.contains(MetadataTags::SourceImagery()))
  {
    QStringList list = tags.get(MetadataTags::SourceImagery()).split(";");
    QSet<QString> values = list.toSet();
    int first = -1;
    int last = -1;
    int observations = 0;
    int misses = 0;
    int consecutive = 0;
    int consecutive_start = -1;
    int max_consecutive = 0;
    int max_consecutive_start = -1;
    std::ostringstream hist;

    for (int i = 0; i < _sources.size(); ++i)
    {
      if (values.contains(_sources[i]))
      {
        //  Observed
        observations++;
        //  Check if this is the first observation
        if (first < 0)
          first = i;
        //  Set the last on each observation
        last = i;
        //  Update the consecutive
        consecutive++;
        if (consecutive_start == -1)
          consecutive_start = i;
        //  Update the histogram
        hist << "X";
      }
      else
      {
        //  Missed
        misses++;
        //  Consecutive streak is broken
        if (max_consecutive <= consecutive)
        {
          max_consecutive = consecutive;
          max_consecutive_start = consecutive_start;
        }
        consecutive = 0;
        consecutive_start = -1;
        //  Update the histogram
        hist << "_";
      }
    }
    //  Save off the max_consecutive if needed
    if (consecutive != 0 && max_consecutive <= consecutive)
    {
      max_consecutive = consecutive;
      max_consecutive_start = consecutive_start;
    }
    //  Add the tags
    tags[MetadataTags::HootFirstSeen()] = (first >= 0 ? _sources[first] : "N/A");
    tags[MetadataTags::HootLastSeen()] = (last >= 0 ? _sources[last] : "N/A");
    tags[MetadataTags::HootTotalObservations()] = QString("%1").arg(observations);
    tags[MetadataTags::HootTotalMisses()] = QString("%1").arg(misses);
    tags[MetadataTags::HootMaxConsecutive()] = QString("%1").arg(max_consecutive);
    tags[MetadataTags::HootMaxConsecutiveStart()] = (max_consecutive_start >= 0 ? _sources[max_consecutive_start] : "N/A");
    tags[MetadataTags::HootHistogram()] = QString(hist.str().c_str());
  }
}

}
