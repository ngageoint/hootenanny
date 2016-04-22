#ifndef OSMCHANGESETXMLFILEWRITER_H
#define OSMCHANGESETXMLFILEWRITER_H

#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>

// Qt
class QXmlStreamWriter;

namespace hoot
{

/**
 * Writes an OSM changeset to an XML file.
 *
 * Before writing this changeset to an OSM API database, the corresponding changeset
 * would have to be created using the API.  Optionally, after writing this the changeset
 * can be closed via the API.
 */
class OsmChangesetXmlFileWriter
{

public:

  OsmChangesetXmlFileWriter();

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

#endif // OSMCHANGESETXMLFILEWRITER_H
