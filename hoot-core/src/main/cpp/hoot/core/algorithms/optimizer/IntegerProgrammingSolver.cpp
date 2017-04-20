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
#include "IntegerProgrammingSolver.h"

#include <hoot/core/util/SignalCatcher.h>

namespace hoot
{

IntegerProgrammingSolver::IntegerProgrammingSolver()
{
  _lp = glp_create_prob();
  _timeLimit = -1;
}

IntegerProgrammingSolver::~IntegerProgrammingSolver()
{
  glp_delete_prob(_lp);
}

double IntegerProgrammingSolver::getColumnPrimalValue(int j) const
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

double IntegerProgrammingSolver::getObjectiveValue() const
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

void IntegerProgrammingSolver::solve()
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

void IntegerProgrammingSolver::solveBranchAndCut()
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
    //  Register a new SIGABRT signal handler to ignore it for this call only
    SignalCatcher::getInstance()->registerHandler(SIGABRT, SIG_IGN);
    //  This function can intermittently throw an exception that is heretofore unhandled
    result = glp_intopt(_lp, &iocp);
    //  Unregister the SIGABRT signal handler and use the hoot default
    SignalCatcher::getInstance()->unregisterHandler(SIGABRT);
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

void IntegerProgrammingSolver::solveSimplex()
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
    //  Register a new SIGABRT signal handler to ignore it for this call only
    SignalCatcher::getInstance()->registerHandler(SIGABRT, SIG_IGN);
    //  This function can potentially throw an exception that is heretofore unhandled
    result = glp_simplex(_lp, &smcp);
    //  Unregister the SIGABRT signal handler and use the hoot default
    SignalCatcher::getInstance()->unregisterHandler(SIGABRT);
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


}
