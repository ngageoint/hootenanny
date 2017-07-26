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

#include "../visitors/AddDebugTagsVisitor.h"

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
  AddDebugTagsVisitor().visit(copy);
  Envelope e = _bounds->calculateSearchBounds(OsmMapPtr(), copy);

  QString result = "{";

  result += "\"minx\":" + QString::number(e.getMinX(), 'g', 16);
  result += ",\"miny\":" + QString::number(e.getMinY(), 'g', 16);
  result += ",\"maxx\":" + QString::number(e.getMaxX(), 'g', 16);
  result += ",\"maxy\":" + QString::number(e.getMaxY(), 'g', 16);
  result += ",\"element\":{\"type\":\"node\"";
  result += ",\"id\":" + QString::number(copy->getId(), 'g', 16);
  result += ",\"lat\":" + QString::number(copy->getY(), 'g', 16);
  result += ",\"lon\":" + QString::number(copy->getX(), 'g', 16);
  result += ",\"tags\":{";

  bool first = true;
  const Tags& tags = copy->getTags();
  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    if (!first)
    {
      result += ",";
    }
    result += OsmJsonWriter::markupString(it.key()) + ":" + OsmJsonWriter::markupString(it.value());
    first = false;
  }

  result += "}}}\n";

  if (_fp->write(result.toUtf8()) == -1)
  {
    throw HootException("Error writing to file: " + _fp->errorString());
  }
}

}
