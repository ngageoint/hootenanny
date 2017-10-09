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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef IMPLICITTAGRULESJSONWRITER_H
#define IMPLICITTAGRULESJSONWRITER_H

// Hoot
#include <hoot/rnd/io/ImplicitTagRulesWriter.h>

// Qt
#include <QFile>

namespace hoot
{

/**
 *
 */
class ImplicitTagRulesJsonWriter : public ImplicitTagRulesWriter
{

public:

  static std::string className() { return "hoot::ImplicitTagRulesJsonWriter"; }

  ImplicitTagRulesJsonWriter();
  ~ImplicitTagRulesJsonWriter();

  virtual bool isSupported(const QString url);

  virtual void open(const QString url);

  virtual void write(const ImplicitTagRules& rules);

  virtual void write(const ImplicitTagRulesByWord& rules);

  virtual void close();

private:

  boost::shared_ptr<QFile> _file;
};

}

#endif // IMPLICITTAGRULESJSONWRITER_H
