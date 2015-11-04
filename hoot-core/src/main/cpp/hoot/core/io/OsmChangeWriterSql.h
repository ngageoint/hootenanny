#ifndef OSMCHANGEWRITERSQL_H
#define OSMCHANGEWRITERSQL_H

#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <QString>
#include <QFile>
#include <boost/shared_ptr.hpp>
#include <hoot/core/elements/Element.h>

namespace hoot
{

class OsmChangeWriterSql
{
public:
  OsmChangeWriterSql();

  void write(const QString& path, const ChangeSetProviderPtr cs);


private:
  QFile _outputSql;

  void _writeNewElement(const ConstElementPtr newElement);
  void _updateExistingElement(const ConstElementPtr updatedElement);
  void _deleteExistingElement(const ConstElementPtr removedElement);

};

}

#endif // OSMCHANGEWRITERSQL_H
