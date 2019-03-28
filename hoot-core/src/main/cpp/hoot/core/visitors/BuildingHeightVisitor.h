
#ifndef BUILDING_HEIGHT_VISITOR_H
#define BUILDING_HEIGHT_VISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/info/NumericStatistic.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/info/OperationStatusInfo.h>

// Qt
#include <QRegularExpression>

namespace hoot
{

class BuildingHeightVisitor : public ConstElementVisitor, public NumericStatistic,
  public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::BuildingHeightVisitor"; }

  static unsigned int logWarnCount;

  BuildingHeightVisitor();

  virtual void visit(const ConstElementPtr& e) override;

  virtual QString getDescription() const { return "Calculates building height statistics"; }

  virtual QString getInitStatusMessage() const
  { return "Calculating building height statistics..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Calculated height statistics for " + QString::number(_numAffected) + " buildings"; }

  virtual long numWithStat() const { return _numAffected; }
  virtual double getStat() const { return _totalHeight; }
  virtual double getMin() const { return _minHeight; }
  virtual double getMax() const { return _maxHeight; }
  virtual double getAverage() const { return _totalHeight / _numAffected; }

private:

  BuildingCriterion _crit;

  // in meters
  Meters _totalHeight;
  Meters _minHeight;
  Meters _maxHeight;

  QRegularExpression _feetRegEx;

  void _createFeetRegEx();
  Meters _getHeight(const ConstElementPtr& e);
  // considered putting this parsing logic in Tags::getLength() but decided against it for the time
  // being
  Meters _parseMetersVal(const QString heightStr);
  Meters _parseFeetVal(const QString heightStr, const QRegularExpressionMatch& regexMatch) const;
  Meters _parseFeetToken(const QString type, const QRegularExpressionMatch& regexMatch,
                         bool& successfulParse) const;
  void _logInvalidFeetHeight(const QString heightStr) const;
  void _cleanHeightStr(QString& heightStr);
};

}

#endif // BUILDING_HEIGHT_VISITOR_H
