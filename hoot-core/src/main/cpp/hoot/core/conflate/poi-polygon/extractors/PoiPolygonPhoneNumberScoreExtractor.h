
#ifndef POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H
#define POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QSet>

namespace hoot
{

/**
 *
 */
class PoiPolygonPhoneNumberScoreExtractor : public FeatureExtractorBase, public Configurable
{
public:

  static std::string className() { return "hoot::PoiPolygonPhoneNumberScoreExtractor"; }

  PoiPolygonPhoneNumberScoreExtractor();
  ~PoiPolygonPhoneNumberScoreExtractor();

  virtual std::string getClassName() const
  { return PoiPolygonPhoneNumberScoreExtractor::className(); }

  virtual double extract(const OsmMap& map, const ConstElementPtr& poi,
                         const ConstElementPtr& poly) const;

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "TODO"; }

  void setRegionCode(QString code) { _regionCode = code; }

private:

  QString _regionCode;

  QSet<QString> _getPhoneNumbers(const ConstElementPtr& element) const;
};

}

#endif // POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H
