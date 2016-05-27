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

import java.lang.reflect.InvocationTargetException;
import java.sql.Connection;

import org.apache.commons.lang3.ClassUtils;
import org.apache.commons.lang3.reflect.ConstructorUtils;

import com.mysema.query.Tuple;

import hoot.services.models.osm.Element.ElementType;


/**
 * Factory for creating the different OSM element types
 */
public class ElementFactory {
    private ElementFactory() {
    }

    private static ElementFactory instance;

    /**
     * Singleton instance access
     *
     * @return an ElementFactory
     */
    public static ElementFactory getInstance() {
        if (instance == null) {
            instance = new ElementFactory();
        }
        return instance;
    }

    /**
     * Creates an element
     *
     * @param elementType
     *            the type of element to create
     * @param conn
     *            JDBC Connection
     * @return an element
     * @throws ClassNotFoundException
     * @throws IllegalAccessException
     * @throws InstantiationException
     * @throws InvocationTargetException
     * @throws NoSuchMethodException
     */
    public Element create(final long mapId, final ElementType elementType, Connection conn)
            throws InstantiationException, IllegalAccessException, ClassNotFoundException, NoSuchMethodException,
            InvocationTargetException {
        return (Element) ConstructorUtils.invokeConstructor(
                Class.forName(ClassUtils.getPackageName(ElementFactory.class) + "." + elementType.toString()),
                new Object[] { new Long(mapId), conn }, new Class<?>[] { Long.class, Connection.class });
    }

    /**
     * Creates an element from a new element record
     *
     * @param elementType
     *            the type of element to create
     * @param record
     *            record to associate with the element
     * @param conn
     *            JDBC Connection
     * @return an element
     * @throws ClassNotFoundException
     * @throws InstantiationException
     * @throws InvocationTargetException
     * @throws IllegalAccessException
     * @throws NoSuchMethodException
     */
    public Element create(final ElementType elementType, final Object record, Connection conn, final long mapId)
            throws NoSuchMethodException, IllegalAccessException, InvocationTargetException, InstantiationException,
            ClassNotFoundException, Exception {
        Object oRec = record;
        Object oElem = oRec;
        if (oRec instanceof Tuple) {
            // This was forced since we are using reflection which need to be
            // refactored to something more solid.

            Tuple tRec = (Tuple) oRec;
            Object[] tRecs = tRec.toArray();
            if (tRecs.length > 0) {
                // assume first record is good.
                oElem = tRecs[0];
            }
            else {
                throw new Exception(
                        "Bad Record type. Tuple is empty. Please make sure the first object is tuple is DTO "
                                + "that supports setVersion.");
            }
        }
        Long oMapId = new Long(mapId);
        ClassUtils.getPackageName(ElementFactory.class);
        elementType.toString();
        return (Element) ConstructorUtils.invokeConstructor(
                Class.forName(ClassUtils.getPackageName(ElementFactory.class) + "." + elementType.toString()),
                new Object[] { oMapId, conn, oElem },
                new Class<?>[] { Long.class, Connection.class, oElem.getClass() });
    }
}
