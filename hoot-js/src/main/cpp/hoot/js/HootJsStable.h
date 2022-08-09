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
 * @copyright Copyright (C) 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef HOOTJSSTABLE_H
#define HOOTJSSTABLE_H

// hoot
#include <hoot/core/HootCoreStable.h>

#pragma GCC diagnostic push
//  Remove unused-parameter warnings from v8
#pragma GCC diagnostic ignored "-Wunused-parameter"
//  Remove shadowed variable warnings from v8
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wshadow-compatible-local"

// v8
#include <v8.h>
#include <libplatform/libplatform.h>

#pragma GCC diagnostic pop

namespace hoot
{

//  v8::Persistent is no longer derived from v8::Local requiring these conversion functions
template<class T>
inline v8::Local<T> ToLocal(v8::Persistent<T>* p) { return *reinterpret_cast<v8::Local<T>*>(p); }
template<class T>
inline v8::Local<T> ToLocal(const v8::Persistent<T>* p) { return *reinterpret_cast<v8::Local<T>*>(const_cast<v8::Persistent<T>*>(p)); }
template<class T>
inline v8::Persistent<T> ToPersistent(v8::Local<T>* l) { return *reinterpret_cast<v8::Persistent<T>*>(l); }
template<class T>
inline v8::Persistent<T> ToPersistent(const v8::Local<T>* l) { return *reinterpret_cast<v8::Persistent<T>*>(const_cast<v8::Local<T>*>(l)); }

}

#endif // HOOTJSSTABLE_H
