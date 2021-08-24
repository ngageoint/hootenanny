/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SCHEMATRANSLATIONVISITOR_H
#define SCHEMATRANSLATIONVISITOR_H

// Hoot
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class ScriptToOgrSchemaTranslator;

/**
 * Translates elements that are passed to the visitor.
 */
class SchemaTranslationVisitor : public ElementVisitor, public Configurable
{
public:

  static QString className() { return "hoot::SchemaTranslationVisitor"; }

  SchemaTranslationVisitor();
  ~SchemaTranslationVisitor() = default;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  void visit(const ElementPtr& e) override;

  QString getInitStatusMessage() const override
  { return "Translating features to a schema..."; }
  QString getCompletedStatusMessage() const override
  { return "Translated " + StringUtils::formatLargeNumber(_numAffected) + " features to a schema"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Translates features to a schema"; }

  void setTranslationScript(QString path);
  void setTranslationDirection(QString direction);

private:

  ScriptSchemaTranslatorPtr _translator;
  std::shared_ptr<ScriptToOgrSchemaTranslator> _ogrTranslator;
  bool _toOgr;

  // keys for tags containing CE data
  QStringList _circularErrorTagKeys;
  // if populated, skips elements whose status don't match the filter; defaults to Status::Invalid,
  // which equates to no filtering
  Status _elementStatusFilter;
};

}

#endif // SCHEMATRANSLATIONVISITOR_H
