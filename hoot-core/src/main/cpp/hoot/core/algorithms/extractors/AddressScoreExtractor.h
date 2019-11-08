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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ADDRESS_SCORE_EXTRACTOR_H
#define ADDRESS_SCORE_EXTRACTOR_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/string/ExactStringDistance.h>
#include <hoot/core/conflate/address/AddressParser.h>

namespace hoot
{

class Address;

/**
 * Calculates the address similarity score of two features involved in certain types of conflation.
 * Only exact string matches yield a positive score (no partial matching...for now).
 *
 * Some effort was spent in validating addresses with libaddressinput
 * (https://github.com/googlei18n/libaddressinput).  It was found that yields no utility since the
 * features we are comparing are geographically close, don't need higher level address info (state,
 * city, etc.), and we basically assume feature addresses are valid anyway...we're just trying to
 * match them to each other.
 */
class AddressScoreExtractor : public FeatureExtractorBase, public Configurable
{
public:

  static std::string className() { return "hoot::AddressScoreExtractor"; }

  AddressScoreExtractor();

  virtual std::string getClassName() const { return AddressScoreExtractor::className(); }

  virtual void setConfiguration(const Settings& conf);

  /**
   * Calculates the address similarity score of two features
   *
   * @param map map containing the elements whose score is to be determined
   * @param element1 the first element whose address is to be compared
   * @param element2 the second element whose address is to be compared
   * @return the address score
   */
  virtual double extract(const OsmMap& map, const ConstElementPtr& element1,
                         const ConstElementPtr& element2) const;

  virtual QString getDescription() const
  { return "Scores address similarity for conflation"; }

  long getAddressesProcessed() const { return _addressesProcessed; }
  bool getMatchAttemptMade() const { return _matchAttemptMade; }
  void setAllowLenientHouseNumberMatching(bool allow)
  { _addressParser.setAllowLenientHouseNumberMatching(allow); }
  bool isConfigured() const { return _configured; }
  void setReturnNegativeScoreWhenEitherInputHasNoAddress(bool returnNegative)
  { _returnNegativeScoreWhenEitherInputHasNoAddress = returnNegative; }

private:

  friend class AddressScoreExtractorTest;

  mutable long _addressesProcessed;
  mutable bool _matchAttemptMade;

  AddressParser _addressParser;

  bool _configured;
  bool _returnNegativeScoreWhenEitherInputHasNoAddress;

  /*
   * Searches for an address on element. Will look also look for address way nodes and address
   * node relation members. elementBeingComparedWith is passed in so we don't count an address
   * more than once if element happens to contain elementBeingComparedWith.
   */
  QList<Address> _getElementAddresses(const OsmMap& map, const ConstElementPtr& element,
                                      const ConstElementPtr& elementBeingComparedWith) const;
};

}

#endif // ADDRESS_SCORE_EXTRACTOR_H
