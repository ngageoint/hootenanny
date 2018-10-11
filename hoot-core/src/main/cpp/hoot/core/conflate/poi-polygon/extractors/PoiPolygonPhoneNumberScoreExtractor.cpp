
#include "PoiPolygonPhoneNumberScoreExtractor.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

// libphonenumber
#include <phonenumbers/phonenumberutil.h>
using namespace i18n::phonenumbers;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonPhoneNumberScoreExtractor)

PoiPolygonPhoneNumberScoreExtractor::PoiPolygonPhoneNumberScoreExtractor() :
_regionCode("")
{
}

PoiPolygonPhoneNumberScoreExtractor::~PoiPolygonPhoneNumberScoreExtractor()
{
}

void PoiPolygonPhoneNumberScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  _regionCode = config.getPoiPolygonPhoneNumberRegion();
}

QSet<QString> PoiPolygonPhoneNumberScoreExtractor::_getPhoneNumbers(
  const ConstElementPtr& element) const
{
  //phone=* is the standard OSM tag, but have seen many others over time...keeping the allowed tags
  //loose for now.
  QSet<QString> parsedPhoneNums;
  for (Tags::const_iterator it = element->getTags().constBegin();
       it != element->getTags().constEnd(); ++it)
  {
    const QString tagKey = it.key();
    LOG_VART(tagKey);
    if (tagKey.contains("phone", Qt::CaseInsensitive))
    {
      const QString tagValue = it.value();
      LOG_VART(tagValue);
      if (_regionCode.isEmpty() ||
          PhoneNumberUtil::GetInstance()->IsPossibleNumberForString(
            tagValue.toStdString(), _regionCode.toStdString()))
      {
        parsedPhoneNums.insert(tagValue);
      }
    }
  }
  return parsedPhoneNums;
}

double PoiPolygonPhoneNumberScoreExtractor::extract(const OsmMap& /*map*/,
                                                    const ConstElementPtr& poi,
                                                    const ConstElementPtr& poly) const
{

  const QSet<QString> poiPhoneNumbers = _getPhoneNumbers(poi);
  const QSet<QString> polyPhoneNumbers = _getPhoneNumbers(poly);
  for (QSet<QString>::const_iterator poiPhoneNumberItr = poiPhoneNumbers.constBegin();
       poiPhoneNumberItr != poiPhoneNumbers.constEnd(); ++poiPhoneNumberItr)
  {
    const QString poiNumber = *poiPhoneNumberItr;
    for (QSet<QString>::const_iterator polyPhoneNumberItr = polyPhoneNumbers.constBegin();
         polyPhoneNumberItr != polyPhoneNumbers.constEnd(); ++polyPhoneNumberItr)
    {
      const QString polyNumber = *polyPhoneNumberItr;
      PhoneNumberUtil::MatchType numberMatchType =
        PhoneNumberUtil::GetInstance()->IsNumberMatchWithTwoStrings(
          poiNumber.toStdString(), polyNumber.toStdString());
      LOG_VART(numberMatchType);
      if (numberMatchType > PhoneNumberUtil::MatchType::NO_MATCH)
      {
        return 1.0;
      }
    }
  }

  return 0.0;
}

}
