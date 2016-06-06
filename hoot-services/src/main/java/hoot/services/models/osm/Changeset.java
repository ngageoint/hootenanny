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

import java.io.IOException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.Timestamp;
import java.sql.Types;
import java.util.Calendar;
import java.util.Map;

import javax.ws.rs.core.Response.Status;
import javax.xml.transform.TransformerException;

import org.apache.xpath.XPathAPI;
import org.joda.time.DateTime;
import org.joda.time.Minutes;
import org.joda.time.Seconds;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;

import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLInsertClause;
import com.mysema.query.sql.dml.SQLUpdateClause;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db2.Changesets;
import hoot.services.db2.QChangesets;
import hoot.services.geo.BoundingBox;
import hoot.services.geo.GeoUtils;
import hoot.services.utils.ResourceErrorHandler;


/**
 * Represents the model of an OSM changeset
 */
public class Changeset extends Changesets {
    @SuppressWarnings("unused")
    private static final long serialVersionUID = 4011802505587120104L;

    private static final Logger log = LoggerFactory.getLogger(Changeset.class);
    private static final QChangesets changesets = QChangesets.changesets;
    private Connection conn;
    private long _mapId = -1;

    /**
     * Constructor
     *
     * @param id
     *            changeset ID
     * @param conn
     *            JDBC Connection
     * @throws IOException
     * @throws NumberFormatException
     */
    public Changeset(final long mapId, final long id, final Connection conn) throws NumberFormatException, IOException {
        _mapId = mapId;
        setId(id);
        this.conn = conn;
    }

    /**
     * Creates a new changeset
     *
     * @param changesetDoc
     *            changeset create XML
     * @param mapId
     *            ID of the map owning the changeset
     * @param userId
     *            ID of the user creating the changeset
     * @param dbConn
     *            JDBC Connection
     * @return ID of the created changeset
     * @throws Exception
     */
    public static long createChangeset(final Document changesetDoc, final long mapId, final long userId,
            Connection dbConn) throws Exception {
        log.debug("Creating changeset for map ID: " + mapId + "...");

        final long changesetId = Changeset.insertNew(mapId, userId, dbConn);
        if (changesetId == Long.MAX_VALUE || changesetId < 1) {
            throw new Exception("Invalid changeset ID: " + changesetId);
        }

        (new Changeset(mapId, changesetId, dbConn)).insertTags(mapId,
                XPathAPI.selectNodeList(changesetDoc, "//changeset/tag"), dbConn);

        log.debug("Created changeset for with ID: " + changesetId + " for map with ID: " + mapId);

        return changesetId;
    }

    /**
     * Creates a new changeset
     * 
     * @param mapId
     *            ID of the map owning the changeset
     * @param userId
     *            ID of the user creating the changeset
     * @param tags
     *            tags
     * @param dbConn
     *            JDBC Connection
     * @return ID of the created changeset
     * @throws Exception
     */
    public static long createChangeset(final long mapId, final long userId, final java.util.Map<String, String> tags,
            Connection dbConn) throws Exception {
        log.debug("Creating changeset for map ID: " + mapId + "...");

        final long changesetId = Changeset.insertNew(mapId, userId, dbConn);
        if (changesetId == Long.MAX_VALUE || changesetId < 1) {
            throw new Exception("Invalid changeset ID: " + changesetId);
        }

        (new Changeset(mapId, changesetId, dbConn)).insertTags(mapId, tags, dbConn);

        log.debug("Created changeset for with ID: " + changesetId + " for map with ID: " + mapId);

        return changesetId;
    }

    /**
     * Closes a changeset
     *
     * @param changesetId
     *            ID of the changeset to close
     * @param dbConn
     *            JDBC Connection
     * @throws Exception
     */
    public static void closeChangeset(final long mapId, final long changesetId, Connection dbConn) throws Exception {
        Changeset changeset = new Changeset(mapId, changesetId, dbConn);
        changeset.verifyAvailability();
        final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());

