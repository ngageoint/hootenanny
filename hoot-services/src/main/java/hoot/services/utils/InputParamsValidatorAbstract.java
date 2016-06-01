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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.utils;

import java.util.Locale;


/**
 * Abstract class for validating a set of input parameters to a method
 * 
 * Inheritors are responsible for transforming input parameters into strings,
 * which are then passed to the validateAndParseParamValueString method.
 */
public abstract class InputParamsValidatorAbstract {

    /**
     * Validates a string input parameter
     * 
     * @param paramValueStr
     *            parameter value
     * @param type
     *            type of the parameter
     * @param rangeMin
     *            minimum allowable value for numeric parameters
     * @param rangeMax
     *            maximum allowable value for numeric parameters
     * @return a parameter value
     * @throws Exception
     */
    protected Object validateAndParseParamValueString(final String paramValueStr, final String name, final Object type,
            final Object rangeMin, final Object rangeMax) throws Exception {
        Object paramValue = null;
        if (type instanceof String) {
            paramValue = paramValueStr;
        }
        else if (type instanceof Boolean) {
            if (!paramValueStr.toLowerCase(Locale.ENGLISH).equals("false")
                    && !paramValueStr.toLowerCase(Locale.ENGLISH).equals("true")) {
                throw new Exception(
                        "Invalid input parameter: " + name + " sent to: " + ReflectUtils.getCallingClassName());
            }
            paramValue = Boolean.parseBoolean(paramValueStr);
        }
        else {
            boolean errorParsingVal = false;
            boolean valueBelowMin = false;
            boolean valueAboveMax = false;
            if (type instanceof Long) {
                try {
                    paramValue = Long.parseLong(paramValueStr);
                }
                catch (NumberFormatException e) {
                    errorParsingVal = true;
                }

                if (!errorParsingVal && paramValue != null) {
                    if (rangeMin != null && ((Long) paramValue < (Long) rangeMin)) {
                        valueBelowMin = true;
                    }
                    if (rangeMax != null && ((Long) paramValue > (Long) rangeMax)) {
                        valueAboveMax = true;
                    }
                }
            }
            else if (type instanceof Integer) {
                try {
                    paramValue = Integer.parseInt(paramValueStr);
                }
                catch (NumberFormatException e) {
                    errorParsingVal = true;
                }

                if (!errorParsingVal && paramValue != null) {
                    if (rangeMin != null && ((Integer) paramValue < (Integer) rangeMin)) {
                        valueBelowMin = true;
                    }
                    if (rangeMax != null && ((Integer) paramValue > (Integer) rangeMax)) {
                        valueAboveMax = true;
                    }
                }
            }
            else if (type instanceof Double) {
                try {
                    paramValue = Double.parseDouble(paramValueStr);
                }
                catch (NumberFormatException e) {
                    errorParsingVal = true;
                }

                if (!errorParsingVal && paramValue != null) {
                    if (rangeMin != null && ((Double) paramValue < (Double) rangeMin)) {
                        valueBelowMin = true;
                    }
                    if (rangeMax != null && ((Double) paramValue > (Double) rangeMax)) {
                        valueAboveMax = true;
                    }
                }
            }

            if (errorParsingVal) {
                throw new Exception(
                        "Invalid input parameter: " + name + " sent to " + ReflectUtils.getCallingClassName());
            }
            if (valueBelowMin) {
                throw new Exception("Invalid input parameter: " + name + " sent to "
                        + ReflectUtils.getCallingClassName()
                        + " is out of the allowable range.  Minimum allowed value = " + String.valueOf(rangeMin));
            }
            if (valueAboveMax) {
                throw new Exception("Invalid input parameter: " + name + " sent to "
                        + ReflectUtils.getCallingClassName()
                        + " is out of the allowable range.  Maximum allowed value = " + String.valueOf(rangeMax));
            }
        }

        return paramValue;
    }
}
