
#include "AddressNormalizer.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/LibPostalInit.h>
#include <hoot/core/conflate/address/AddressTagKeys.h>

// libpostal
#include <libpostal/libpostal.h>

namespace hoot
{

AddressNormalizer::AddressNormalizer()
{
}

void AddressNormalizer::normalizeAddresses(const ElementPtr& e)
{
  const QSet<QString> addressTagKeys = AddressTagKeys::getInstance()->getAddressTagKeys(*e);
  LOG_VART(addressTagKeys);
  for (QSet<QString>::const_iterator addressTagKeyItr = addressTagKeys.begin();
       addressTagKeyItr != addressTagKeys.end(); ++addressTagKeyItr)
  {
    const QString addressTagKey = *addressTagKeyItr;
    // normalization may find multiple addresses; we'll arbitrarily take the first one and put the
    // rest in an alt field
    const QSet<QString> normalizedAddresses = normalizeAddress(e->getTags().get(addressTagKey));
    bool firstAddressParsed = false;
    QString altAddresses;
    for (QSet<QString>::const_iterator normalizedAddressItr = normalizedAddresses.begin();
         normalizedAddressItr != normalizedAddresses.end(); ++normalizedAddressItr)
    {
      const QString normalizedAddress = *normalizedAddressItr;
      if (!firstAddressParsed)
      {
        e->getTags().set(addressTagKey, normalizedAddress);
        LOG_TRACE(
          "Set normalized address: " << normalizedAddress << " for tag key: " << addressTagKey);
        firstAddressParsed = true;
      }
      else
      {
        altAddresses += normalizedAddress + ";";
      }
    }
    if (!altAddresses.isEmpty())
    {
      altAddresses.chop(1);
      e->getTags().set("alt_address", altAddresses);
      LOG_TRACE(
        "Set alt normalized address(es): " << altAddresses << " for tag key: " << addressTagKey);
    }
  }
}

QSet<QString> AddressNormalizer::normalizeAddress(const QString address) const
{
  // See note about init of this in AddressParser::parseAddresses.
  LibPostalInit::getInstance();

  QSet<QString> normalizedAddresses;

  size_t num_expansions;
  //TODO: specifying a language in the options is optional, but could we get better performance if
  //we did specify one when we know what it is (would have to check to see if it was supported
  //first, of course)?
  char** expansions =
    libpostal_expand_address(address.toUtf8().data(), libpostal_get_default_options(),
    &num_expansions);
  // add all the normalizations libpostal finds as possible addresses
  for (size_t i = 0; i < num_expansions; i++)
  {
    const QString normalizedAddress = QString::fromUtf8(expansions[i]);
    normalizedAddresses.insert(normalizedAddress);
    LOG_TRACE("Normalized address from: " << address << " to: " << normalizedAddress);
  }
  libpostal_expansion_array_destroy(expansions, num_expansions);

  return normalizedAddresses;
}

}
