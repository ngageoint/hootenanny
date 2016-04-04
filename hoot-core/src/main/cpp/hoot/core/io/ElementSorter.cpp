#include "ElementSorter.h"

// Standard
#include <algorithm>

namespace hoot
{

ElementSorter::ElementSorter(ConstOsmMapPtr source) :
  _nodeIndex(0),
  _wayIndex(0),
  _relationIndex(0)
{
  _source = source;

  for (NodeMap::const_iterator it = _source->getNodeMap().begin();
    it != _source->getNodeMap().end(); ++it)
  {
    _nodeIds.push_back(it->first);
  }
  for (WayMap::const_iterator it = _source->getWays().begin();
    it != _source->getWays().end(); ++it)
  {
    _wayIds.push_back(it->first);
  }
  for (RelationMap::const_iterator it = _source->getRelationMap().begin();
    it != _source->getRelationMap().end(); ++it)
  {
    _relationIds.push_back(it->first);
  }

  sort(_nodeIds.begin(), _nodeIds.end());
  sort(_wayIds.begin(), _wayIds.end());
  sort(_relationIds.begin(), _relationIds.end());
}

boost::shared_ptr<OGRSpatialReference> ElementSorter::getProjection() const
{
  return _source->getProjection();
}

bool ElementSorter::hasMoreElements()
{
  return _nodeIndex != _nodeIds.size() ||
      _wayIndex != _wayIds.size() ||
      _relationIndex != _relationIds.size();
}

ElementPtr ElementSorter::readNextElement()
{
  ElementPtr result;
  ConstElementPtr cr;

  if (_nodeIndex != _nodeIds.size())
  {
    cr = _source->getNode(_nodeIds[_nodeIndex++]);
  }
  else if (_wayIndex != _wayIds.size())
  {
    cr = _source->getWay(_wayIds[_wayIndex++]);
  }
  else if (_relationIndex != _relationIds.size())
  {
    cr = _source->getRelation(_relationIds[_relationIndex++]);
  }

  if (cr.get())
  {
    result.reset(cr->clone());
  }

  return result;
}

}
