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
 * @copyright Copyright (C) 2015, 2017, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__BASIC_MATH_CALCULATOR_NODE_H__
#define __TGS__BASIC_MATH_CALCULATOR_NODE_H__

// Standard Includes
#include <math.h>

#include "CalculatorGenomeNode.h"

namespace Tgs
{
  class TGS_EXPORT BinaryCalculatorNode : public CalculatorGenomeNode
  {
  public:
    // I started out doing these as simple static member variables, but it complained when
    // exporting out of the DLL. Did some searches and it doesn't make sense to me, but this
    // works.
    static const std::string V1() { return std::string("v1"); }
    static const std::string V2() { return std::string("v2"); }

    virtual ~BinaryCalculatorNode() {}

    virtual const std::map<std::string, std::string> getInputs() const;

    virtual double getOutput(const int uid) const;

    virtual void mutate(double /*p*/) {}

  protected:
    virtual double _calculate(double v1, double v2) const = 0;
  };

  class TGS_EXPORT AddCalculatorNode : public BinaryCalculatorNode
  {
  public:
    virtual ~AddCalculatorNode() {}
  protected:
    virtual double _calculate(double v1, double v2) const { return v1 + v2; }

    virtual CalculatorGenomeNode* _createNew() const { return new AddCalculatorNode(); }

    virtual std::string _toLabel() const { return "+"; }
  };

  class TGS_EXPORT SubtractCalculatorNode : public BinaryCalculatorNode
  {
  public:
    virtual ~SubtractCalculatorNode() {}
  protected:
    virtual double _calculate(double v1, double v2) const { return v1 - v2; }

    virtual CalculatorGenomeNode* _createNew() const { return new SubtractCalculatorNode(); }

    virtual std::string _toLabel() const { return "-"; }
  };

  class TGS_EXPORT MultiplyCalculatorNode : public BinaryCalculatorNode
  {
  public:
    virtual ~MultiplyCalculatorNode() {}

    virtual void mutate(double /*p*/) {}

  protected:
    virtual double _calculate(double v1, double v2) const { return v1 * v2; }

    virtual CalculatorGenomeNode* _createNew() const { return new MultiplyCalculatorNode(); }

    virtual std::string _toLabel() const { return "*"; }
  };

  class TGS_EXPORT DivideCalculatorNode : public BinaryCalculatorNode
  {
  public:
    virtual ~DivideCalculatorNode() {}
  protected:
    virtual double _calculate(double v1, double v2) const { return v2 == 0.0 ? 0.0 : v1 / v2; }

    virtual CalculatorGenomeNode* _createNew() const { return new DivideCalculatorNode(); }

    virtual std::string _toLabel() const { return "/"; }
  };

  class TGS_EXPORT PowCalculatorNode : public BinaryCalculatorNode
  {
  public:
    virtual ~PowCalculatorNode() {}
  protected:
    virtual double _calculate(double v1, double v2) const { return pow(fabs(v1), v2); }

    virtual CalculatorGenomeNode* _createNew() const { return new PowCalculatorNode(); }

    virtual std::string _toLabel() const { return "^"; }
  };

  class TGS_EXPORT UnaryCalculatorNode : public CalculatorGenomeNode
  {
  public:
    static const std::string V1() { return std::string("v1"); }

    virtual ~UnaryCalculatorNode() {}

    virtual const std::map<std::string, std::string> getInputs() const;

    virtual double getOutput(const int uid) const;

    virtual void mutate(double /*p*/) {}
  protected:
    virtual double _calculate(double v1) const = 0;
  };

  class TGS_EXPORT SqrtCalculatorNode : public UnaryCalculatorNode
  {
  public:
    virtual ~SqrtCalculatorNode() {}

  protected:
    virtual double _calculate(double v1) const { return sqrt(fabs(v1)); }

    virtual CalculatorGenomeNode* _createNew() const { return new SqrtCalculatorNode(); }

    virtual std::string _toLabel() const { return "sqrt"; }
  };

  class TGS_EXPORT LogCalculatorNode : public UnaryCalculatorNode
  {
  public:
    virtual ~LogCalculatorNode() {}
  protected:
    virtual double _calculate(double v1) const { return v1 == 0.0 ? -100 : log(fabs(v1)); }

    virtual CalculatorGenomeNode* _createNew() const { return new LogCalculatorNode(); }

    virtual std::string _toLabel() const { return "log"; }
  };
}


#endif
