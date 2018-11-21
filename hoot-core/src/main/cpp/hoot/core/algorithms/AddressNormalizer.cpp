
#include "AddressNormalizer.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/LibPostalInit.h>

// libpostal
#include <libpostal/libpostal.h>

namespace hoot
{

AddressNormalizer::AddressNormalizer()
{
}

QSet<QString> AddressNormalizer::normalizeAddress(const QString address) const
{
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
