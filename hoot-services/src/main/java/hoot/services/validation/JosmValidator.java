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
import java.util.Iterator;
import java.util.NoSuchElementException;

import org.openstreetmap.josm.data.osm.OsmPrimitive;
import org.openstreetmap.josm.data.osm.Node;
import org.openstreetmap.josm.data.validation.OsmValidator;
import org.openstreetmap.josm.data.validation.Test;
import org.openstreetmap.josm.data.validation.TestError;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.spi.preferences.Config;
import org.openstreetmap.josm.spi.preferences.Setting;
import org.openstreetmap.josm.data.Preferences;
import org.openstreetmap.josm.data.preferences.JosmBaseDirectories;
import org.openstreetmap.josm.data.preferences.JosmUrls;

/**
 * 
 */
public class JosmValidator
{
  public JosmValidator()
  {
    Logging.setLogLevel(Logging.LEVEL_DEBUG);
    Preferences pref = Preferences.main();
    Config.setPreferencesInstance(pref);
  }

  public long getBlankNodeIdTest()
  {
    return (new Node(1)).getId();
  }

  public List<String> getAvailableValidators()
  {
    // could use a map here instead, but trying to keep the complexity of the client code down
    // as much as possible
    List<String> testsInfo = new ArrayList<String>();
    try
    {
      Collection<Test> validationTests = OsmValidator.getTests();
      //System.out.println("validationTests size: " + validationTests.size());
      // for each loop was causing a segfault here (?)...oddd
      Iterator<Test> iterator = validationTests.iterator();
      try
      {
        while (iterator.hasNext())
        {
          //System.out.println("value= " + iterator.next());
          Test validationTest = iterator.next();
          if (validationTest != null)
          {
            //System.out.println("name= " + validationTest.getName());
            String testName = validationTest.toString().split("@")[0];
            testsInfo.add(testName + ";" + validationTest.getName());
          }
        }
      }
      catch (NoSuchElementException e)
      {
        System.out.println(e.getMessage());
      }
    }
    catch (Exception e)
    {
      System.out.println(e.getMessage());
      throw e;
    }

    //System.out.println("testsInfo size: " + testsInfo.size());
    return testsInfo;
  }

  public void setValidatorsToUse(List<String> validators)
  {
    validatorsToUse = validators;
  }

  public Map<String, String> validate(String featuresXml)
  {
    //Config.setBaseDirectoriesProvider(JosmBaseDirectories.getInstance());
    //Config.setUrlsProvider(JosmUrls.getInstance());

    //OsmValidator.initializeTests();
    //OsmValidator.initializeErrorLayer();
    //OsmValidator.getEnabledTests(false);

    // element id string to validation msg
    return new HashMap<String, String>();
  }

  public Map<String, OsmPrimitive> validateAndFix(String featuresXml)
  {
    // element id string;fix msg to fixed element
    return new HashMap<String, OsmPrimitive>();
  }

  private List<String> validatorsToUse;
}
