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
#ifndef STATSCOMPOSER_H
#define STATSCOMPOSER_H

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/ops/stats/SingleStat.h>

// Standard
#include <string>

#include <QStringList>
#include <QList>
#include <QVariantMap>

namespace hoot {
using namespace std;

class StatsComposer
{
public:

  static string className() { return "hoot::StatsComposer"; }

  virtual ~StatsComposer() {}

  /**
   * Arguments are passed in to initialize plugin structure.
   * @arg1: input js file
   * @arg2: format type (asciidoc, pdf, html)
   * @arg3: output filename
   */
  virtual void initialize(const QString& scriptPath, const QString& format,
                          const QString& outputPath, const QVariantMap& args) = 0;

  /**
   * Stats are passed in, and based on the initialization, are substituted into the template
   * @arg1: stats list of lists
   * @arg2: list with names of input files (e.g.,input1.osm, input2.osm)
   */
  virtual QString compose(QList< QList<SingleStat> >& stats, const QStringList& names) = 0;

};

} // end namespace

#endif // STATSCOMPOSER_H
