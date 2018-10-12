
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
#include <phonenumbers/phonenumbermatcher.h>
#include <phonenumbers/phonenumbermatch.h>
using namespace i18n::phonenumbers;

namespace hoot
{

long PoiPolygonPhoneNumberScoreExtractor::phoneNumbersProcesed = 0;
bool PoiPolygonPhoneNumberScoreExtractor::matchAttemptMade = false;

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonPhoneNumberScoreExtractor)

PoiPolygonPhoneNumberScoreExtractor::PoiPolygonPhoneNumberScoreExtractor() :
_regionCode(""),
_searchInText(false)
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

void PoiPolygonPhoneNumberScoreExtractor::setSearchInText(bool search)
{
  if (search && _regionCode.isEmpty())
  {
    throw HootException("A region code must be set when searching for phone numbers in text.");
  }
  _searchInText = search;
}

void PoiPolygonPhoneNumberScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setRegionCode(config.getPoiPolygonPhoneNumberRegionCode().trimmed().toUpper());
  setAdditionalTagKeys(config.getPoiPolygonPhoneNumberAdditionalTagKeys());
  setSearchInText(config.getPoiPolygonPhoneNumberSearchInText());
}

void PoiPolygonPhoneNumberScoreExtractor::_addPhoneNumber(const QString name, const QString tagKey,
                                                          const QString tagValue,
                                                      QList<ElementPhoneNumber>& phoneNumbers) const
{
  LOG_TRACE("Possibly a phone number: " << tagKey << "=" << tagValue);
  ElementPhoneNumber elementPhoneNumber;
  elementPhoneNumber.name = name;
  elementPhoneNumber.tagKey = tagKey;
  elementPhoneNumber.tagValue = tagValue;
  phoneNumbers.append(elementPhoneNumber);
  phoneNumbersProcesed++;
}

QList<ElementPhoneNumber> PoiPolygonPhoneNumberScoreExtractor::_getPhoneNumbers(
  const ConstElementPtr& element) const
{
  //phone=* is the standard OSM tag, but have seen many others over time...keeping the allowed tags
  //fairly loose for now
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

      if (_regionCode.isEmpty())
      {
        // If we're not using a region code to see if the number is valid, then just check for at
        // least one digit (vanity numbers can have letters).
        if (StringUtils::hasDigit(tagValue))
        {
          _addPhoneNumber(
            PoiPolygonNameScoreExtractor::getElementName(element), tagKey, tagValue,
            parsedPhoneNums);
        }
      }
      else
      {
        if (!_searchInText)
        {
          // IsPossibleNumber is a fairly quick check (IsValidNumber is more strict and a little
          // more expensive)
          if (PhoneNumberUtil::GetInstance()->IsPossibleNumberForString(
                tagValue.toStdString(), _regionCode.toStdString()))
          {
            _addPhoneNumber(
              PoiPolygonNameScoreExtractor::getElementName(element), tagKey, tagValue,
              parsedPhoneNums);
          }
        }
        else
        {
          // This lets us search through text that may have other things in it besides phone
          // numbers.
          PhoneNumberMatcher numberFinder(
            *PhoneNumberUtil::GetInstance(),
            tagValue.toStdString(),
            _regionCode.toStdString(),
            PhoneNumberMatcher::Leniency::POSSIBLE,
            //not sure what a good number is for max tries yet or what that number actually even
            //means
            1);
          int parserFinds = 0;
          while (numberFinder.HasNext())
          {
            PhoneNumberMatch match;
            numberFinder.Next(&match);
            const QString parsedNum = QString::fromStdString(match.raw_string());
            _addPhoneNumber(
              PoiPolygonNameScoreExtractor::getElementName(element), tagKey, parsedNum,
              parsedPhoneNums);
            parserFinds++;
          }
          LOG_TRACE("Number finder found " << parserFinds << " numbers.");
        }
      }

      if (parsedPhoneNums.size() == 0)
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
