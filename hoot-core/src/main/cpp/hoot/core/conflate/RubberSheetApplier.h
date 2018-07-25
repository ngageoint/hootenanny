
#ifndef RUBBERSHEETAPPLIER_H
#define RUBBERSHEETAPPLIER_H

// Qt
#include <QString>

namespace hoot
{

class RubberSheetApplier
{
public:

  RubberSheetApplier();

  void apply(const QString transform, const QString input, const QString output);
};

}

#endif // RUBBERSHEETAPPLIER_H
