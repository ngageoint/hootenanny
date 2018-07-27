
#ifndef CONFIGOPTIONSDISPLAYER_H
#define CONFIGOPTIONSDISPLAYER_H

// Qt
#include <QString>

namespace hoot
{

/**
 *
 */
class ConfigOptionsDisplayer
{
public:

  /**
   *
   *
   * @param withDetails
   */
  static void displayAllOptionNames(const bool withDetails = false);

  /**
   *
   *
   * @param optionName
   * @param withDetails
   */
  static void displayOptionName(const QString optionName, const bool withDetails = false);
  
private:

  static void _printAllConfigOptionsDetails(const QString optionName,
                                            const QString configOptionsFile);
};

}

#endif // CONFIGOPTIONSDISPLAYER_H
