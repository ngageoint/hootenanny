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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef SCHEMATRANSLATIONOP_H
#define SCHEMATRANSLATIONOP_H

// hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/SchemaTranslationVisitor.h>

namespace hoot
{

/**
 * Applies a translation to an entire map. This would be needed if doing a translation from the JS
 * interface. If doing a translation via the convert command, consider using TranslationVisitor
 * instead if memory is a concern, as it does not require the whole map to be read into memory at
 * once.
 */
class SchemaTranslationOp : public OsmMapOperation, public Configurable
{
public:

  static QString className() { return "SchemaTranslationOp"; }

  SchemaTranslationOp() = default;
  ~SchemaTranslationOp() override = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override
  { return _translator.getInitStatusMessage(); }
  QString getCompletedStatusMessage() const override
  { return _translator.getCompletedStatusMessage(); }

  QString getDescription() const override { return _translator.getDescription(); }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  /**
   * Set the path to the translation script.
   */
  void setTranslationScript(QString path) { _translator.setTranslationScript(path); }

private:

  SchemaTranslationVisitor _translator;
};

}

#endif // SCHEMATRANSLATIONOP_H
