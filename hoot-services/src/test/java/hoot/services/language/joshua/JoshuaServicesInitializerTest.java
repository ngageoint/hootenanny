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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

package hoot.services.language.joshua;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static hoot.services.HootProperties.*;

import java.util.Map;
import java.lang.reflect.Method;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;

import org.apache.commons.exec.CommandLine;

import hoot.services.UnitTest;

@RunWith(PowerMockRunner.class)
@PrepareForTest({ JoshuaServicesConfigReader.class })
public class JoshuaServicesInitializerTest  
{
  //@Test
  //@Category(UnitTest.class)
  public void testGetCommand() throws Exception
  {
    JoshuaServiceInfo serviceInfo = new JoshuaServiceInfo();
    serviceInfo.setLanguagePackPath("/my/path");
    serviceInfo.setPort(1);
    serviceInfo.setLanguageCode("de");
    
    Method getCommandMethod = 
     JoshuaServicesInitializer.class.getDeclaredMethod("getProcessExecCommand", JoshuaServiceInfo.class);
    getCommandMethod.setAccessible(true);
    CommandLine command = (CommandLine)getCommandMethod.invoke(null, serviceInfo);

    Assert.assertEquals(
      "java -mx " + JOSHUA_MAX_MEMORY + " -Dfile.encoding=utf8 -Djava.library.path=./lib -cp " + 
      serviceInfo.getLanguagePackPath() + "/target/" + JOSHUA_LIBRARY + 
      " org.apache.joshua.decoder.JoshuaDecoder -c " + serviceInfo.getLanguagePackPath() + 
      "/joshua.config -v 1 -server-port 1 -server-type tcp", 
      command.toString()); 
  }

  //@Test
  //@Category(UnitTest.class)
  public void testReadConfig() throws Exception
  {
    JoshuaServiceInfo[] serviceInfos = new JoshuaServiceInfo[2];
    JoshuaServiceInfo serviceInfo1 = new JoshuaServiceInfo();
    serviceInfo1.setLanguagePackPath("/my/path1");
    serviceInfo1.setPort(1);
    serviceInfo1.setLanguageCode("de");
    serviceInfos[0] = serviceInfo1;
    JoshuaServiceInfo serviceInfo2 = new JoshuaServiceInfo();
    serviceInfo2.setLanguagePackPath("/my/path2");
    serviceInfo2.setPort(2);
    serviceInfo2.setLanguageCode("es");
    serviceInfos[1] = serviceInfo2;

    PowerMockito.mockStatic(JoshuaServicesConfigReader.class);
    PowerMockito.when(JoshuaServicesConfigReader.readConfig(null)).thenReturn(serviceInfos);
    
    Method readConfigMethod = 
     JoshuaServicesInitializer.class.getDeclaredMethod("readServicesConfig", null);
    readConfigMethod.setAccessible(true);
    Map<String, JoshuaServiceInfo> services = 
      (Map<String, JoshuaServiceInfo>)readConfigMethod.invoke(null, null);

    Assert.assertEquals(2, services.size());
    
    Assert.assertEquals("de", services.get("de").getLanguageCode());
    Assert.assertEquals("/my/path1", services.get("de").getLanguagePackPath());
    Assert.assertEquals(1, services.get("de").getPort());

    Assert.assertEquals("es", services.get("es").getLanguageCode());
    Assert.assertEquals("/my/path2", services.get("es").getLanguagePackPath());
    Assert.assertEquals(2, services.get("es").getPort());
  }
}
