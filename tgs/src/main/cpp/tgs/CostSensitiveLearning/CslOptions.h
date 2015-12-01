#ifndef CSLOPTIONS_H
#define CSLOPTIONS_H

namespace Tgs
{

class CslOptions
{
public:
  static void run(int argc, char** argv);

private:
  static void _runBasicExample(int argc, char** argv);

  static void _runValidationExample(int argc, char** argv);
};

}

#endif // CSLOPTIONS_H
