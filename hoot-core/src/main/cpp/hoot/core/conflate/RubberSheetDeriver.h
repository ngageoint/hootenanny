
#ifndef RUBBERSHEETDERIVER_H
#define RUBBERSHEETDERIVER_H

// Qt
#include <QString>

/**
 * Derives a rubber sheet transforms
 */
namespace hoot
{

class RubberSheetDeriver
{
public:

  RubberSheetDeriver();

  /**
   * Derives rubber sheet transforms for rubber sheeting either direction between two inputs
   *
   * @param input1 input 1
   * @param input2 input 2
   * @param transform2To1 output transformation for rubber sheeting from input 2 to input 1
   * @param transform1To2 output transformation for rubber sheeting from input 1 to input 2
   * @param ref if true, input 1 is treated as the reference input and held constant
   */
  void derive(const QString input1, const QString input2, const QString transform2To1,
              const QString transform1To2 = "", const bool ref = false);
};

}

#endif // RUBBERSHEETDERIVER_H
