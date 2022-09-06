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

#ifndef HOOTEXCEPTION_H
#define HOOTEXCEPTION_H

// Qt
#include <QString>
#include <QByteArray>

// Standard
#include <exception>
#include <iostream>
#include <memory> // Strangely getting compile errors in hoot-test w/o this, even though its in HootCoreStable.h.
#include <string>
#include <vector>

namespace hoot
{

/**
 * This class is intended to be the base class for all exceptions thrown by Hootenanny.
 */
class HootException : public std::exception
{
public:

  static QString className() { return "HootException"; }

  HootException() = default;
  HootException(const char* str) { _what = QString::fromUtf8(str); }
  HootException(const std::string& str) { _what = QString::fromStdString(str); }
  HootException(QString str) { _what = str; }
  HootException(const HootException& e) { _what = e._what; }
  virtual ~HootException() throw() = default;

  virtual std::shared_ptr<HootException> clone() const
  { return std::make_shared<HootException>(*this); }

  virtual QString getName() const { return className(); }

  const QString& getWhat() const { return _what; }
  const char* what() const throw() override
  {
    _tmp = _what.toLatin1();
    return _tmp.constData();
  }

private:

  QString _what;
  mutable QByteArray _tmp;
};

/**
 * (Singleton)
 *
 * Cliff Notes for adding an exception:
 * 1. Add a HOOT_DEFINE_EXCEPTION[_STR] in your .h (possibly here).
 * 2. Add a HOOT_REGISTER_EXCEPTION in your .cpp (possibly HootException.cpp).
 *
 * Given a [shared] pointer to an exception this class will cast and throw the actual exception
 * type. This class requires exceptions to register before they can be thrown.
 *
 * This is most useful in v8 where we don't have the ability to rethrow the exception by type, but
 * only get the pointer.
 *
 * Every exception should be exposed through the factory interface. This allows V8
 * (See HootExceptionJs.h) to instantiate and throw Hoot specific exceptions such as NeedsReview.
 * To facilitate this we must be able to instantiate the exception from the factory _and_ throw
 * a hoot exception from a hoot exception pointer. If you simply
 * "HootException* e = getException(); throw *myexception;" you will throw an exception of the
 * HootException type rather than NeedsReview or similar. C++11 has some facilities that may help
 * with simplifying this eventually...
 *
 * To work around this we expose HootExceptionThrower::rethrowPointer(). This method takes a pointer
 * to an exception and loops through all known exceptions and throws an exception of the appropriate
 * type. For this to work the exceptions must first be registered via HOOT_REGISTER_EXCEPTION.
 */
class HootExceptionThrower
{
public:

  using ThrowMethod = void(*)(HootException* e);

  static HootExceptionThrower& getInstance();

  /**
   * Registers an exception so it can be thrown. You probably want to use HOOT_REGISTER_EXCEPTION.
   */
  void registerException(ThrowMethod m) { _throwMethods.push_back(m); }

  /**
   * Throw an exception of the appropriate type given a pointer. If the appropriate exception type
   * is not found a HootException will be thrown instead.
   */
  void rethrowPointer(std::shared_ptr<HootException> e) const { rethrowPointer(e.get()); }
  void rethrowPointer(HootException* e) const;

private:

  std::vector<ThrowMethod> _throwMethods;

  /** Default constructor/destructor */
  HootExceptionThrower() = default;
  ~HootExceptionThrower() = default;
  /** Delete copy constructor and assignment operator */
  HootExceptionThrower(const HootExceptionThrower&) = delete;
  HootExceptionThrower& operator=(const HootExceptionThrower&) = delete;
};

#define HOOT_REGISTER_EXCEPTION(ClassName)      \
  HOOT_FACTORY_REGISTER(HootException, ClassName) \
  static hoot::AutoRegisterException<ClassName> ClassName##AutoRegisterException;

template<class T>
class AutoRegisterException
{
public:

  AutoRegisterException()
  {
    HootExceptionThrower::getInstance().registerException(tryThrow);
  }

  static void tryThrow(HootException* e)
  {
    T* castException = dynamic_cast<T*>(e);
    if (castException)
      throw *castException;
  }
};


#define HOOT_DEFINE_EXCEPTION(Name) \
class Name : public HootException \
{ \
public: \
  static QString className() { return "" #Name; } \
  Name() : HootException() {} \
  Name(QString str) : HootException(str) { } \
  Name(const Name& e) : HootException(e.getWhat()) { } \
  virtual ~Name() throw() {} \
  std::shared_ptr<HootException> clone() const override { return std::make_shared<Name>(*this); } \
  QString getName() const override { return className(); } \
};

#define HOOT_DEFINE_EXCEPTION_STR(Name, Default) \
class Name : public HootException \
{ \
public: \
  static QString className() { return "" #Name; } \
  Name() : HootException(Default) {} \
  Name(QString str) : HootException(str) { } \
  Name(const Name& e) : HootException(e.getWhat()) { } \
  virtual ~Name() throw() {} \
  std::shared_ptr<HootException> clone() const override { return std::make_shared<Name>(*this); } \
  QString getName() const override { return className(); } \
};

/****
 * So you want to add an exception. PLEASE READ HootExceptionThrower above
 ****/
HOOT_DEFINE_EXCEPTION(FileNotFoundException)
HOOT_DEFINE_EXCEPTION(IllegalArgumentException)
HOOT_DEFINE_EXCEPTION(InternalErrorException)
HOOT_DEFINE_EXCEPTION(IoException)
HOOT_DEFINE_EXCEPTION(NeedsReviewException)
HOOT_DEFINE_EXCEPTION(UnsupportedException)
// Was the error a result of a test being misconfigured? If so, use this.
HOOT_DEFINE_EXCEPTION(TestConfigurationException)

// This exception exists to kick out of logic whose performance ends up having a very poor runtime
// against certain input data. Not the most elegant solution but necessary at this time. See River
// Conflation as an example.
HOOT_DEFINE_EXCEPTION_STR(RecursiveComplexityException, "RecursiveComplexityException")
HOOT_DEFINE_EXCEPTION_STR(NotImplementedException, "Not Implemented")
HOOT_DEFINE_EXCEPTION_STR(EmptyMapInputException, "Empty map input.")
}

#endif // HOOTEXCEPTION_H
