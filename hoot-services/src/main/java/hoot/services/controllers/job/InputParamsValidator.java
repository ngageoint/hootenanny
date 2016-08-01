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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job;

import java.util.Map;

import org.apache.commons.lang3.StringUtils;

import hoot.services.geo.BoundingBox;
import hoot.services.utils.ReflectUtils;


/**
 * Validates Jersey service input parameters
 */
public class InputParamsValidator extends InputParamsValidatorAbstract {
    private final Map<String, Object> inputParams;

    public InputParamsValidator(Map<String, Object> inputParams) {
        this.inputParams = inputParams;
    }

    /**
     * Parses and validates a set of input parameters to a Jersey service method
     * 
     * @param name
     *            name of the parameter
     * @param type
     *            type of the parameter
     * @param rangeMin
     *            minimum allowable value for numeric parameters
     * @param rangeMax
     *            maximum allowable value for numeric parameters
     * @param optional
     *            if true; the parameter is considered optional and must not be
     *            present
     * @param defaultValue
     *            a default value to assign to the parameter, if it has no value
     * @return a parameter value
     */
    public Object validateAndParseInputParam(String name, Object type, Object rangeMin,
            Object rangeMax, boolean optional, Object defaultValue) {

        // special case
        if (name.equals("geospatialBounds")) {
            if ((inputParams.get("geospatialBounds") == null) && (defaultValue != null)) {
                return new BoundingBox((String) defaultValue);
            }
            return new BoundingBox((String) inputParams.get("geospatialBounds"));
        }

        Object param = inputParams.get(name);
        if (((param == null) || (StringUtils.trimToNull(String.valueOf(param).trim()) == null)) && !optional) {
            throw new IllegalArgumentException("Invalid input parameter value.  Required parameter: "
                    + name + " not sent to: " + ReflectUtils.getCallingClassName());
        }

        if ((param == null) && (defaultValue != null)) {
            if (!type.getClass().equals(defaultValue.getClass())) {
                throw new IllegalArgumentException("Invalid input parameter value.  Mismatching input parameter type: "
                        + type + " and default value type: " + defaultValue + "for parameter: "
                        + name + " sent to: " + ReflectUtils.getCallingClassName());
            }
            return defaultValue;
        }

        if (param != null) {
            if (StringUtils.trimToNull(String.valueOf(param).trim()) == null) {
                throw new IllegalArgumentException("Invalid input parameter: " + name +
                        " sent to: " + ReflectUtils.getCallingClassName());
            }

            return validateAndParseParamValueString(String.valueOf(param).trim(), name, type, rangeMin, rangeMax);
        }

        Object paramValue = null;

        return paramValue;
    }
}
