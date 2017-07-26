#ifndef SEARCHBOUNDSCALCULATOR_H
#define SEARCHBOUNDSCALCULATOR_H

// geos
#include <geos/geom/Envelope.h>

// hoot
#include <hoot/core/conflate/SearchRadiusProvider.h>
#include <hoot/core/elements/Node.h>

namespace hoot
{

/**
 * Given a POI in WGS84, calculate the search bounds in decimal degrees.
 *
 * Bad things may happen at the anti-meridian and poles.
 */
class SearchBoundsCalculator
{
public:
  SearchBoundsCalculator(SearchRadiusProviderPtr radiusProvider);

  geos::geom::Envelope calculateSearchBounds(const ConstOsmMapPtr& map, const ConstNodePtr& n)
    const;

private:
  SearchRadiusProviderPtr _radiusProvider;
};

typedef boost::shared_ptr<SearchBoundsCalculator> SearchBoundsCalculatorPtr;

}

#endif // SEARCHBOUNDSCALCULATOR_H
