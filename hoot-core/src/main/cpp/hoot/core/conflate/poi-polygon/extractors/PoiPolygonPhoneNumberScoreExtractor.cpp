
#include "PoiPolygonPhoneNumberScoreExtractor.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/OsmMap.h>

// libphonenumber
#include <phonenumbers/phonenumberutil.h>
#include <phonenumbers/phonenumbermatcher.h>
#include <phonenumbers/phonenumbermatch.h>
using namespace i18n::phonenumbers;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonPhoneNumberScoreExtractor)

PoiPolygonPhoneNumberScoreExtractor::PoiPolygonPhoneNumberScoreExtractor() :
_phoneNumbersProcessed(0),
_matchAttemptMade(false)
{
}

void PoiPolygonPhoneNumberScoreExtractor::setConfiguration(const Settings& conf)
{
  _phoneNumberParser.setConfiguration(conf);
}

double PoiPolygonPhoneNumberScoreExtractor::extract(const OsmMap& /*map*/,
                                                    const ConstElementPtr& poi,
                                                    const ConstElementPtr& poly) const
{
  const QList<ElementPhoneNumber> poiPhoneNumbers = _phoneNumberParser.parsePhoneNumbers(poi);
  if (poiPhoneNumbers.size() > 0)
  {
    LOG_VART(poiPhoneNumbers.size());
  }
  const QList<ElementPhoneNumber> polyPhoneNumbers = _phoneNumberParser.parsePhoneNumbers(poly);
  if (polyPhoneNumbers.size() > 0)
  {
    LOG_VART(polyPhoneNumbers.size());
  }

  for (QList<ElementPhoneNumber>::const_iterator poiPhoneNumberItr = poiPhoneNumbers.constBegin();
       poiPhoneNumberItr != poiPhoneNumbers.constEnd(); ++poiPhoneNumberItr)
  {
    const ElementPhoneNumber poiNumber = *poiPhoneNumberItr;
    for (QList<ElementPhoneNumber>::const_iterator polyPhoneNumberItr = polyPhoneNumbers.constBegin();
         polyPhoneNumberItr != polyPhoneNumbers.constEnd(); ++polyPhoneNumberItr)
    {
      _matchAttemptMade = true;
      const ElementPhoneNumber polyNumber = *polyPhoneNumberItr;

      PhoneNumberUtil::MatchType numberMatchType =
        PhoneNumberUtil::GetInstance()->IsNumberMatchWithTwoStrings(
          poiNumber.tagValue.toStdString(), polyNumber.tagValue.toStdString());
      LOG_VART(numberMatchType);
      if (numberMatchType > PhoneNumberUtil::MatchType::NO_MATCH)
      {
        LOG_TRACE(
          "Phone number match: " << poiNumber.tagKey.toStdString() << "=" <<
          poiNumber.tagValue.toStdString() << " <--> " << polyNumber.tagKey.toStdString() << "=" <<
          polyNumber.tagValue.toStdString() << "; POI name: " << poiNumber.name <<
          ", Poly name: " << polyNumber.name);
        return 1.0;
      }
      else
      {
        LOG_TRACE(
          "Phone number mismatch: " << poiNumber.tagKey.toStdString() << "=" <<
          poiNumber.tagValue.toStdString() << " <--> " << polyNumber.tagKey.toStdString() << "=" <<
          polyNumber.tagValue.toStdString() << "; POI name: " << poiNumber.name <<
          ", Poly name: " << polyNumber.name);
      }
    }
  }

  return 0.0;
}

}
