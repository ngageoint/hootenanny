
#ifndef RUBBERSHEETAPPLIER_H
#define RUBBERSHEETAPPLIER_H

// Qt
#include <QString>

namespace hoot
{

/**
 * Applies a rubber sheet transform
 */
class RubberSheetApplier
{
public:

  RubberSheetApplier();

  /**
   * Applies a rubber sheet transform to input and writes to output
   *
   * @param transform transform to apply
   * @param input input to apply the transform to
   * @param output path to write the transformed output
   */
  void apply(const QString transform, const QString input, const QString output);
};

}

#endif // RUBBERSHEETAPPLIER_H
