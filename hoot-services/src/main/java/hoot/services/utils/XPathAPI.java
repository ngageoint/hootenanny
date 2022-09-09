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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */

package hoot.services.utils;

import javax.xml.transform.TransformerException;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathFactory;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public final class XPathAPI {
    private static XPath xpath = XPathFactory.newInstance().newXPath();

    public static NodeList selectNodeList(Object xmlObject, String expression) throws TransformerException {
        try {
            return (NodeList) xpath.compile(expression).evaluate((Object) xmlObject, XPathConstants.NODESET);
        } catch (XPathExpressionException exception) {
            throw new TransformerException(exception);
        }
    }
    
    public static Node selectSingleNode(Object xmlObject, String path) throws TransformerException {
        NodeList nodeList = selectNodeList(xmlObject, path);
        return nodeList.getLength() > 0 ? nodeList.item(0) : null;
    }
}