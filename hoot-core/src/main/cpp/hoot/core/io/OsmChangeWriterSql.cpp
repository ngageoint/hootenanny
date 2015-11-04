#include "OsmChangeWriterSql.h"

// hoot
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/OsmUtils.h>

// Qt
#include <QFile>
#include <QXmlStreamWriter>

namespace hoot
{

OsmChangeWriterSql::OsmChangeWriterSql()
{
  ;
}

void OsmChangeWriterSql::write(const QString& path, const ChangeSetProviderPtr cs)
{
  _outputSql.setFileName(path);
  if (_outputSql.open(QIODevice::WriteOnly | QIODevice::Text) == false)
  {
    throw HootException(QObject::tr("Error opening %1 for writing").arg(path));
  }


}

void OsmChangeWriterSql::_updateExistingElement(const ConstElementPtr updatedElement)
{
  //_modify(updatedElement);
}

void OsmChangeWriterSql::_deleteExistingElement(const ConstElementPtr removedElement)
{
  throw NotImplementedException("Deleting existing element not supported");
}

void OsmChangeWriterSql::_modify(const ConstWayPtr way)
{
  // Modify contents of way
}

void OsmChangeWriterSql::_modify(const ConstRelationPtr relation)
{
  throw NotImplementedException("Modifying relations not yet supported");
}

long OsmChangeWriterSql::_getLatestVersion(const ConstElementPtr element)
{
  throw NotImplementedException("Getting latest version not implemented");
  return -1;
}

}
