

#ifndef HOOT_SERVICES_TRANSLATION_INFO_RESPONSE_PARSER_H
#define HOOT_SERVICES_TRANSLATION_INFO_RESPONSE_PARSER_H

// Boost
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

// Qt
#include <QString>

namespace hoot
{

/**
 * Prints response info from the hoot translation service
 */
class HootServicesTranslationInfoResponseParser
{
public:

  /**
   * Returns an available languages response string
   *
   * @param type type of response to print; "translatable" or "detectable"
   * @param response the response to print
   * @return a display string
   */
  static QString parseAvailableLanguagesResponse(const QString type,
                                               boost::shared_ptr<boost::property_tree::ptree> response);

  /**
   * Returns an available language apps response string
   *
   * @param type type of response to print; "translators" or "detectors"
   * @param response the response to print
   * @return a display string
   */
  static QString parseAvailableAppsResponse(const QString type,
                                          boost::shared_ptr<boost::property_tree::ptree> response);
};

}

#endif // HOOT_SERVICES_TRANSLATION_INFO_RESPONSE_PARSER_H
