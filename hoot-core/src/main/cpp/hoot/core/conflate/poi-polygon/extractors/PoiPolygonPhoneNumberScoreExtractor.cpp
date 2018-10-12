
#include "PoiPolygonPhoneNumberScoreExtractor.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/OsmMap.h>
#include "PoiPolygonNameScoreExtractor.h"

// libphonenumber
#include <phonenumbers/phonenumberutil.h>
using namespace i18n::phonenumbers;

namespace hoot
{

long PoiPolygonPhoneNumberScoreExtractor::phoneNumbersProcesed = 0;
bool PoiPolygonPhoneNumberScoreExtractor::matchAttemptMade = false;

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonPhoneNumberScoreExtractor)

PoiPolygonPhoneNumberScoreExtractor::PoiPolygonPhoneNumberScoreExtractor() :
_regionCode("")
{
}

void PoiPolygonPhoneNumberScoreExtractor::setRegionCode(QString code)
{
  if (!code.isEmpty())
  {
    std::set<std::string> regions;
      PhoneNumberUtil::GetInstance()->GetSupportedRegions(&regions);
    std::set<std::string>::const_iterator it = regions.find(code.toStdString());
    if (it == regions.end())
    {
      throw HootException("Invalid phone number region code: " + code);
    }
  }
  _regionCode = code;
}

void PoiPolygonPhoneNumberScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setRegionCode(config.getPoiPolygonPhoneNumberRegionCode().toUpper());
  _additionalTagKeys = config.getPoiPolygonPhoneNumberAdditionalTagKeys();
}

QList<ElementPhoneNumber> PoiPolygonPhoneNumberScoreExtractor::_getPhoneNumbers(
  const ConstElementPtr& element) const
{
  //phone=* is the standard OSM tag, but have seen many others over time...keeping the allowed tags
  //fairly loose for now.
  QList<ElementPhoneNumber> parsedPhoneNums;
  for (Tags::const_iterator it = element->getTags().constBegin();
       it != element->getTags().constEnd(); ++it)
  {
    const QString tagKey = it.key();
    LOG_VART(tagKey);
    if (_additionalTagKeys.contains(tagKey, Qt::CaseInsensitive) ||
        tagKey.contains("phone", Qt::CaseInsensitive))
    {
      const QString tagValue = it.value().toUtf8().trimmed().simplified();
      LOG_VART(tagValue);
      // If we're not using libphonenumber with a region code to see if the number is valid, then
      // check for at least one digit (vanity numbers can have letters).
      if ((_regionCode.isEmpty() && StringUtils::hasDigit(tagValue)) ||
          // IsPossibleNumber is a fairly quick check (IsValidNumber is more strict and a little
          // more expensive)
          PhoneNumberUtil::GetInstance()->IsPossibleNumberForString(
            tagValue.toStdString(), _regionCode.toStdString()))
      {
        phoneNumbersProcesed++;

        LOG_TRACE("Possibly a phone number: " << tagKey << "=" << tagValue);
        ElementPhoneNumber elementPhoneNumber;
        elementPhoneNumber.name = PoiPolygonNameScoreExtractor::getElementName(element);
        elementPhoneNumber.tagKey = tagKey;
        elementPhoneNumber.tagValue = tagValue;
        parsedPhoneNums.append(elementPhoneNumber);
      }
      else
      {
        LOG_TRACE("Not a phone number: " << tagKey << "=" << tagValue);
      }
    }
  }
  return parsedPhoneNums;
}

double PoiPolygonPhoneNumberScoreExtractor::extract(const OsmMap& /*map*/,
                                                    const ConstElementPtr& poi,
                                                    const ConstElementPtr& poly) const
{

  phoneNumbersProcesed = 0;
  matchAttemptMade = false;

  const QList<ElementPhoneNumber> poiPhoneNumbers = _getPhoneNumbers(poi);
  if (poiPhoneNumbers.size() > 0)
  {
    LOG_VART(poiPhoneNumbers.size());
  }
  const QList<ElementPhoneNumber> polyPhoneNumbers = _getPhoneNumbers(poly);
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
      matchAttemptMade = true;  
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
