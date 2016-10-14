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
package hoot.services.models.osm;

import org.apache.commons.lang3.ClassUtils;
import org.apache.commons.lang3.reflect.ConstructorUtils;

import com.querydsl.core.Tuple;

import hoot.services.models.osm.Element.ElementType;


/**
 * Factory for creating the different OSM element types
 */
public final class ElementFactory {
    private ElementFactory() {}

    /**
     * Creates an element
     *
     * @param elementType
     *            the type of element to create
     * @return an element
     */
    public static Element create(long mapId, ElementType elementType) {
        try {
            return (Element) ConstructorUtils.invokeConstructor(
                    Class.forName(ClassUtils.getPackageName(ElementFactory.class) + "." + elementType),
                    new Object[] { Long.valueOf(mapId) }, new Class<?>[] { Long.class });
        }
        catch (Exception e) {
            throw new RuntimeException("Error creating " + elementType + " OSM element for map with id = " + mapId, e);
        }
    }

    /**
     * Creates an element from a new element record
     *
     * @param elementType
     *            the type of element to create
     * @param record
     *            record to associate with the element
     * @return an element
     */
    public static Element create(ElementType elementType, Object record, long mapId) {
        Object oElem = record;

        if (record instanceof Tuple) {
            // This was forced since we are using reflection which need to be
            // refactored to something more solid.

            Tuple tRec = (Tuple) record;
            Object[] tRecs = tRec.toArray();
            if (tRecs.length > 0) {
                // assume first record is good.
                oElem = tRecs[0];
            }
            else {
                throw new IllegalArgumentException(
                        "Bad Record type. Tuple is empty. Please make sure the first object is tuple is DTO "
                                + "that supports setVersion.");
            }
        }

        try {
            Long oMapId = mapId;
            return (Element) ConstructorUtils.invokeConstructor(
                Class.forName(ClassUtils.getPackageName(ElementFactory.class) + "." + elementType),
                new Object[] { oMapId, oElem },
                new Class<?>[] { Long.class, oElem.getClass() });
        }
        catch (Exception e) {
            throw new RuntimeException("Error creating " + elementType + " OSM element for map with id = " + mapId, e);
        }
    }
}
