
#ifndef RUBBERSHEETER_H
#define RUBBERSHEETER_H

// Qt
#include <QString>

namespace hoot
{

/**
 * Rubber sheets two inputs together
 */
class RubberSheeter
{
public:

  RubberSheeter();

  /**
   * Rubber sheets two inputs together
   *
   * @param input1 the first input to rubber sheet
   * @param input2 the second input to rubber sheet
   * @param output the rubber sheeted combined output
   */
  void rubberSheet(const QString input1, const QString input2, const QString output);
 
};

}

#endif // RUBBERSHEETER_H
