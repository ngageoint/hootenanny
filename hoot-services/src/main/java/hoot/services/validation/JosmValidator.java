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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.validation;

import java.util.List;
import java.util.Map;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Collection;
import java.lang.Exception;

import org.openstreetmap.josm.data.osm.OsmPrimitive;
import org.openstreetmap.josm.data.osm.Node;
import org.openstreetmap.josm.data.validation.OsmValidator;
import org.openstreetmap.josm.data.validation.Test;
import org.openstreetmap.josm.data.validation.TestError;
import org.openstreetmap.josm.tools.Logging;

/**
 * 
 */
public class JosmValidator
{
  public JosmValidator()
  {
    //System.out.println("test1");
  }

  public long getBlankNodeIdTest()
  {
    return (new Node(1)).getId();
  }

  public List<String> getAvailableValidators()
  {
    List<String> testNames = new ArrayList<String>();
    try
    {
      Logging.setLogLevel(Logging.LEVEL_DEBUG);
      OsmValidator.initializeTests(); // dying here
      OsmValidator.initializeErrorLayer();

      Collection<Test> validationTests = OsmValidator.getEnabledTests(false);
      for (Test validationTest : validationTests)
      {
        testNames.add(validationTest.getName());
      }
    }
    catch (Exception e)
    {
      System.out.println(e.getMessage());
      throw e;
    }

    return testNames;
  }

  public void setValidatorsToUse(List<String> validators)
  {
    validatorsToUse = validators;
  }

  public Map<String, String> validate(List<OsmPrimitive> features)
  {
    // element id string to validation msg
    return new HashMap<String, String>();
  }

  public Map<String, OsmPrimitive> validateAndFix(List<OsmPrimitive> features)
  {
    // element id string;fix msg to fixed element
    return new HashMap<String, OsmPrimitive>();
  }

  private List<String> validatorsToUse;
}
