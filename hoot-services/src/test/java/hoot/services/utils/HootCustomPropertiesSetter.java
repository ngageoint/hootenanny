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
 * @copyright Copyright (C) 2016, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.utils;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;

import hoot.services.HootProperties;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public final class HootCustomPropertiesSetter {

    private static final Logger logger = LoggerFactory.getLogger(HootCustomPropertiesSetter.class);

    private HootCustomPropertiesSetter() {}

    public static void setProperty(String key, Object value) {
        try {
            setProperty(key, value, HootProperties.class);
        }
        catch (Exception e) {
            //logger.error(e.getMessage());
            throw new RuntimeException("Error setting " + key + " to " + value, e);
        }
    }

    public static void setProperty(String key, Object value, Class<?> clazz) {
        try {
            Field field = clazz.getDeclaredField(key);
            field.setAccessible(true);

            Field modifiersField = Field.class.getDeclaredField("modifiers");
            modifiersField.setAccessible(true);
            modifiersField.setInt(field, field.getModifiers() & ~Modifier.FINAL);

            field.set(null, value);
        }
        catch (Exception e) {
            throw new RuntimeException("Error setting " + key + " to " + value + " for class " + clazz.getName(), e);
        }
    }
}
