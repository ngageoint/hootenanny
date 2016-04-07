#include "OsmChangeWriter.h"

// hoot
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/OsmUtils.h>

// Qt
#include <QFile>
#include <QXmlStreamWriter>

namespace hoot
{

OsmChangeWriter::OsmChangeWriter()
{
  _precision = ConfigOptions().getWriterPrecision();
}

void OsmChangeWriter::write(QString path, ChangeSetProviderPtr cs)
{
  LOG_INFO("Writing changeset to " << path);

  QFile f;
  f.setFileName(path);
  if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw HootException(QObject::tr("Error opening %1 for writing").arg(path));
  }

  write(f, cs);
}

void OsmChangeWriter::write(QIODevice &d, ChangeSetProviderPtr cs)
{
  QXmlStreamWriter writer(&d);
  writer.setCodec("UTF-8");
  writer.setAutoFormatting(true);
  writer.writeStartDocument();

  writer.writeStartElement("osmChange");
  writer.writeAttribute("version", "0.6");
  writer.writeAttribute("generator", "hootenanny");

  //Make the changeset have only one set of entries per change type.  Also for readability,
  //sort the set of changes within each change type by element type.
  QMap<Change::ChangeType, QMap<ElementType::Type, QVector<ElementPtr> > > elementsByChangesetTypeAndElementType;
  while (cs->hasMoreChanges())
  {
    const Change change = cs->readNextChange();
    elementsByChangesetTypeAndElementType[change.type][change.e->getElementType().getEnum()].push_back(change.e);
  }
  const int NUM_CHANGE_TYPES = 3;
  const int NUM_ELEMENT_TYPES = 3;
  for (int i = 0; i < NUM_CHANGE_TYPES; i++)
  {
    const Change::ChangeType changeType = static_cast<Change::ChangeType>(i);
    writer.writeStartElement(Change::changeTypeToString(changeType).toLower());
    for (int j = 0; j < NUM_ELEMENT_TYPES; j++)
    {
      const QVector<ElementPtr> elements =
        elementsByChangesetTypeAndElementType[changeType][ElementType::Type(static_cast<ElementType::Type>(j))];
      QVectorIterator<ElementPtr> elementItr(elements);
      while (elementItr.hasNext())
      {
        ElementPtr element = elementItr.next();
        switch (element->getElementType().getEnum())
        {
          case ElementType::Node:
            writeNode(writer, dynamic_pointer_cast<const Node>(element));
            break;

          case ElementType::Way:
            writeWay(writer, dynamic_pointer_cast<const Way>(element));
            break;

          case ElementType::Relation:
            writeRelation(writer, dynamic_pointer_cast<const Relation>(element));
            break;

          default:
            throw IllegalArgumentException("Unexpected element type.");
        }
      }
    }
    writer.writeEndElement();
  }

  writer.writeEndDocument();
}

void OsmChangeWriter::writeNode(QXmlStreamWriter& writer, ConstNodePtr n)
{
  writer.writeStartElement("node");
  writer.writeAttribute("id", QString::number(n->getId()));
  if (n->getTimestamp() != 0)
  {
    writer.writeAttribute("timestamp", OsmUtils::toTimeString(n->getTimestamp()));
  }
  writer.writeAttribute("version", QString::number(n->getVersion()));

  writer.writeAttribute("lat", QString::number(n->getY(), 'f', _precision));
  writer.writeAttribute("lon", QString::number(n->getX(), 'f', _precision));

  const Tags& tags = n->getTags();
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); it++)
  {
    if (it.key().isEmpty() == false && it.value().isEmpty() == false)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", OsmWriter().removeInvalidCharacters(it.key()));
      writer.writeAttribute("v", OsmWriter().removeInvalidCharacters(it.value()));
      writer.writeEndElement();
    }
  }

  // turn this on when we start using node circularError.
  if (n->hasCircularError() &&
      n->getTags().getNonDebugCount() > 0)
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", "error:circular");
    writer.writeAttribute("v", QString("%1").arg(n->getCircularError()));
    writer.writeEndElement();
  }

  writer.writeEndElement();
}

void OsmChangeWriter::writeWay(QXmlStreamWriter& writer, ConstWayPtr w)
{
  writer.writeStartElement("way");
  writer.writeAttribute("id", QString::number(w->getId()));
  if (w->getTimestamp() != 0)
  {
    writer.writeAttribute("timestamp", OsmUtils::toTimeString(w->getTimestamp()));
  }
  writer.writeAttribute("version", QString::number(w->getVersion()));

  for (size_t j = 0; j < w->getNodeCount(); j++)
  {
    writer.writeStartElement("nd");
    writer.writeAttribute("ref", QString::number(w->getNodeId(j)));
    writer.writeEndElement();
  }

  const Tags& tags = w->getTags();
  for (Tags::const_iterator tit = tags.constBegin(); tit != tags.constEnd(); ++tit)
  {
    if (tit.key().isEmpty() == false && tit.value().isEmpty() == false)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", OsmWriter().removeInvalidCharacters(tit.key()));
      writer.writeAttribute("v", OsmWriter().removeInvalidCharacters(tit.value()));
      writer.writeEndElement();
    }
  }

  if (w->hasCircularError())
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", "error:circular");
    writer.writeAttribute("v", QString("%1").arg(w->getCircularError()));
    writer.writeEndElement();
  }

  writer.writeEndElement();
}

void OsmChangeWriter::writeRelation(QXmlStreamWriter& writer, ConstRelationPtr r)
{
  writer.writeStartElement("relation");
  writer.writeAttribute("id", QString::number(r->getId()));
  if (r->getTimestamp() != 0)
  {
    writer.writeAttribute("timestamp", OsmUtils::toTimeString(r->getTimestamp()));
  }
  writer.writeAttribute("version", QString::number(r->getVersion()));

  const vector<RelationData::Entry>& members = r->getMembers();
  for (size_t j = 0; j < members.size(); j++)
  {
    const RelationData::Entry& e = members[j];
    writer.writeStartElement("member");
    writer.writeAttribute("type", e.getElementId().getType().toString().toLower());
    writer.writeAttribute("ref", QString::number(e.getElementId().getId()));
    writer.writeAttribute("role", OsmWriter().removeInvalidCharacters(e.role));
    writer.writeEndElement();
  }

  const Tags& tags = r->getTags();
  for (Tags::const_iterator tit = tags.constBegin(); tit != tags.constEnd(); ++tit)
  {
    if (tit.key().isEmpty() == false && tit.value().isEmpty() == false)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", OsmWriter().removeInvalidCharacters(tit.key()));
      writer.writeAttribute("v", OsmWriter().removeInvalidCharacters(tit.value()));
      writer.writeEndElement();
    }
  }

  if (r->getType() != "")
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", "type");
    writer.writeAttribute("v", OsmWriter().removeInvalidCharacters(r->getType()));
    writer.writeEndElement();
  }

  if (r->hasCircularError())
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", "error:circular");
    writer.writeAttribute("v", QString("%1").arg(r->getCircularError()));
    writer.writeEndElement();
  }

  writer.writeEndElement();
}

}
