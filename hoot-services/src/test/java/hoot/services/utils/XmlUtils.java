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
 * @copyright Copyright (C) 2015, 2016, 2017, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.utils;

import java.io.IOException;
import java.io.StringWriter;

import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.NodeList;


/**
 * Various xml utilities
 */
public final class XmlUtils {
    private XmlUtils() {
    }

    /**
     * Creates an XPATH instance for querying with
     * 
     * @return an XPATH instance
     */
    public static XPath createXPath() {
        return XPathFactory.newInstance().newXPath();
    }

    /**
     * Converts an XML node list to a string
     * 
     * @param nodeList
     * @return a string representation of the node list
     * @throws TransformerFactoryConfigurationError
     * @throws TransformerException
     * @throws IOException
     */
    public static String nodeListToString(NodeList nodeList)
            throws TransformerFactoryConfigurationError, TransformerException, IOException {
        StringBuilder result = new StringBuilder();
        for (int i = 0; i < nodeList.getLength(); i++) {
            try (StringWriter sw = new StringWriter()) {
                Transformer serializer = TransformerFactory.newInstance().newTransformer();
                serializer.transform(new DOMSource(nodeList.item(i)), new StreamResult(sw));
                result.append(sw);
            }
        }
        return result.toString();
    }
}
