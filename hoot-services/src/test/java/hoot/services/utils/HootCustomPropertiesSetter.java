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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.utils;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.Properties;

import hoot.services.HootProperties;


/**
 * Created by dmylov on 5/10/16.
 */
public class HootCustomPropertiesSetter {

    public static void setProperties(Properties newProperties) throws Exception {
        Field field = HootProperties.class.getDeclaredField("properties");
        field.setAccessible(true);

        Field modifiersField = Field.class.getDeclaredField("modifiers");
        modifiersField.setAccessible(true);
        modifiersField.setInt(field, field.getModifiers() & ~Modifier.FINAL);

        field.set(null, newProperties);
    }

    public static void setProperty(String key, String value) throws Exception {
        Field propertiesField = HootProperties.class.getDeclaredField("properties");
        propertiesField.setAccessible(true);

        Properties properties = (Properties) propertiesField.get(null);
        properties.setProperty(key, value);
    }
}
