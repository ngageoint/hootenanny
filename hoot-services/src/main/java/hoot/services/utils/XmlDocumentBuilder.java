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
import java.io.StringReader;
import java.io.StringWriter;
import java.io.Writer;

import javax.xml.XMLConstants;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.apache.xml.serialize.OutputFormat;
import org.apache.xml.serialize.XMLSerializer;
import org.w3c.dom.Document;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

//import javax.xml.transform.TransformerConfigurationException;
//import javax.xml.transform.TransformerFactory;


/**
 * General XML utilities
 */
@SuppressWarnings("deprecation")
public final class XmlDocumentBuilder {
    private XmlDocumentBuilder() {
    }

    /**
     * Creates a new XML DOM
     * 
     * @return XML document
     * @throws ParserConfigurationException
     */
    public static Document create() throws ParserConfigurationException {
        return getSecureDocBuilderFactory().newDocumentBuilder().newDocument();
    }

    /**
     * Parses an XML string into a DOM
     * 
     * @param xml
     *            an XML string
     * @return an XML DOM
     * @throws SAXException
     * @throws IOException
     * @throws ParserConfigurationException
     */
    public static Document parse(String xml) throws SAXException, IOException, ParserConfigurationException {
        DocumentBuilderFactory domFactory = getSecureDocBuilderFactory();
        // DocumentBuilderFactory domFactory = getNormalDocBuilderFactory();
        domFactory.setNamespaceAware(false);
        domFactory.setValidating(false);
        domFactory.setFeature("http://xml.org/sax/features/namespaces", false);
        domFactory.setFeature("http://xml.org/sax/features/validation", false);
        domFactory.setFeature("http://apache.org/xml/features/nonvalidating/load-dtd-grammar", false);
        domFactory.setFeature("http://apache.org/xml/features/nonvalidating/load-external-dtd", false);

        DocumentBuilder builder = domFactory.newDocumentBuilder();

        // Disable printing of messages to System.out and System.err
        builder.setErrorHandler(null);

        InputSource is = new InputSource();
        is.setCharacterStream(new StringReader(xml));

        // #6760: formerly line 132
        return builder.parse(is);
    }

    /**
     * Returns a string representation of an XML DOM
     * 
     * @param document
     *            an XML DOM
     * @return an XML string
     * @throws IOException
     */
    public static String toString(Document document) throws IOException {
        StringWriter writer = new StringWriter();
        write(document, writer);
        return writer.toString();
    }

    /**
     * Writes an XML DOM to a writer
     * 
     * @param document
     *            XML DOM to write
     * @param out
     *            writer to write the XML DOM to
     * @throws IOException
     *             if unable to write the XML DOM
     */
    public static void write(Document document, Writer out) throws IOException {
        // happy to replace this code w/ the non-deprecated code, but I couldn't
        // get the transformer approach to work.
        OutputFormat format = new OutputFormat(document);
        format.setIndenting(true);
        format.setIndent(2);
        XMLSerializer serializer = new XMLSerializer(out, format);
        serializer.serialize(document);
    }

    /**
     * Returns a secure DocumentBuilderFactory, as identified by HP Fortify
     * 
     * @return a DocumentBuilderFactory
     * @throws ParserConfigurationException
     */
    public static DocumentBuilderFactory getSecureDocBuilderFactory() throws ParserConfigurationException {
        DocumentBuilderFactory docBuilderFactory = DocumentBuilderFactory.newInstance();
        docBuilderFactory.setFeature(XMLConstants.FEATURE_SECURE_PROCESSING, true);
        docBuilderFactory.setFeature("http://xml.org/sax/features/external-general-entities", false);
        docBuilderFactory.setFeature("http://xml.org/sax/features/external-parameter-entities", false);
        docBuilderFactory.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
        return docBuilderFactory;
    }

    private static DocumentBuilderFactory getNormalDocBuilderFactory() {
        DocumentBuilderFactory docBuilderFactory = DocumentBuilderFactory.newInstance();
        return docBuilderFactory;
    }
}
