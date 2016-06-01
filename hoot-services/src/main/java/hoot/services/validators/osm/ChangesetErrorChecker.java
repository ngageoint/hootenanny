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

import java.sql.Connection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import org.apache.xpath.XPathAPI;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import com.mysema.query.sql.SQLQuery;

import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.EntityChangeType;
import hoot.services.db2.CurrentNodes;
import hoot.services.db2.QCurrentNodes;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.ElementFactory;


/**
 *
 */
public class ChangesetErrorChecker {
    private static final Logger log = LoggerFactory.getLogger(ChangesetErrorChecker.class);

    protected static final QCurrentNodes currentNodes = QCurrentNodes.currentNodes;

    private Document changesetDoc;
    private long mapId;
    private Connection dbConn;

    public ChangesetErrorChecker(Document changesetDoc, long mapId, Connection dbConn) {
        this.changesetDoc = changesetDoc;
        this.mapId = mapId;
        this.dbConn = dbConn;
    }

    /**
     * @throws Exception
     */
    public void checkForVersionErrors() throws Exception {
        log.debug("Checking for element version errors...");

        for (ElementType elementType : ElementType.values()) {
            if (elementType != ElementType.Changeset) {
                for (EntityChangeType entityChangeType : EntityChangeType.values()) {
                    NodeList elementXmlNodes = XPathAPI.selectNodeList(changesetDoc, "//osmChange/"
                            + entityChangeType.toString().toLowerCase() + "/" + elementType.toString().toLowerCase());

                    Map<Long, Long> elementIdsToVersionsFromChangeset = new HashMap<>();
                    for (int i = 0; i < elementXmlNodes.getLength(); i++) {
                        long id = Long
                                .parseLong(elementXmlNodes.item(i).getAttributes().getNamedItem("id").getNodeValue());
                        long parsedVersion = Long.parseLong(
                                elementXmlNodes.item(i).getAttributes().getNamedItem("version").getNodeValue());
                        if (entityChangeType == EntityChangeType.CREATE) {
                            if (parsedVersion != 0) {
                                throw new Exception("Invalid version: " + parsedVersion + " specified for created "
                                        + elementType + " with ID: " + id + "; expected version 0.");
                            }
                        }
                        else {
                            elementIdsToVersionsFromChangeset.put(id, parsedVersion);
                        }
                    }

                    if ((entityChangeType != EntityChangeType.CREATE)
                            && (!elementIdsToVersionsFromChangeset.isEmpty())) {
                        Element prototype = ElementFactory.getInstance().create(mapId, elementType, dbConn);
                        Map<Long, Long> elementIdsToVersionsFromDb = new SQLQuery(dbConn,
                                DbUtils.getConfiguration(mapId))
                                        .from(prototype.getElementTable())
                                        .where(prototype.getElementIdField()
                                                .in(elementIdsToVersionsFromChangeset.keySet()))
                                        .map(prototype.getElementIdField(), prototype.getElementVersionField());

                        if (!elementIdsToVersionsFromDb.equals(elementIdsToVersionsFromChangeset)) {
                            throw new Exception("Invalid version specified for element(s).");
                        }
                    }
                }
            }
        }
    }

    /**
     * @throws Exception
     *             //TODO: is this check actually necessary?
     */
    public void checkForElementVisibilityErrors() throws Exception {
        log.debug("Checking for element visibility errors...");

        // if a child element is referenced and is invisible, then fail.
        // elements are created visible
        // by default with a create changeset, which is why we can skip checking
        // negative id's (elements
        // being created). we're also skipping checking anything in the delete
        // sections, b/c since the
        // elements are being deleted, they're visibility no longer is
        // important.

        for (ElementType elementType : ElementType.values()) {
            if (elementType != ElementType.Changeset) {
                Set<Long> relationMemberIds = new HashSet<>();
                // I know there's a way to use the '|' operator in the xpath
                // query to make this simpler...
                // its just not working
                for (EntityChangeType entityChangeType : EntityChangeType.values()) {
                    if (entityChangeType != EntityChangeType.DELETE) {
                        NodeList relationMemberIdXmlNodes = XPathAPI.selectNodeList(changesetDoc,
                                "//osmChange/" + entityChangeType.toString().toLowerCase()
                                        + "/relation/member[@type = \"" + elementType.toString().toLowerCase() + "\"]");

                        for (int i = 0; i < relationMemberIdXmlNodes.getLength(); i++) {
                            // don't need to check for empty id here, b/c
                            // previous checking would have already errored out
                            // for it
                            long id = Long.parseLong(relationMemberIdXmlNodes.item(i).getAttributes()
                                    .getNamedItem("ref").getNodeValue());

                            if (id > 0) {
                                relationMemberIds.add(id);
                            }
                        }
                    }

                }

                if (!Element.allElementsVisible(mapId, elementType, relationMemberIds, dbConn)) {
                    throw new Exception(elementType + " member(s) aren't visible for relation.");
                }
            }
        }

        Set<Long> wayNodeIds = new HashSet<>();
        for (EntityChangeType entityChangeType : EntityChangeType.values()) {
            if (entityChangeType != EntityChangeType.DELETE) {
                NodeList wayNodeIdXmlNodes = XPathAPI.selectNodeList(changesetDoc,
                        "//osmChange/" + entityChangeType.toString().toLowerCase() + "/way/nd");

                for (int i = 0; i < wayNodeIdXmlNodes.getLength(); i++) {
                    // don't need to check for empty id here, b/c previous
                    // checking would have already errored out for it
                    long id = Long
                            .parseLong(wayNodeIdXmlNodes.item(i).getAttributes().getNamedItem("ref").getNodeValue());

                    if (id > 0) {
                        wayNodeIds.add(id);
                    }
                }
            }
        }

        if (!Element.allElementsVisible(mapId, ElementType.Node, wayNodeIds, dbConn)) {
            throw new Exception("Way node(s) aren't visible for way.");
        }
    }

