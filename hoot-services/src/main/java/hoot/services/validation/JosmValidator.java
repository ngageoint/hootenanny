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
import java.util.ArrayList;
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
 * TODO

 using delimited strings in place of containers here to keep the code simpler initially; will
 replace with containers later if performance dictates it
 */
public class JosmValidator
{
  /**
   * TODO
   */
  public JosmValidator()
  {
    Logging.setLogLevel(Logging.LEVEL_DEBUG);
    Preferences pref = Preferences.main();
    Config.setPreferencesInstance(pref);
  }

  /**
   * TODO
   */
  public String getAvailableValidators()
  {
    String testsInfo = "";

    try
    {
      Collection<Test> validationTests = OsmValidator.getTests();
      //System.out.println("validationTests size: " + validationTests.size());
      // for each loop was causing a segfault here (?)...odd
      Iterator<Test> iterator = validationTests.iterator();
      while (iterator.hasNext())
      {
        //System.out.println("value= " + iterator.next());
        Test validationTest = iterator.next();
        if (validationTest != null)
        {
          //System.out.println("name= " + validationTest.getName());
          String testName = validationTest.toString().split("@")[0];
          testsInfo += testName + "," + validationTest.getName() + ";";
        }
      }
    }
    catch (Exception e)
    {
      System.out.println(e.getMessage());
      throw e;
    }

    return testsInfo;
  }

  /**
   * TODO
   */
  public String validate(String validators, String featuresXml, bool fix)
  {
    // will try passing features around as xml for the first draft; if too slow can try
    // an OsmMap --> OsmPrimitive conversion

    assert(validators.contains(";");
    List<String> validatorsParts = validators.split(";");


    //Config.setBaseDirectoriesProvider(JosmBaseDirectories.getInstance());
    //Config.setUrlsProvider(JosmUrls.getInstance());

    //OsmValidator.initializeTests();
    //OsmValidator.initializeErrorLayer();
    //OsmValidator.getEnabledTests(false);

    // if not fix: <element id string>;<validation msg>
    // if fix: <element id string>;fix msg;<fixed element xml>
    return new ArrayList<String>();
  }
}
