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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SCHEMATRANSLATIONVISITOR_H
#define SCHEMATRANSLATIONVISITOR_H

// Hoot
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QString>

namespace hoot
{

class ScriptToOgrSchemaTranslator;

/**
 * Translates elements that are passed to the visitor.
 */
class SchemaTranslationVisitor : public ElementVisitor, public Configurable
{
public:

  static std::string className() { return "hoot::SchemaTranslationVisitor"; }

  SchemaTranslationVisitor();

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  void setTranslationScript(QString path);
  void setTranslationDirection(QString direction);

  virtual void visit(const ElementPtr& e);

  virtual QString getDescription() const { return "Translates features to a schema"; }

  virtual QString getInitStatusMessage() const
  { return "Translating features to a schema..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Translated " + QString::number(_numAffected) + " features to a schema"; }

  virtual std::string getClassName() const { return className(); }

private:

  ScriptSchemaTranslatorPtr _translator;
  ScriptToOgrSchemaTranslator* _ogrTranslator;
  bool _toOgr;
};

}

#endif // SCHEMATRANSLATIONVISITOR_H
