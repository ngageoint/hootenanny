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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SparkJsonWriter.h"

// geos
#include <geos/geom/Envelope.h>

using namespace geos::geom;

// hoot
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/CalculateHashVisitor.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, SparkJsonWriter)

using namespace boost;

SparkJsonWriter::SparkJsonWriter() :
  _precision(round(ConfigOptions().getWriterPrecision()))
{

}

void SparkJsonWriter::open(QString fileName)
{
  close();

  _fp.reset(new QFile());
  _fp->setFileName(fileName);
  if (!_fp->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw HootException(QObject::tr("Error opening %1 for writing").arg(fileName));
  }

  // find a match creator that can provide the search bounds.
  foreach (boost::shared_ptr<MatchCreator> mc, MatchFactory::getInstance().getCreators())
  {
    SearchRadiusProviderPtr sbc = dynamic_pointer_cast<SearchRadiusProvider>(mc);

    if (sbc.get())
    {
      if (_bounds.get())
      {
        LOG_WARN("Found more than one bounds calculator. Using the first one.");
      }
      else
      {
        _bounds.reset(new SearchBoundsCalculator(sbc));
      }
    }
  }

  if (!_bounds.get())
  {
    throw HootException("You must specify one match creator that supports search radius "
      "calculation.");
  }
}

void SparkJsonWriter::writePartial(const ConstNodePtr& n)
{
  NodePtr copy(dynamic_cast<Node*>(n->clone()));
  _addExportTagsVisitor.visit(copy);
  Envelope e = _bounds->calculateSearchBounds(OsmMapPtr(), copy);

  QString result;
  // 600 was picked b/c OSM POI records were generally ~500.
  result.reserve(600);
  result += "A\t";

  result += QString::number(e.getMinX(), 'g', 16) % "\t";
  result += QString::number(e.getMinY(), 'g', 16) % "\t";
  result += QString::number(e.getMaxX(), 'g', 16) % "\t";
  result += QString::number(e.getMaxY(), 'g', 16) % "\t";
  /// @todo Update after https://github.com/ngageoint/hootenanny/issues/1663
  result += CalculateHashVisitor::toHashString(n) % "\t";
  result += "{\"element\":{\"type\":\"node\"";
  result += ",\"id\":" % QString::number(copy->getId(), 'g', 16);
  result += ",\"lat\":" % QString::number(copy->getY(), 'g', 16);
  result += ",\"lon\":" % QString::number(copy->getX(), 'g', 16);
  result += ",\"tags\":{";

  bool first = true;
  const Tags& tags = copy->getTags();
  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    if (!first)
    {
      result += ",";
    }
    result += OsmJsonWriter::markupString(it.key()) % ":" % OsmJsonWriter::markupString(it.value());
    first = false;
  }

  result += "}}}\n";

  if (_fp->write(result.toUtf8()) == -1)
  {
    throw HootException("Error writing to file: " + _fp->errorString());
  }
}

}
