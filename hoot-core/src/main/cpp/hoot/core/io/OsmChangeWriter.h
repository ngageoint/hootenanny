#ifndef OSMCHANGEWRITER_H
#define OSMCHANGEWRITER_H

#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/ElementType.h>

// Qt
class QXmlStreamWriter;

namespace hoot
{

class OsmChangeWriter
{

public:

  OsmChangeWriter();

  QString writeToString(ChangeSetProviderPtr cs);

  void write(QString path, ChangeSetProviderPtr cs);

  void write(QIODevice& d, ChangeSetProviderPtr cs);

  void writeNode(QXmlStreamWriter& writer, ConstNodePtr n);
  void writeWay(QXmlStreamWriter& writer, ConstWayPtr w);
  void writeRelation(QXmlStreamWriter& writer, ConstRelationPtr n);

private:

  int _precision;

};

}

#endif // OSMCHANGEWRITER_H
