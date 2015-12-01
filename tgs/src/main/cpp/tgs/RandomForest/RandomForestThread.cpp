#include "RandomForestThread.h"

#include "../TgsException.h"

namespace Tgs
{
  RandomForestThread::RandomForestThread(boost::shared_ptr<DataFrame> data,
    boost::shared_ptr<RandomTree> tree, unsigned int numFactors, unsigned int nodeSize,
    bool makeBalanced)
  {
    _data = data;
    _tree = tree;
    _numFactors = numFactors;
    _nodeSize = nodeSize;
    _makeBalanced = makeBalanced;
  }

  RandomForestThread::~RandomForestThread(){}

  void RandomForestThread::run()
  {
    try
    {
      _tree->trainMulticlass(_data, _numFactors, _nodeSize, _makeBalanced);
    }
    catch(const Exception & e)
    {
      std::stringstream pathStr;
      pathStr << typeid(this).name() << "::" << __FUNCTION__ << " Line( " << __LINE__ << "): -->" <<
        e.what();
      emit exceptionThrown(e.what());
    }
  }
}

