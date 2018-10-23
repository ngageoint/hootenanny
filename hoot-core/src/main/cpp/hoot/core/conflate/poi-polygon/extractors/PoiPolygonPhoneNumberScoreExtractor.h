
#ifndef POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H
#define POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H

// hoot
#include <hoot/core/conflate/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/PhoneNumberParser.h>

// Qt
#include <QList>

namespace hoot
{

class OsmMap;
class Element;

/**
 * Scores phone number similarity between features.
 */
class PoiPolygonPhoneNumberScoreExtractor : public FeatureExtractorBase, public Configurable
{
public:

  static std::string className() { return "hoot::PoiPolygonPhoneNumberScoreExtractor"; }

  PoiPolygonPhoneNumberScoreExtractor();

  virtual std::string getClassName() const
  { return PoiPolygonPhoneNumberScoreExtractor::className(); }

  virtual double extract(const OsmMap& map, const ConstElementPtr& poi,
                         const ConstElementPtr& poly) const;

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "Scores phone number similarity for POI/Polygon conflation"; }

  long getPhoneNumbersProcessed() const { return _phoneNumberParser.getPhoneNumbersProcessed(); }
  bool getMatchAttemptMade() const { return _matchAttemptMade; }

private:

  friend class PoiPolygonPhoneNumberScoreExtractorTest;

  mutable long _phoneNumbersProcessed;
  mutable bool _matchAttemptMade;

  PhoneNumberParser _phoneNumberParser;
};

}

#endif // POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H
