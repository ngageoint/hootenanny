/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "PoiPolygonPhoneNumberScoreExtractor.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

// libphonenumber
#include <phonenumbers/phonenumberutil.h>
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
  const QList<ElementPhoneNumber> poiPhoneNumbers = _phoneNumberParser.parsePhoneNumbers(*poi);
  if (!poiPhoneNumbers.empty())
  {
    LOG_VART(poiPhoneNumbers.size());
  }
  const QList<ElementPhoneNumber> polyPhoneNumbers = _phoneNumberParser.parsePhoneNumbers(*poly);
  if (!polyPhoneNumbers.empty())
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