    public Map<Long, CurrentNodes> checkForElementExistenceErrors() throws Exception {
        log.debug("Checking for element existence errors...");

        // if an element is referenced (besides in its own create change) and
        // doesn't exist in the db, then fail

        Map<ElementType, Set<Long>> elementTypesToElementIds = new HashMap<>();
        for (ElementType elementType : ElementType.values()) {
            if (elementType != ElementType.Changeset) {
                elementTypesToElementIds.put(elementType, new HashSet<Long>());
            }
        }

        String emptyIdErrorMsg = "Element in changeset has empty ID.";

        // check relation members
        for (ElementType elementType : ElementType.values()) {
            if (elementType != ElementType.Changeset) {
                NodeList relationMemberIdXmlNodes = XPathAPI.selectNodeList(changesetDoc,
                        "//osmChange/*/relation/member[@type = '" + elementType.toString().toLowerCase() + "']");

                for (int i = 0; i < relationMemberIdXmlNodes.getLength(); i++) {
                    long id;
                    try {
                        // log.debug(relationMemberIdXmlNodes.item(i).getAttributes().getNamedItem("ref").getNodeValue());
                        id = Long.parseLong(
                                relationMemberIdXmlNodes.item(i).getAttributes().getNamedItem("ref").getNodeValue());
                    }
                    catch (NumberFormatException | NullPointerException e) {
                        throw new Exception(emptyIdErrorMsg);
                    }

                    if (id > 0) {
                        elementTypesToElementIds.get(elementType).add(id);
                    }
                }
            }
        }

        // check way nodes
        NodeList wayNodeIdXmlNodes = XPathAPI.selectNodeList(changesetDoc, "//osmChange/*/way/nd/@ref");
        for (int i = 0; i < wayNodeIdXmlNodes.getLength(); i++) {
            long id;
            try {
                id = Long.parseLong(wayNodeIdXmlNodes.item(i).getNodeValue());
            }
            catch (NumberFormatException | NullPointerException e) {
                throw new Exception(emptyIdErrorMsg);
            }

            if (id > 0) {
                elementTypesToElementIds.get(ElementType.Node).add(id);
            }
        }

        // check top level elements
        for (EntityChangeType entityChangeType : EntityChangeType.values()) {
            if (entityChangeType != EntityChangeType.CREATE) {

                for (ElementType elementType : ElementType.values()) {
                    if (elementType != ElementType.Changeset) {
                        NodeList elementIdXmlNodes = XPathAPI.selectNodeList(changesetDoc,
                                "//osmChange/" + entityChangeType.toString().toLowerCase() + "/"
                                        + elementType.toString().toLowerCase() + "/@id");
                        // log.debug(String.valueOf(nodeIdXmlNodes.getLength()));
                        for (int i = 0; i < elementIdXmlNodes.getLength(); i++) {
                            long id;
                            try {
                                id = Long.parseLong(elementIdXmlNodes.item(i).getNodeValue());
                            }
                            catch (NumberFormatException | NullPointerException e) {
                                throw new Exception(emptyIdErrorMsg);
                            }
                            if (id > 0) {
                                elementTypesToElementIds.get(elementType).add(id);
                            }
                        }
                    }
                }
            }
        }

        for (ElementType elementType : ElementType.values()) {
            if (elementType != ElementType.Changeset) {
                if (!Element.allElementsExist(mapId, elementType, elementTypesToElementIds.get(elementType), dbConn)) {
                    // TODO: list the id's and types of the elements that don't
                    // exist
                    throw new Exception("Element(s) being referenced don't exist.");
                }
            }
        }

        if (!elementTypesToElementIds.get(ElementType.Node).isEmpty()) {
            return new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(currentNodes)
                    .where(currentNodes.id.in(elementTypesToElementIds.get(ElementType.Node)))
                    .map(currentNodes.id, currentNodes);
        }

        return null;
    }
}
