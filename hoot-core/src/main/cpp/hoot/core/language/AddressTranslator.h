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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ADDRESS_TRANSLATOR_H
#define ADDRESS_TRANSLATOR_H

// hoot
#include <hoot/core/language/ToEnglishTranslator.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Translates addresses to English
 */
class AddressTranslator : public Configurable
{
public:

  static std::string className() { return "hoot::AddressTranslator"; }

  AddressTranslator();

  virtual void setConfiguration(const Settings& conf);

  /**
   * Translates an address to English
   *
   * @param address a complete address string
   */
  QString translateToEnglish(const QString address) const;

private:

  friend class PoiPolygonAddressScoreExtractorTest;

  // See comments in PoiPolygonTypeScoreExtractor as to why this is static.
  static boost::shared_ptr<ToEnglishTranslator> _translator;
};

}

#endif // ADDRESS_TRANSLATOR_H
