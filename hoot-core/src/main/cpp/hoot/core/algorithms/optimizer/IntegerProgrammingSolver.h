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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef INTEGERPROGRAMMINGSOLVER_H
#define INTEGERPROGRAMMINGSOLVER_H

#include <hoot/core/HootConfig.h>

// GLPK
#if HOOT_HAVE_GLPK_GLPK_H == 1
# include <glpk/glpk.h>
#elif HOOT_HAVE_GLPK_H == 1
# include <glpk.h>
#else
# error "A valid glpk.h was not found during configure."
#endif

// Hoot
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QString>

// Standard
#include <vector>

namespace hoot
{
using namespace std;
/**
 * A thin C++ style wrapper for a small subset of GLPK.
 * http://www.gnu.org/software/glpk/
 *
 */
class IntegerProgrammingSolver
{
public:
  IntegerProgrammingSolver();

  virtual ~IntegerProgrammingSolver();

  void addColumns(int colCount) { glp_add_cols(_lp, colCount); }

  void addRows(int rowCount) { glp_add_rows(_lp, rowCount); }

  double getColumnPrimalValue(int j) const
  {
    if (isIntegerProblem())
    {
      return glp_mip_col_val(_lp, j);
    }
    else
    {
      return glp_get_col_prim(_lp, j);
    }
  }

  double getObjectiveValue() const
  {
    if (isIntegerProblem())
    {
      return glp_mip_obj_val(_lp);
    }
    else
    {
      return glp_get_obj_val(_lp);
    }
  }

  int getNumColumns() const { return glp_get_num_cols(_lp); }

  /**
   * Returns true if at least one column is an integer or binary column.
   */
  bool isIntegerProblem() const
  {
    return glp_get_num_int(_lp) > 0 || glp_get_num_bin(_lp) > 0;
  }

  /**
   * Just like GLPK the index starts at 1, not 0. This means that i.size() - 1 records will be
   * loaded.
   */
  void loadMatrix(const vector<int>& i, const vector<int>& j, const vector<double>& r)
  {
    glp_load_matrix(_lp, i.size() - 1, &(i[0]), &(j[0]), &(r[0]));
  }

  void setColumnBounds(int colIndex, int type, double lowerBound, double upperBound)
  {
    glp_set_col_bnds(_lp, colIndex, type, lowerBound, upperBound);
  }

  void setColumnKind(int colIndex, int kind) { glp_set_col_kind(_lp, colIndex, kind); }

  void setColumnName(int colIndex, const QString& s)
  {
    glp_set_col_name(_lp, colIndex, toUtf8(s));
  }

  void setObjectiveCoefficient(int colIndex, double coef) { glp_set_obj_coef(_lp, colIndex, coef); }

  /**
   * Sets the objective direction to either GLP_MAX or GLP_MIN.
   */
  void setObjectiveDirection(int dir) { glp_set_obj_dir(_lp, dir); }

  /**
   * Similar to glp_set_row_name.
   */
  void setRowName(int rowIndex, const QString& s)
  {
    glp_set_row_name(_lp, rowIndex, s.toUtf8().data());
  }

  /**
   * Similar to glp_set_row_bnds.
   */
  void setRowBounds(int rowIndex, int type, double lowerBound, double upperBound)
  {
    glp_set_row_bnds(_lp, rowIndex, type, lowerBound, upperBound);
  }

  void setProblemName(const QString& s) { glp_set_prob_name(_lp, toUtf8(s)); }

  void setTimeLimit(double limit) { _timeLimit = limit; }

  /**
   * Solves the problem using the default method.
   */
  void solve()
  {
    if (isIntegerProblem())
    {
      solveBranchAndCut();
    }
    else
    {
      solveSimplex();
    }
  }

  void solveBranchAndCut()
  {
    glp_iocp iocp;
    glp_init_iocp(&iocp);
    iocp.presolve = GLP_ON;
    iocp.binarize = GLP_ON;
    if (_timeLimit > 0)
    {
      iocp.tm_lim = _timeLimit * 1000.0 + 0.5;
    }
    if (Log::getInstance().getLevel() <= Log::Debug)
    {
      iocp.msg_lev = GLP_MSG_ON;
    }
    else if (Log::getInstance().getLevel() <= Log::Warn)
    {
      iocp.msg_lev = GLP_MSG_ERR;
    }
    else
    {
      iocp.msg_lev = GLP_MSG_OFF;
    }

    int result = 0;
    try
    {
      //  This function can intermittently throw an exception that is heretofore unhandled
      result = glp_intopt(_lp, &iocp);
    }
    catch (...)
    {
      throw Exception(QString("Error solving integer programming problem in glp_intopt()."));
    }

    // if there was an error and the error was not a timeout or iteration limit error.
    if (result != 0 && result != GLP_EITLIM && result != GLP_ETMLIM)
    {
      throw Exception(QString("Error solving integer programming problem. %1").arg(result));
    }
  }

  void solveSimplex()
  {
    glp_smcp smcp;
    glp_init_smcp(&smcp);
    if (_timeLimit > 0)
    {
      smcp.tm_lim = _timeLimit * 1000.0 + 0.5;
    }
    if (Log::getInstance().getLevel() <= Log::Debug)
    {
      smcp.msg_lev = GLP_MSG_ON;
    }
    else if (Log::getInstance().getLevel() <= Log::Warn)
    {
      smcp.msg_lev = GLP_MSG_ERR;
    }
    else
    {
      smcp.msg_lev = GLP_MSG_OFF;
    }

    int result = 0;
    try
    {
      //  This function can potentially throw an exception that is heretofore unhandled
      result = glp_simplex(_lp, &smcp);
    }
    catch (...)
    {
      throw Exception(QString("Error solving integer programming problem in glp_simplex()."));
    }

    // if there was an error and the error was not a timeout or iteration limit error.
    if (result != 0 && result != GLP_EITLIM && result != GLP_ETMLIM)
    {
      throw Exception(QString("Error solving integer programming problem. %1").arg(result));
    }
  }

private:

  mutable glp_prob* _lp;
  double _timeLimit;

  const char* toUtf8(const QString& s) { return s.toUtf8().data(); }
};

}

#endif // INTEGERPROGRAMMINGSOLVER_H
