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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import java.lang.reflect.Constructor;
import java.util.HashMap;
import java.util.Map;

import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class ExportCommandFactoryTest {

    @Ignore
    @Test
    @Category(UnitTest.class)
    public void testCreateOSCCommand() {
        String jobId = "1";
        Map<String, String> paramMap = new HashMap<>();

        ExportCommandFactory exportCommandFactory = new ExportCommandFactory();
        Constructor[] constructors = ExportOSCCommand.class.getDeclaredConstructors();

        exportCommandFactory.build(jobId, paramMap, "debug", ExportOSCCommand.class, this.getClass());
    }
}