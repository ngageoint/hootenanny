
#ifndef RECURSIVE_SET_TAG_VALUE_OP_H
#define RECURSIVE_SET_TAG_VALUE_OP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * TODO
 */
class RecursiveSetTagValueOp : public OsmMapOperation, public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::RecursiveSetTagValueOp"; }

  RecursiveSetTagValueOp();
  RecursiveSetTagValueOp(ElementCriterionPtr elementCriterion, const QString& key,
                         const QString& val);

  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  virtual QString getDescription() const override { return "TODO"; }

  virtual QString getInitStatusMessage() const { return "TODO"; }

  virtual QString getCompletedStatusMessage() const { return "TODO"; }

private:

  ElementCriterionPtr _crit;
  QString _key;
  QString _val;
};

}

#endif // RECURSIVE_SET_TAG_VALUE_OP_H
