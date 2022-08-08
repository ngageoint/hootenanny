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
 * @copyright Copyright (C) 2018, 2021, 2022 Maxar (http://www.maxar.com/)
 */

package hoot.services.language.joshua;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Reads a hoot managed config file that determines which Joshua services to launch.  
 *
 * @article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015} }
 */
public final class JoshuaServicesConfigReader
{
  private static final Logger logger = LoggerFactory.getLogger(JoshuaServicesConfigReader.class);

  public JoshuaServicesConfigReader()
  {
  }

  /**
   * Reads a Joshua services configuration input stream and returns the information
   * 
   * @param configStrm configuration file input stream
   */
  public static JoshuaServiceInfo[] readConfig(InputStream configStrm) throws Exception
  { 
    String line = null;
    List<JoshuaServiceInfo> services = new ArrayList<JoshuaServiceInfo>();
    BufferedReader reader = new BufferedReader(new InputStreamReader(configStrm));                         
    while ((line = reader.readLine()) != null) 
    {    
      line = line.trim();
      if (!line.isEmpty() && !line.startsWith("#"))
      {  
        String[] lineParts = line.split("\t");
        if (lineParts.length != 3)
        {
          throw new IOException("Invalid Joshua services config entry: " + line);
        }

        JoshuaServiceInfo service = new JoshuaServiceInfo();
        service.setLanguageCode(lineParts[0]);
        service.setLanguagePackPath(lineParts[1]);
        service.setPort(Integer.parseInt(lineParts[2]));
        services.add(service);
      }
    }
    return services.toArray(new JoshuaServiceInfo[]{});
  }
}
