#include "OsmNetworkExtractor.h"

#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

class OsmNetworkExtractorVisitor : public ElementVisitor
{
public:

  OsmNetworkExtractorVisitor(OsmNetworkExtractor& parent) : _parent(parent) {}

  virtual void visit(const ConstElementPtr& e)
  {
    _parent._visit(e);
  }

private:
  OsmNetworkExtractor& _parent;
};

OsmNetworkExtractor::OsmNetworkExtractor()
{
}

OsmNetworkPtr OsmNetworkExtractor::extractNetwork(ConstOsmMapPtr map)
{
  OsmNetworkPtr result(new OsmNetwork());

  // go through all the elements.
  OsmNetworkExtractorVisitor v(*this);
  map->visitRo(v);
}

bool OsmNetworkExtractor::_isValidElement(const ConstElementPtr& e)
{
  bool result = true;
  if (e->getElementType() == ElementType::Node)
  {
    result = false;
  }
  else if (e->getElementType() == ElementType::Relation)
  {
    ConstRelationPtr r = dynamic_pointer_cast<const Relation>(e);
    if (OsmSchema::getInstance().isLinear(*e) == false)
    {
      LOG_WARN("Received a non-linear relation as a valid network element. Ignoring relation. " <<
        e);
      result = false;
    }
    else
    {
      const vector<RelationData::Entry>& members = r->getMembers();
      for (size_t i = 0; i < members.size(); ++i)
      {
        if (members[i].getElementId().getType() != ElementType::Way)
        {
          LOG_WARN("Received a linear relation that contains a non-linear element: " << e);
        }
      }
    }
  }

  return result;
}

void OsmNetworkExtractor::_visit(const ConstElementPtr& e)
{
  if (_criterion->isSatisfied(e) && _isValidElement(e))
  {
    //_addEdge(e);
  }
}

}
