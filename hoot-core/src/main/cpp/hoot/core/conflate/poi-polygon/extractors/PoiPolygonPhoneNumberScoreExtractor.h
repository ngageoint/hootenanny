
#ifndef POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H
#define POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QSet>
#include <QList>

namespace hoot
{

struct ElementPhoneNumber
{
  QString name;
  QString tagKey;
  QString tagValue;
};


/**
 * TODO: seeing this error message logged out of libphonenumber quite a bit:
 *
 * UTF-8 buffer is not interchange-valid
 *
 * comes out of unicodetext.cc; need to suppress
 *
 * @todo look for numbers in note or description field with PhoneNumberMatcher::Find ??
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
  { return "TODO"; }

  void setRegionCode(QString code) { _regionCode = code; }

  static long numPhoneNumbersFound;
  static bool matchAttemptMade;

private:

  QString _regionCode;

  QList<ElementPhoneNumber> _getPhoneNumbers(const ConstElementPtr& element) const;
};

}

#endif // POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H
