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
 * @copyright Copyright (C) 2018, 2021 Maxar (http://www.maxar.com/)
 */

package hoot.services.language.joshua;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static hoot.services.HootProperties.*;

import java.util.Map;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;

public class JoshuaServicesConfigReaderTest  
{
  @Test
  @Category(UnitTest.class)
  public void testRead() throws Exception
  {
    JoshuaServiceInfo[] serviceInfos = 
      JoshuaServicesConfigReader.readConfig(
        JoshuaServicesConfigReaderTest.class.getClassLoader().getResourceAsStream(
          "hoot/services/language/joshua/joshuaServices"));
    
    Assert.assertEquals(2, serviceInfos.length);
    
    Assert.assertEquals("de", serviceInfos[0].getLanguageCode());
    Assert.assertEquals("/my/path1", serviceInfos[0].getLanguagePackPath());
    Assert.assertEquals(1, serviceInfos[0].getPort());

    Assert.assertEquals("es", serviceInfos[1].getLanguageCode());
    Assert.assertEquals("/my/path2", serviceInfos[1].getLanguagePackPath());
    Assert.assertEquals(2, serviceInfos[1].getPort());
  }
}
