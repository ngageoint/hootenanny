
#ifndef RUBBERSHEETDERIVER_H
#define RUBBERSHEETDERIVER_H

// Qt
#include <QString>

namespace hoot
{

class RubberSheetDeriver
{
public:

  RubberSheetDeriver();

  void derive(const QString input1, const QString input2, const QString transform2To1,
              const QString transform1To2 = "", const bool ref = false);
};

}

#endif // RUBBERSHEETDERIVER_H