        new SQLUpdateClause(dbConn, DbUtils.getConfiguration(mapId), changesets).where(changesets.id.eq(changesetId))
                .set(changesets.closedAt, now).execute();

    }

    /**
     * Determines whether the changeset is open or closed
     *
     * Each changeset is automatically assigned an expiration date when it is
     * created, so this basically checks to see if that expiration has occurred.
     *
     * @return true if the changeset is open; false otherwise
     * @throws IOException
     * @throws NumberFormatException
     */
    public boolean isOpen() throws NumberFormatException, IOException {
        // For some strange reason, Changeset DAO's started not working at some
        // point. More
        // specifically, calls to ChangesetDao would return stale data. I
        // suspect it has something to
        // do with the way the transaction is being initialized, but since I
        // couldn't figure out how to
        // fix it, I changed this code to not use ChangesetDao anymore.

        final Changesets changesetRecord = new SQLQuery(conn, DbUtils.getConfiguration(_mapId)).from(changesets)
                .where(changesets.id.eq(getId())).singleResult(changesets);

        final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        return changesetRecord.getClosedAt().after(now) && changesetRecord.getNumChanges() < Integer
                .parseInt(HootProperties.getPropertyOrDefault("maximumChangesetElements"));
    }

    /**
     * Close this changeset
     *
     * @throws Exception
     */
    public void close() throws Exception {
        log.debug("Closing changeset...");
        final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());

        if (new SQLUpdateClause(conn, DbUtils.getConfiguration(_mapId), changesets).where(changesets.id.eq(getId()))
                .set(changesets.closedAt, now).execute() != 1) {
            throw new Exception("Error closing changeset.");
        }
    }

    /**
     * Updates the expiration of this changeset in the database by modifying it
     * is closed at time
     *
     * This logic is pulled directly from the Rails port, and is meant to be
     * executed at the end of each upload process involving this changeset. This
     * effectively extends the changeset's expiration once any data is written
     * to it and leaves it with a shorter expiration if it has been opened but
     * had no data added to it.
     *
     * @throws Exception
     */
    public void updateExpiration() throws Exception {
        final DateTime now = new DateTime();

        Changesets changesetRecord = new SQLQuery(conn, DbUtils.getConfiguration(_mapId)).from(changesets)
                .where(changesets.id.eq(getId())).singleResult(changesets);

        if (isOpen()) {
            final int maximumChangesetElements = Integer
                    .parseInt(HootProperties.getPropertyOrDefault("maximumChangesetElements"));
            Timestamp newClosedAt = null;
            assert (changesetRecord.getNumChanges() <= maximumChangesetElements);
            if (changesetRecord.getNumChanges() == maximumChangesetElements) {
                newClosedAt = new Timestamp(now.getMillis());
            }
            else if (changesetRecord.getNumChanges() > 0) {
                /*
                 * from rails port:
                 *
                 * if (closed_at - created_at) > (MAX_TIME_OPEN - IDLE_TIMEOUT)
                 * self.closed_at = create_at + MAX_TIME_OPEN else
                 * self.closed_at = Time.now.getutc + IDLE_TIMEOUT
                 */

                final DateTime createdAt = new DateTime(changesetRecord.getCreatedAt().getTime());
                final DateTime closedAt = new DateTime(changesetRecord.getClosedAt().getTime());

                final int changesetIdleTimeout = Integer
                        .parseInt(HootProperties.getPropertyOrDefault("changesetIdleTimeoutMinutes"));
                final int changesetMaxOpenTime = Integer
                        .parseInt(HootProperties.getPropertyOrDefault("changesetMaxOpenTimeHours"));
                // The testChangesetAutoClose option = true causes
                // changesetIdleTimeoutMinutes and
                // changesetMaxOpenTimeHours to be interpreted in seconds rather
                // than minutes and hours,
                // respectively. This enables faster running of auto-close
                // related unit tests.
                if (Boolean.parseBoolean(HootProperties.getPropertyOrDefault("testChangesetAutoClose"))) {
                    final int changesetMaxOpenTimeSeconds = changesetMaxOpenTime;
                    final int changesetIdleTimeoutSeconds = changesetIdleTimeout;
                    if (Seconds.secondsBetween(createdAt, closedAt)
                            .getSeconds() > (changesetMaxOpenTimeSeconds - changesetIdleTimeoutSeconds)) {
                        newClosedAt = new Timestamp(createdAt.plusSeconds(changesetMaxOpenTimeSeconds).getMillis());
                    }
                    else {
                        newClosedAt = new Timestamp(now.plusSeconds(changesetIdleTimeoutSeconds).getMillis());
                    }
                }
                else {
                    final int changesetMaxOpenTimeMinutes = changesetMaxOpenTime * 60;
                    final int changesetIdleTimeoutMinutes = changesetIdleTimeout;
                    if (Minutes.minutesBetween(createdAt, closedAt)
                            .getMinutes() > (changesetMaxOpenTimeMinutes - changesetIdleTimeoutMinutes)) {
                        newClosedAt = new Timestamp(createdAt.plusMinutes(changesetMaxOpenTimeMinutes).getMillis());
                    }
                    else {
                        newClosedAt = new Timestamp(now.plusMinutes(changesetIdleTimeoutMinutes).getMillis());
                    }
                }
            }

            if (newClosedAt != null) {
                if (new SQLUpdateClause(conn, DbUtils.getConfiguration(_mapId), changesets)
                        .where(changesets.id.eq(getId())).set(changesets.closedAt, newClosedAt).execute() != 1) {
                    throw new Exception("Error updating expiration on changeset.");
                }
            }
        }
        else {
            // TODO: see if this code block is still needed

            // I have no idea why this code block is needed now. It didn't use
            // to be, but after
            // some refactoring to support the changes to marking items as
            // reviewed in ReviewResource, it
            // now is needed. I've been unable to track down what causes this to
            // happen.
            if (!changesetRecord.getClosedAt().before(new Timestamp(now.getMillis()))) {
                if (new SQLUpdateClause(conn, DbUtils.getConfiguration(_mapId), changesets)
                        .where(changesets.id.eq(getId())).set(changesets.closedAt, new Timestamp(now.getMillis()))
                        .execute() != 1) {
                    throw new Exception("Error updating expiration on changeset.");
                }
            }
        }
    }

    /**
     * Updates the number of changes associated with this changeset in the
     * database
     *
     * @param numChanges
     *            the number of changes for the changeset
     * @throws Exception
     */
    public void updateNumChanges(final int numChanges) throws Exception {
        log.debug("Updating num changes...");
        int maximumChangesetElements = Integer
                .parseInt(HootProperties.getPropertyOrDefault("maximumChangesetElements"));
        Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(_mapId)).from(changesets)
                .where(changesets.id.eq(getId())).singleResult(changesets);
        final int currentNumChanges = changeset.getNumChanges();
        assert ((currentNumChanges + numChanges) <= maximumChangesetElements);
        if (new SQLUpdateClause(conn, DbUtils.getConfiguration(_mapId), changesets).where(changesets.id.eq(getId()))
                .set(changesets.numChanges, currentNumChanges + numChanges).execute() != 1) {
            throw new Exception("Error updating num changes.");
        }
    }

    /**
     * Updates a changeset's bounds in the database
     *
     * @param bounds
     *            new bounds
     * @throws Exception
     */
    public void setBounds(final BoundingBox bounds) throws Exception {
        log.debug("Updating changeset bounds...");

        if (new SQLUpdateClause(conn, DbUtils.getConfiguration(_mapId), changesets).where(changesets.id.eq(getId()))
                .set(changesets.maxLat, bounds.getMaxLat()).set(changesets.maxLon, bounds.getMaxLon())
                .set(changesets.minLat, bounds.getMinLat()).set(changesets.minLon, bounds.getMinLon()).execute() != 1) {
            throw new Exception("Error updating changeset bounds.");
        }
    }

    /**
     * Retrieves a changeset's bounds from the database
     *
     * @return changeset bounds
     * @throws Exception
     */
    public BoundingBox getBounds() throws Exception {
        log.debug("Retrieving changeset bounds...");

        Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(_mapId)).from(changesets)
                .where(changesets.id.eq(getId())).singleResult(changesets);

        // I don't like doing this...
        double minLon = changeset.getMinLon();
        double minLat = changeset.getMinLat();
        double maxLon = changeset.getMaxLon();
        double maxLat = changeset.getMaxLat();
        if (minLon == GeoUtils.DEFAULT_COORD_VALUE || minLat == GeoUtils.DEFAULT_COORD_VALUE
                || maxLon == GeoUtils.DEFAULT_COORD_VALUE || maxLat == GeoUtils.DEFAULT_COORD_VALUE) {
            return new BoundingBox();
        }
        // this BoundingBox constructor requires that all values be valid (can't
        // create an invalid
        // empty bounds with this one)
        return new BoundingBox(minLon, minLat, maxLon, maxLat);
    }

    /**
     * Inserts a new empty changeset into the services database
     *
     * @param mapId
     *            corresponding map ID for the node
     * @param userId
     *            corresponding user ID for the node
     * @param dbConn
     *            JDBC Connection
     * @return ID of the inserted changeset
     * @throws Exception
     */
    public static long insertNew(final long mapId, final long userId, Connection dbConn) throws Exception {
        log.debug("Inserting new changeset...");

        final DateTime now = new DateTime();

        Timestamp closedAt = null;
        final int changesetIdleTimeout = Integer
                .parseInt(HootProperties.getPropertyOrDefault("changesetIdleTimeoutMinutes"));
        // The testChangesetAutoClose option = true causes
        // changesetIdleTimeoutMinutes to be interpreted
        // in seconds rather than minutes and enables faster running of
        // auto-close related unit tests.
        if (Boolean.parseBoolean(HootProperties.getPropertyOrDefault("testChangesetAutoClose"))) {
            closedAt = new Timestamp(now.plusSeconds(changesetIdleTimeout).getMillis());
        }
        else {
            closedAt = new Timestamp(now.plusMinutes(changesetIdleTimeout).getMillis());
        }

        return new SQLInsertClause(dbConn, DbUtils.getConfiguration("" + mapId), changesets)
                .columns(changesets.closedAt, changesets.createdAt, changesets.maxLat, changesets.maxLon,
                        changesets.minLat, changesets.minLon, changesets.userId)
                .values(closedAt, new Timestamp(now.getMillis()), GeoUtils.DEFAULT_COORD_VALUE,
                        GeoUtils.DEFAULT_COORD_VALUE, GeoUtils.DEFAULT_COORD_VALUE, GeoUtils.DEFAULT_COORD_VALUE,
                        userId)
                .executeWithKey(changesets.id);
    }

    /**
     * Determines whether the changeset is available for update
     *
     * @throws Exception
     *             //TODO: verify user updating changeset is the same one that
     *             created it; otherwise return 409
     */
    public void verifyAvailability() throws Exception {
        // see comments in isOpen method for why ChangesetDao is not used here
        // anymore
        Changesets changesetRecord = null;
        boolean changesetExists = false;
        try {
            log.debug("Verifying changeset with ID: " + getId() + " has previously been created ...");

            changesetExists = new SQLQuery(conn, DbUtils.getConfiguration(_mapId)).from(changesets)
                    .where(changesets.id.eq(getId())).count() > 0;
        }
        catch (Exception e) {
            ResourceErrorHandler.handleError(
                    "Error updating changeset with ID: " + getId() + " (" + e.getMessage() + ")", Status.BAD_REQUEST,
                    log);
        }
        if (!changesetExists) {
            // I haven't been able to explicit find in the OSM docs or code what
            // type of response is
            // returned here, but a 404 seems to make sense.
            throw new Exception("Changeset to be updated does not exist with ID: " + getId() + ".  Please create the "
                    + "changeset first.");
        }

        // this handles checking changeset expiration
        if (!isOpen()) {
            // this needs to be retrieved again to refresh the data
            changesetRecord = new SQLQuery(conn, DbUtils.getConfiguration(_mapId)).from(changesets)
                    .where(changesets.id.eq(getId())).singleResult(changesets);
            throw new Exception(
                    "The changeset with ID: " + getId() + " was closed at " + changesetRecord.getClosedAt());
        }
    }

    /**
     * Determines whether the current number of changes associated with this
     * changeset plus some new set of changes exceeds the maximum allowed
     * threshold.
     *
     * @return true; if the changeset entity count is exceeded; false otherwise
     * @throws IOException
     *             if unable to open the services configuration file
     * @throws NumberFormatException
     * @throws TransformerException
     */
    public boolean requestChangesExceedMaxElementThreshold(final Document changesetDiffDoc)
            throws NumberFormatException, IOException, TransformerException {
        final int newChangeCount = XPathAPI.selectNodeList(changesetDiffDoc, "//osmChange/*/node").getLength()
                + XPathAPI.selectNodeList(changesetDiffDoc, "//osmChange/*/way").getLength()
                + XPathAPI.selectNodeList(changesetDiffDoc, "//osmChange/*/relation").getLength();

        Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(_mapId)).from(changesets)
                .where(changesets.id.eq(getId())).singleResult(changesets);
        return (newChangeCount + changeset.getNumChanges()) > Integer
                .parseInt(HootProperties.getProperty("maximumChangesetElements"));
    }

    private void writeTags(final long mapId, final String tagsStr) throws Exception {
        PreparedStatement ps = null;
        try {
            String sql = "UPDATE changesets_" + mapId + " SET tags=? WHERE id=?";
            ps = conn.prepareStatement(sql);
            ps.setObject(1, tagsStr, Types.OTHER);
            ps.setLong(2, getId());

            long execResult = ps.executeUpdate();
            if (execResult < 1) {
                throw new Exception("No tags were changed for changeset_" + mapId);
            }
        }
        finally {
            if (ps != null) {
                ps.close();
            }
        }
    }

    /**
     * Inserts all tags for an element into the services database
     * 
     * @param mapId
     *            ID of the map owning the element
     * @param tags
     *            map containing tags
     * @param conn
     *            JDBC Connection
     * @throws Exception
     */
    public void insertTags(final long mapId, final java.util.Map<String, String> tags, Connection conn)
            throws Exception {
        try {
            log.debug("Inserting tags for changeset with ID: " + getId());

            String strKv = "";
            for (Map.Entry<String, String> tagEntry : tags.entrySet()) {
                if (strKv.length() > 0) {
                    strKv += ",";
                }
                strKv += tagEntry.getKey() + "=>" + tagEntry.getValue();
            }
            String tagsStr = "";
            tagsStr += strKv;

            writeTags(mapId, tagsStr);
        }
        catch (Exception e) {
            throw new Exception("Error inserting tags for changeset with ID: " + getId() + " - " + e.getMessage());
        }
    }

    /**
     * Inserts all tags for an element into the services database
     *
     * @param mapId
     *            ID of the map owning the element
     * @param xml
     *            list of XML tags
     * @param conn
     *            JDBC Connection
     * @throws Exception
     */
    public void insertTags(final long mapId, final NodeList xml, Connection conn) throws Exception {
        try {
            log.debug("Inserting tags for changeset with ID: " + getId());

            String strKv = "";
            for (int i = 0; i < xml.getLength(); i++) {
                NamedNodeMap tagAttributes = xml.item(i).getAttributes();
                String key = "\"" + tagAttributes.getNamedItem("k").getNodeValue() + "\"";
                String val = "\"" + tagAttributes.getNamedItem("v").getNodeValue() + "\"";
                if (strKv.length() > 0) {
                    strKv += ",";
                }
                strKv += key + "=>" + val;
            }
            String tagsStr = "";
            tagsStr += strKv;

            writeTags(mapId, tagsStr);
        }
        catch (Exception e) {
            throw new Exception("Error inserting tags for changeset with ID: " + getId() + " - " + e.getMessage());
        }
    }
}
