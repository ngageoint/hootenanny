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
package hoot.services.utils;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;


/**
 * String utilities
 */
public class StringUtils {
    /**
     * URI encodes a string for Javascript consumption; client should call
     * decodeURIComponent to decode
     * 
     * @param str
     *            string to encode
     * @return a URI encoded string safe for Javascript consumption
     * @throws UnsupportedEncodingException
     */
    public static String encodeURIComponentForJavaScript(final String str) throws UnsupportedEncodingException {
        return URLEncoder.encode(str, "UTF-8").replaceAll("\\+", "%20").replaceAll("\\%21", "!")
                .replaceAll("\\%27", "'").replaceAll("\\%28", "(").replaceAll("\\%29", ")").replaceAll("\\%7E", "~")
                .replaceAll("\\%3B", ";");
    }
}
