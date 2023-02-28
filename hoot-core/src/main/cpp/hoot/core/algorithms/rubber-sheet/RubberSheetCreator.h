#ifndef RUBBERSHEETCREATOR_H
#define RUBBERSHEETCREATOR_H

namespace hoot
{

/**
 * @brief The RubberSheetCreator class creates a rubber sheet transforms from manual tie-points.
 */
class RubberSheetCreator
{
public:

  RubberSheetCreator() = default;

  /**
   * @brief derive derives rubber sheet transforms for rubber sheeting either direction between two
   * inputs.
   * @param tiepoints tie point filename
   * @param input filename for the input map used for projection
   * @param transform2To1 output transformation for rubber sheeting from input 2 to input 1
   * @param transform1To2 output transformation for rubber sheeting from input 1 to input 2
   * @param ref if true, input 1 is treated as the reference input and held constant
   */
  void create(const QString& tiepoints, const QString& input, const QString& input2,  const QString& transform2To1,
              const QString& transform1To2 = "", const bool ref = false) const;
};

}

#endif // RUBBERSHEETCREATOR_H
