#ifndef ELEMENTSORTER_H
#define ELEMENTSORTER_H

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Converts data from one Hootenanny supported format to another
 */
class DataConverter
{

public:

  static unsigned int logWarnCount;

  DataConverter();

  void convert(const QStringList inputs, const QString output);

  void setTranslation(const QString translation) { _translation = translation; }
  void setColumns(const QStringList columns) { _columns = columns; }
  void setColsArgSpecified(const bool specified) { _colsArgSpecified = specified; }
  void setFeatureReadLimit(const int limit) { _featureReadLimit = limit; }

private:

  QString _translation;
  QStringList _columns;
  bool _colsArgSpecified;
  int _featureReadLimit;

  void _convertOsmToShp(const QString input, const QString output);
  void _convertOsmToOgr(const QString input, const QString output);
  void _convertOgrToOsm(const QStringList inputs, const QString output);
  void _generalConvert(const QString input, const QString output);

  /*
   * Return true if all the specified operations are valid streaming operations.
   *
   * There are some ops that require the whole map be available in RAM (e.g. remove duplicate
   * nodes). These operations are not applicable for streaming.
   */
  bool _areValidStreamingOps(const QStringList ops);

  void _validateInput(const QStringList inputs, const QString output);
};

}

#endif // DATACONVERTER_H
