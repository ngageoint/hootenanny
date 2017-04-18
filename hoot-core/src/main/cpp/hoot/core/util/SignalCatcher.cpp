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

#include "SignalCatcher.h"

// GCC
#include <execinfo.h>

// Standard
#include <cxxabi.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

namespace hoot
{

boost::shared_ptr<SignalCatcher> SignalCatcher::_instance;

SignalCatcher::SignalCatcher()
  : _defaultSet(false)
{
}

boost::shared_ptr<SignalCatcher> SignalCatcher::getInstance()
{
  if (!SignalCatcher::_instance)
    SignalCatcher::_instance.reset(new SignalCatcher());
  return SignalCatcher::_instance;
}

void SignalCatcher::registerDefaultHandlers()
{
  // These _might_ work depending on the specific error.
  std::set_terminate(terminateHandler);
  registerHandler(SIGSEGV, default_handler);
  registerHandler(SIGABRT, default_handler);
  _defaultSet = true;
}

void SignalCatcher::registerHandler(unsigned int signal_id, __sighandler_t handler)
{
  if (signal_id < _NSIG)
  {
    if (_handlers.find(signal_id) == _handlers.end())
      _handlers[signal_id] = std::stack<__sighandler_t>();
    _handlers[signal_id].push(handler);
    signal(signal_id, handler);
  }
}

void SignalCatcher::unregisterHandler(unsigned int signal_id)
{
  if (signal_id >= _NSIG)
    return;
  __sighandler_t handler = SIG_DFL;
  if (_handlers.find(signal_id) != _handlers.end())
  {
    switch (signal_id)
    {
    case SIGSEGV:
    case SIGABRT:
      //  These could possibly have a default handler already set
      if ((_handlers[signal_id].size() > 1 && _defaultSet) || _handlers[signal_id].size() > 0)
      {
        _handlers[signal_id].pop();
        handler = _handlers[signal_id].top();
      }
      break;
    default:
      //  No default other than SIG_DFL
      if (_handlers[signal_id].size() > 0)
      {
        _handlers[signal_id].pop();
        handler = _handlers[signal_id].top();
      }
      break;
    }
  }
  signal(signal_id, handler);
}

void SignalCatcher::default_handler(int sig)
{
  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);

  print_stacktrace();

  fflush(stderr);
  exit(-1);
}

/** Print a demangled stack backtrace of the caller function to FILE* out. */
void SignalCatcher::print_stacktrace(FILE *out, unsigned int max_frames)
{
    fprintf(out, "stack trace:\n");

    // storage array for stack trace address data
    void* addrlist[max_frames+1];

    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0) {
        fprintf(out, "  <empty, possibly corrupt>\n");
        return;
    }

    // resolve addresses into strings containing "filename(function+address)",
    // this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (int i = 1; i < addrlen; i++)
    {
        char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

        // find parentheses and +address offset surrounding the mangled name:
        // ./module(function+0x15c) [0x8048a6d]
        for (char *p = symbollist[i]; *p; ++p)
        {
            if (*p == '(')
                begin_name = p;
            else if (*p == '+')
                begin_offset = p;
            else if (*p == ')' && begin_offset) {
                end_offset = p;
                break;
            }
        }

        if (begin_name && begin_offset && end_offset
            && begin_name < begin_offset)
        {
            *begin_name++ = '\0';
            *begin_offset++ = '\0';
            *end_offset = '\0';

            // mangled name is now in [begin_name, begin_offset) and caller
            // offset in [begin_offset, end_offset). now apply
            // __cxa_demangle():

            int status;
            char* ret = abi::__cxa_demangle(begin_name,
                                            funcname, &funcnamesize, &status);
            if (status == 0) {
                funcname = ret; // use possibly realloc()-ed string
                //fprintf(out, "  %s : %s+%s\n",
                //        symbollist[i], funcname, begin_offset);
                fprintf(out, "  %s +%s\n", funcname, begin_offset);
            }
            else {
                // demangling failed. Output function name as a C function with
                // no arguments.
                fprintf(out, "  %s : %s()+%s\n",
                        symbollist[i], begin_name, begin_offset);
            }
        }
        else
        {
            // couldn't parse the line? print the whole line.
            fprintf(out, "  %s\n", symbollist[i]);
        }
    }

    free(funcname);
    free(symbollist);
}

void SignalCatcher::terminateHandler()
{
  static bool tried_throw = false;

  try
  {
    // try once to re-throw currently active exception
    if (!tried_throw++)
    {
      throw;
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << __FUNCTION__ << " caught unhandled exception. what(): "
              << e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << __FUNCTION__ << " caught unknown/unhandled exception."
              << std::endl;
  }

  print_stacktrace();
  fflush(stderr);
  abort();
}

}
