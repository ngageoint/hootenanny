
#ifndef ADDRESS_NORMALIZER_H
#define ADDRESS_NORMALIZER_H

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
   * normalize also translates
   *
   * @param address
   * @return
   */
  QSet<QString> normalizeAddress(const QString address) const;
};

}

#endif // ADDRESS_PARSER_H
