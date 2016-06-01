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
package hoot.services.validators.osm;

import org.apache.commons.lang3.StringUtils;
import org.apache.xpath.XPathAPI;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;

import hoot.services.utils.XmlDocumentBuilder;


/**
 * OSM changeset validator
 */
public class ChangesetUploadXmlValidator {
    private static final Logger log = LoggerFactory.getLogger(ChangesetUploadXmlValidator.class);

    /**
     * Validates a changeset
     * 
     * @param changesetXml
     *            an OSM changeset for reviewed items
     */
    public Document parseAndValidate(final String changesetXml) throws Exception {
        Document changesetDiffDoc = null;
        try {
            log.debug("Parsing changeset diff XML: " + StringUtils.abbreviate(changesetXml, 1000));
            changesetDiffDoc = XmlDocumentBuilder.parse(changesetXml);
        }
        catch (Exception e) {
            throw new Exception("Error parsing changeset diff data: " + changesetXml + " (" + e.getMessage() + ")");
        }

        if (XPathAPI.selectNodeList(changesetDiffDoc, "//osmChange").getLength() > 1) {
            throw new Exception("Only one changeset may be uploaded at a time.");
        }

        if (!changesetHasElements(changesetDiffDoc)) {
            throw new Exception("No items in uploaded changeset.");
        }

        return changesetDiffDoc;
    }

    /**
     * Determines whether a changeset DOM has any elements in it. (Strangely, I
     * couldn't figure out how to do this correctly with XPath).
     * 
     * @param changesetDiffDoc
     *            the changeset to check for elements
     * @return true if the changeset has elements; false otherwise
     */
    public static boolean changesetHasElements(final Document changesetDiffDoc) {
        return changesetDiffDoc.getElementsByTagName("node").getLength()
                + changesetDiffDoc.getElementsByTagName("way").getLength()
                + changesetDiffDoc.getElementsByTagName("relation").getLength() > 0;
    }
}
