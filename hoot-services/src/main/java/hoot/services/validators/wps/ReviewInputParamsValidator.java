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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.validators.wps;

import hoot.services.geo.BoundingBox;
import hoot.services.utils.InputParamsValidatorAbstract;
import hoot.services.utils.ReflectUtils;

import org.apache.commons.lang3.StringUtils;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.input.BoundingBoxInput;
import org.deegree.services.wps.input.LiteralInput;
import org.deegree.services.wps.input.ProcessletInput;


/**
 * Validates review deegree WPS service input parameters
 */
public class ReviewInputParamsValidator extends InputParamsValidatorAbstract
{
  private ProcessletInputs inputParams;
  
  public ReviewInputParamsValidator(final ProcessletInputs inputParams)
  {
    this.inputParams = inputParams;
  }
  
  /**
   * Parses and validates a set of input parameters to a deegree WPS review service method
   * 
   * @param name name of the parameter
   * @param type type of the parameter
   * @param rangeMin minimum allowable value for numeric parameters
   * @param rangeMax maximum allowable value for numeric parameters
   * @param optional if true; the parameter is considered optional and must not be present
   * @param defaultValue a default value to assign to the parameter, if it has no value
   * @return a parameter value
   * @throws Exception
   */
  public Object validateAndParseInputParam(final String name, final Object type, 
    final Object rangeMin, final Object rangeMax, final boolean optional, final Object defaultValue) 
    throws Exception
  {
    Object paramValue = null;
    
    //special case
    if (name.equals("geospatialBounds"))
    {
      if (inputParams.getParameter("geospatialBounds") == null && defaultValue != null)
      {
        return new BoundingBox((String)defaultValue);
      }
      else
      {
        return new BoundingBox((BoundingBoxInput)inputParams.getParameter("geospatialBounds"));
      }
    }
    
    ProcessletInput param = inputParams.getParameter(name);
    if ((param == null || ((LiteralInput)param).getValue() == null || 
          StringUtils.trimToNull(((LiteralInput)param).getValue().trim()) == null) && 
        !optional)
    {
      throw new Exception(
        "Invalid input parameter value.  Required parameter: " + name + " not sent to: " + 
        ReflectUtils.getCallingClassName());
    }
    
    if (param == null && defaultValue != null)
    {
      if (!type.getClass().getName().equals(defaultValue.getClass().getName()))
      {
        throw new Exception(
          "Invalid input parameter value.  Mismatching input parameter type: " + type.toString() + 
          " and default value type: " + defaultValue.toString() + "for parameter: " + name + 
          " sent to: " + ReflectUtils.getCallingClassName());
      }
      return defaultValue;
    }
    
    if (param != null)
    {
      if (((LiteralInput)param).getValue() == null || 
          StringUtils.trimToNull(((LiteralInput)param).getValue().trim()) == null)
      {
        throw new Exception(
          "Invalid input parameter: " + name + " sent to: " + ReflectUtils.getCallingClassName());
      }
      
      return validateAndParseParamValueString(
        ((LiteralInput)param).getValue().trim(),name, type, rangeMin, rangeMax);
    }
    
    return paramValue;
  }
}
