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
package hoot.services.info;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

/**
 * Services build info file
 */
public class BuildInfo {
    private static Properties info;

    private BuildInfo() {
    }

    /**
     * Returns the build info for the services
     *
     * @return a set of build info properties
     * @throws IOException if unable to read from the file
     */
    public static synchronized Properties getInstance() throws IOException {
        if ((info == null) || (info.isEmpty())) {
            info = new Properties();
            try (InputStream buildInfoStrm = BuildInfo.class.getClassLoader().getResourceAsStream("build.info")) {
                info.load(buildInfoStrm);
            }
        }
        return info;
    }
}
