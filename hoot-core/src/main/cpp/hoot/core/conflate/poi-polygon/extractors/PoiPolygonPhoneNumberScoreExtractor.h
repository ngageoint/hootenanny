
#ifndef POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H
#define POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H

// hoot
#include <hoot/core/conflate/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QList>

namespace hoot
{

class OsmMap;
class Element;

struct ElementPhoneNumber
{
  QString name;
  QString tagKey;
  QString tagValue;
};

/**
 * TODO: seeing this error message logged out of libphonenumber quite a bit:
 * UTF-8 buffer is not interchange-valid;c omes out of unicodetext.cc; need to suppress
 *
 * @todo look for numbers in note or description field with PhoneNumberMatcher::Find ??
 * This class is generic enough that it could be used outside of POI/Poly conflation, if needed.
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

  void setRegionCode(QString code);

  static long phoneNumbersProcesed;
  static bool matchAttemptMade;

private:

  QString _regionCode;

  QList<ElementPhoneNumber> _getPhoneNumbers(const ConstElementPtr& element) const;
};

}

#endif // POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H
