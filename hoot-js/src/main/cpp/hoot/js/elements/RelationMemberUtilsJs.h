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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef RELATION_MEMBER_UTILS_JS_H
#define RELATION_MEMBER_UTILS_JS_H

// Hoot
#include <hoot/js/HootBaseJs.h>
#include <hoot/js/SystemNodeJs.h>

namespace hoot
{

/**
 * @brief The RelationMemberUtilsJs class is a JS wrapper around the RelationMemberUtils class
 */
class RelationMemberUtilsJs : public HootBaseJs
{
public:

  ~RelationMemberUtilsJs() override = default;

  static void Init(v8::Local<v8::Object> target);

  /**
   * @brief see RelationMemberUtils::isMemberOfRelationWithType
   */
  static void isMemberOfRelationWithType(const v8::FunctionCallbackInfo<v8::Value>& args);
  /**
   * @brief see RelationMemberUtils::isMemberOfRelationWithType
   */
  static void isMemberOfRelationInCategory(const v8::FunctionCallbackInfo<v8::Value>& args);
  /**
   * @brief see RelationMemberUtils::isMemberOfRelationWithType
   */
  static void isMemberOfRelationWithTagKey(const v8::FunctionCallbackInfo<v8::Value>& args);
  /**
   * @brief see RelationMemberUtils::isMemberOfRelationWithType
   */
  static void getNumRelationMemberNodes(const v8::FunctionCallbackInfo<v8::Value>& args);
  /**
   * @brief see RelationMemberUtils::isMemberOfRelationWithType
   */
  static void relationsHaveConnectedWayMembers(const v8::FunctionCallbackInfo<v8::Value>& args);

private:

  RelationMemberUtilsJs() = default;
};

}


#endif // RELATION_MEMBER_UTILS_JS_H
