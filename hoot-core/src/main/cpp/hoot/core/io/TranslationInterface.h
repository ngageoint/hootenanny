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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */

#ifndef TRANSLATION_INTERFACE_H
#define TRANSLATION_INTERFACE_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/io/schema/StrictChecking.h>
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>

// GEOS
#include <geos/geom/Geometry.h>

namespace hoot
{

/**
 * Interface for writer classes to translate elements to features via a translation script
 */
class TranslationInterface
{
public:

  static QString className() { return "TranslationInterface"; }

  TranslationInterface();
  ~TranslationInterface() = default;

  /** Initialize/create the actual translator, must be called *after* setSchemaTranslationScript() */
  void initTranslator();

  /**
   * @brief translateToFeatures Translates the element to a geometry and a
   *        vector of features
   * @param provider Should provide the element and all its children
   * @param e Element to translate
   * @param g Geometry output
   * @param tf Vector of translated features output
   */
  void translateToFeatures(const ElementProviderPtr& provider, const ConstElementPtr& e,
                           std::shared_ptr<geos::geom::Geometry>& g,
                           std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>& tf) const;

  void setSchemaTranslationScript(const QString& path) { _scriptPath = path; }

protected:

  static int logWarnCount;
  /** Script path */
  QString _scriptPath;
  /** Pointer to the actual translator object */
  mutable std::shared_ptr<ScriptToOgrSchemaTranslator> _translator;
  /** Schema loaded by the translator */
  std::shared_ptr<const Schema> _schema;
  StrictChecking _strictChecking;
};

}


#endif // OGRWRITER_H
