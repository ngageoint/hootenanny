#ifndef SPARKJSONWRITER_H
#define SPARKJSONWRITER_H

// hoot
#include <hoot/core/io/PartialOsmMapWriter.h>

// Qt
#include <QFile>

#include "../conflate/multiary/SearchBoundsCalculator.h"

namespace hoot
{

/**
 * Uses the match creator (probably ScriptMatchCreator) to determine the search bounds of each
 * element, then writes a record that is suitable for reading in Spark prototype code.
 *
 * @note Only nodes are supported.
 */
class SparkJsonWriter : public PartialOsmMapWriter
{
public:

  static std::string className() { return "hoot::SparkJsonWriter"; }

  SparkJsonWriter();

  virtual ~SparkJsonWriter() {}

  /**
   * @see OsmMapWriter
   */
  virtual void close() { if (_fp.get()) { _fp->close(); _fp.reset(); } }

  /**
   * @see PartialOsmMapWriter
   */
  virtual void finalizePartial() { close(); }

  /**
   * @see OsmMapWriter
   */
  virtual bool isSupported(QString url) { return url.endsWith(".spark"); }

  /**
   * Open the specified filename for writing.
   */
  virtual void open(QString fileName);

  /**
   * @see PartialOsmMapWriter
   */
  virtual void writePartial(const ConstNodePtr& n);

  /**
   * @see PartialOsmMapWriter
   */
  virtual void writePartial(const ConstWayPtr&) { throw NotImplementedException(); }

  /**
   * @see PartialOsmMapWriter
   */
  virtual void writePartial(const ConstRelationPtr&) { throw NotImplementedException(); }

private:
  boost::shared_ptr<QFile> _fp;

  SearchBoundsCalculatorPtr _bounds;

  int _precision;
};

}

#endif // SPARKJSONWRITER_H
