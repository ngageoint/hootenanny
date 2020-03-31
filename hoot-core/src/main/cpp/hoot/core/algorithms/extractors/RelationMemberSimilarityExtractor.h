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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef RELATION_MEMBER_SIMILARITY_EXTRACTOR_H
#define RELATION_MEMBER_SIMILARITY_EXTRACTOR_H

// hoot
#include <hoot/core/algorithms/extractors/FeatureExtractorBase.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ElementComparer.h>

namespace hoot
{

class RelationMemberComparison
{
public:

  RelationMemberComparison(ElementPtr element, const OsmMap& sourceMap) :
  _element(element)
  {
    _elementComparer.setIgnoreElementId(true);
    OsmMap* map = const_cast<OsmMap*>(&sourceMap);
    _elementComparer.setOsmMap(map);
  }

  ElementPtr getElement() const { return _element; }

  bool operator==(const RelationMemberComparison& memberComp) const
  {
    const bool equals = _elementComparer.isSame(this->_element, memberComp.getElement());
    if (equals)
    {
      LOG_TRACE(
        "Elements equal: " << this->_element->getElementId() << " and " <<
        memberComp.getElement()->getElementId())
    }
    return equals;
  }

  QString toString() const { return _element->getElementId().toString(); }

private:

  ElementPtr _element;
  ElementComparer _elementComparer;
};

inline uint qHash(const RelationMemberComparison& memberComp)
{
  QString elementStr;
  std::stringstream ss(std::stringstream::out);

  NodePtr node;
  WayPtr way;
  RelationPtr relation;
  std::vector<RelationData::Entry> members;
  ElementPtr element = memberComp.getElement();
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:

      node = std::dynamic_pointer_cast<Node>(element);
      ss << QString::number(node->getX(), 'f', ConfigOptions().getWriterPrecision())
         << " " << QString::number(node->getY(), 'f', ConfigOptions().getWriterPrecision());

      break;

    case ElementType::Way:

      way = std::dynamic_pointer_cast<Way>(element);
      // TODO: this may not be unique enough along with tags
      ss << way->getNodeIds().size();

      break;

    case ElementType::Relation:

      relation = std::dynamic_pointer_cast<Relation>(element);
      ss << relation->getType();
      members = relation->getMembers();
      for (size_t i = 0; i < members.size(); i++)
      {
        RelationData::Entry member = members[i];
        ss << " " << member.getRole() << " " << member.getElementId().getType() << ",";
      }

      break;

    default:
      throw IllegalArgumentException("Unexpected element type.");
  }
  ss << " " << element->getTags().toString();

  elementStr = QString::fromUtf8(ss.str().data());
  return qHash(elementStr);
}

/**
 * TODO
 *
 * https://en.wikipedia.org/wiki/Jaccard_index
 * https://www.statisticshowto.datasciencecentral.com/jaccard-index/
 */
class RelationMemberSimilarityExtractor : public FeatureExtractorBase
{
public:

  RelationMemberSimilarityExtractor();

  static std::string className() { return "hoot::RelationMemberSimilarityExtractor"; }

  virtual std::string getClassName() const
  { return RelationMemberSimilarityExtractor::className(); }

  virtual double extract(const OsmMap& map, const std::shared_ptr<const Element>& target,
    const std::shared_ptr<const Element>& candidate) const;

  virtual QString getDescription() const
  { return "TODO"; }

  void setIgnoreIds(bool ignore) { _ignoreIds = ignore; }

private:

  // TODO: make configurable
  bool _ignoreIds;
};

}

#endif // RELATION_MEMBER_SIMILARITY_EXTRACTOR_H
