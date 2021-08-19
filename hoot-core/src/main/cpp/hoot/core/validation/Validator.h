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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef VALIDATOR_H
#define VALIDATOR_H

namespace hoot
{

/**
 * Interface for implementation by validators performing data validation
 */
class Validator
{
public:

  /**
   * @brief Enables validation on the validator.
   *
   * Some validators have dual purposes and have the need to have validation capabilities toggled.
   */
  virtual void enableValidation() = 0;

    /**
     * @brief Disables validation on the validator.
     *
     * Some validators have dual purposes and have the need to have validation capabilities toggled.
     */
  virtual void disableValidation() = 0;

  /**
    * @brief Returns a message with a description of validation errors
    */
  virtual QString getValidationErrorMessage() const = 0;

  /**
    * @brief Returns the total number of validation errors encountered
    */
  virtual int getNumValidationErrors() const = 0;

  /**
    * @brief Returns the total number of features validated
    */
  virtual int getNumFeaturesValidated() const = 0;
};

}

#endif // VALIDATOR_H
