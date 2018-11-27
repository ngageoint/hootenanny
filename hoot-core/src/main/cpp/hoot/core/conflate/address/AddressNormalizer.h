
#ifndef ADDRESS_NORMALIZER_H
#define ADDRESS_NORMALIZER_H

// Hoot
#include <hoot/core/elements/Element.h>

// Qt
#include <QSet>

namespace hoot
{

/**
 *
 */
class AddressNormalizer
{
public:

  AddressNormalizer();

  /**
   * @brief normalizeAddresses
   * @param e
   */
  void normalizeAddresses(const ElementPtr& e);

  /**
   * normalize also translates
   *
   * @param address
   * @return
   */
  QSet<QString> normalizeAddress(const QString address) const;

private:

  static bool _isValidNormalizedAddress(const QString inputAddress,
                                        const QString normalizedAddress);
};

}

#endif // ADDRESS_PARSER_H
