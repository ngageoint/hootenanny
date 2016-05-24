#include "MedianNodeVisitor.h"

// Standard
#include <numeric>

namespace hoot
{

MedianNodeVisitor::MedianNodeVisitor()
{
}

ConstNodePtr MedianNodeVisitor::calculateMedianNode() const
{
  ConstNodePtr best;
  double bestScore = numeric_limits<double>::max();

  for (int i = 0; i < _nodes.size(); ++i)
  {
    double score = _calculateRmsd(_nodes[i]);
    if (score < bestScore)
    {
      bestScore = score;
      best = _nodes[i];
    }
  }

  return best;
}

double MedianNodeVisitor::_calculateRmsd(ConstNodePtr n) const
{
  double sum = 0.0;

  Coordinate c1 = n->toCoordinate();
  for (int i = 0; i < _nodes.size(); ++i)
  {
    double d = c1.distance(_nodes[i]->toCoordinate());
    sum += d;
  }

  return sqrt(sum / _nodes.size());
}

void MedianNodeVisitor::visit(const ConstElementPtr& e)
{
  if (e->getElementType() == ElementType::Node)
  {
    ConstNodePtr n = dynamic_pointer_cast<const Node>(e);
    _nodes.append(n);
  }
}

}
