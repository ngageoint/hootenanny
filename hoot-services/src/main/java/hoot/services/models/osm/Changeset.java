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

import static hoot.services.HootProperties.*;
import static hoot.services.models.db.QChangesets.changesets;
import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Timestamp;
import java.util.HashMap;

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

import hoot.services.geo.BoundingBox;
import hoot.services.models.db.Changesets;
import hoot.services.utils.GeoUtils;


/**
 * Represents the model of an OSM changeset
 */
public class Changeset extends Changesets {
    private static final Logger logger = LoggerFactory.getLogger(Changeset.class);

    private long mapId = -1;

    public Changeset(long mapId, long changesetId) {
        this.mapId = mapId;
        setId(changesetId);
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
     * @return ID of the created changeset
     */
    public static long createChangeset(Document changesetDoc, long mapId, long userId) {
        logger.debug("Creating changeset for map ID: {}...", mapId);

        NodeList nodeList;
        try {
            nodeList = XPathAPI.selectNodeList(changesetDoc, "//changeset/tag");
        }
        catch (TransformerException e) {
            throw new RuntimeException("Error during a call to XPathAPI!", e);
        }

        java.util.Map<String, String> tags = new HashMap<>();

        for (int i = 0; i < nodeList.getLength(); i++) {
            NamedNodeMap tagAttributes = nodeList.item(i).getAttributes();

            if ((tagAttributes.getNamedItem("k") == null) || (tagAttributes.getNamedItem("v") == null)) {
                throw new RuntimeException("Error inserting tags: cannot insert(" + tagAttributes +")");
            }

            String key = tagAttributes.getNamedItem("k").getNodeValue();
            String value = tagAttributes.getNamedItem("v").getNodeValue();

            tags.put(key, value);
        }

        long changesetId = insertNew(mapId, userId, tags);

        logger.debug("Created changeset for with ID: {} for map with ID: {}", changesetId, mapId);

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
     * @return ID of the created changeset
     */
    public static long createChangeset(long mapId, long userId, java.util.Map<String, String> tags) {
        return insertNew(mapId, userId, tags);
    }

    /**
     * Closes a changeset
     *
     * @param changesetId
     *            ID of the changeset to close
     */
    public static void closeChangeset(long mapId, long changesetId) {
        Timestamp now = new Timestamp(System.currentTimeMillis());
        closeChangeset(mapId, changesetId, now);
    }

    private static long closeChangeset(long mapId, long changesetId, Timestamp timestamp) {
        Changeset changeset = new Changeset(mapId, changesetId);
        changeset.verifyAvailability();

        return createQuery(mapId)
                .update(changesets)
                .where(changesets.id.eq(changesetId))
                .set(changesets.closedAt, timestamp)
                .execute();
    }

    private static Changesets getChangesetUsing(Long mapId, long changesetId) {
        return createQuery(mapId).select(changesets).from(changesets).where(changesets.id.eq(changesetId)).fetchOne();
    }

    /**
     * Determines whether the changeset is open or closed
     *
     * Each changeset is automatically assigned an expiration date when it is
     * created, so this basically checks to see if that expiration has occurred.
     *
     * @return true if the changeset is open; false otherwise
     */
    private boolean isOpen() {
        Changesets changesetRecord = getChangesetUsing(mapId, getId());

        Timestamp now = new Timestamp(System.currentTimeMillis());

        return changesetRecord.getClosedAt().after(now) &&
                (changesetRecord.getNumChanges() < Integer.parseInt(MAXIMUM_CHANGESET_ELEMENTS));
    }

    /**
     * Close this changeset
     */
    public void close() {
        Timestamp now = new Timestamp(System.currentTimeMillis());

        if (closeChangeset(mapId, getId(), now) != 1) {
            throw new RuntimeException("Error closing changeset where mapId = " +
                    mapId + " and changesetId = " + getId());
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
     */
    public void updateExpiration() {
        DateTime now = new DateTime();

        Changesets changesetRecord = getChangesetUsing(mapId, getId());

        if (isOpen()) {
            int maximumChangesetElements = Integer.parseInt(MAXIMUM_CHANGESET_ELEMENTS);

            Timestamp newClosedAt = null;
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

                DateTime createdAt = new DateTime(changesetRecord.getCreatedAt().getTime());
                DateTime closedAt = new DateTime(changesetRecord.getClosedAt().getTime());

                int changesetIdleTimeout = Integer.parseInt(CHANGESET_IDLE_TIMEOUT_MINUTES);
                int changesetMaxOpenTime = Integer.parseInt(CHANGESET_MAX_OPEN_TIME_HOURS);

                // The testChangesetAutoClose option = true causes
                // changesetIdleTimeoutMinutes and changesetMaxOpenTimeHours to be interpreted in seconds rather
                // than minutes and hours, respectively. This enables faster running of auto-close related unit tests.
                if (Boolean.parseBoolean(TEST_CHANGESET_AUTO_CLOSE)) {
                    int changesetMaxOpenTimeSeconds = changesetMaxOpenTime;
                    int changesetIdleTimeoutSeconds = changesetIdleTimeout;
                    if (Seconds.secondsBetween(createdAt, closedAt)
                            .getSeconds() > (changesetMaxOpenTimeSeconds - changesetIdleTimeoutSeconds)) {
                        newClosedAt = new Timestamp(createdAt.plusSeconds(changesetMaxOpenTimeSeconds).getMillis());
                    }
                    else {
                        newClosedAt = new Timestamp(now.plusSeconds(changesetIdleTimeoutSeconds).getMillis());
                    }
                }
                else {
                    int changesetMaxOpenTimeMinutes = changesetMaxOpenTime * 60;
                    int changesetIdleTimeoutMinutes = changesetIdleTimeout;
                    if (Minutes.minutesBetween(createdAt, closedAt).getMinutes() >
                            (changesetMaxOpenTimeMinutes - changesetIdleTimeoutMinutes)) {
                        newClosedAt = new Timestamp(createdAt.plusMinutes(changesetMaxOpenTimeMinutes).getMillis());
                    }
                    else {
                        newClosedAt = new Timestamp(now.plusMinutes(changesetIdleTimeoutMinutes).getMillis());
                    }
                }
            }

            if (newClosedAt != null) {
                if (closeChangeset(mapId, getId(), newClosedAt) != 1) {
                    throw new RuntimeException("Error updating expiration on changeset where mapId = " +
                            mapId + " and changesetId = " + newClosedAt);
                }
            }
        }
        else {
            // TODO: see if this code block is still needed

            // I have no idea why this code block is needed now. It didn't use to be, but after some refactoring
            // to support the changes to marking items as reviewed in ReviewResource, it now is needed. I've been
            // unable to track down what causes this to happen.
            if (!changesetRecord.getClosedAt().before(new Timestamp(now.getMillis()))) {
                if (createQuery(mapId)
                        .update(changesets)
                        .where(changesets.id.eq(getId()))
                        .set(changesets.closedAt, new Timestamp(now.getMillis()))
                        .execute() != 1) {
                    throw new RuntimeException("Error updating expiration on changeset where mapId = " +
                            mapId + " and changesetId = " + getId());
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
     */
    public void updateNumChanges(int numChanges) {
        Changesets changeset = getChangesetUsing(mapId, getId());

        int currentNumChanges = changeset.getNumChanges();

        if (createQuery(mapId)
                .update(changesets)
                .where(changesets.id.eq(getId()))
                .set(changesets.numChanges, currentNumChanges + numChanges)
                .execute() != 1) {
            throw new RuntimeException("Error updating num changes where mapId = " +
                    mapId + " and changesetId = " + getId());
        }
    }

    /**
     * Updates a changeset's bounds in the database
     *
     * @param bounds
     *            new bounds
     */
    public void setBounds(BoundingBox bounds) {
        if (createQuery(mapId).update(changesets)
                .where(changesets.id.eq(getId()))
                .set(changesets.maxLat, bounds.getMaxLat())
                .set(changesets.maxLon, bounds.getMaxLon())
                .set(changesets.minLat, bounds.getMinLat())
                .set(changesets.minLon, bounds.getMinLon())
                .execute() != 1) {
            throw new RuntimeException("Error updating changeset bounds where mapId = " +
                    mapId + " and changesetId = " + getId());
        }
    }

    /**
     * Retrieves a changeset's bounds from the database
     *
     * @return changeset bounds
     */
    public BoundingBox getBounds() {
        Changesets changeset = getChangesetUsing(mapId, getId());

        // I don't like doing this...
        double minLon = changeset.getMinLon();
        double minLat = changeset.getMinLat();
        double maxLon = changeset.getMaxLon();
        double maxLat = changeset.getMaxLat();

        if ((minLon == GeoUtils.DEFAULT_COORD_VALUE) || (minLat == GeoUtils.DEFAULT_COORD_VALUE)
                || (maxLon == GeoUtils.DEFAULT_COORD_VALUE) || (maxLat == GeoUtils.DEFAULT_COORD_VALUE)) {
            return new BoundingBox();
        }

        // this BoundingBox constructor requires that all values be valid (can't
        // create an invalid empty bounds with this one)
        return new BoundingBox(minLon, minLat, maxLon, maxLat);
    }

    /**
     * Inserts a new empty changeset into the services database
     *
     * @param mapId
     *            corresponding map ID for the node
     * @param userId
     *            corresponding user ID for the node
     * @return ID of the inserted changeset
     */
    public static long insertNew(long mapId, long userId, java.util.Map<String, String> tags) {
        DateTime now = new DateTime();

        Timestamp closedAt;
        int changesetIdleTimeout = Integer.parseInt(CHANGESET_IDLE_TIMEOUT_MINUTES);

        // The testChangesetAutoClose option = true causes changesetIdleTimeoutMinutes to be interpreted
        // in seconds rather than minutes and enables faster running of auto-close related unit tests.
        if (Boolean.parseBoolean(TEST_CHANGESET_AUTO_CLOSE)) {
            closedAt = new Timestamp(now.plusSeconds(changesetIdleTimeout).getMillis());
        }
        else {
            closedAt = new Timestamp(now.plusMinutes(changesetIdleTimeout).getMillis());
        }

        long changesetId = createQuery(mapId)
                .insert(changesets)
                .columns(changesets.closedAt, changesets.createdAt, changesets.maxLat, changesets.maxLon,
                        changesets.minLat, changesets.minLon, changesets.userId, changesets.tags)
                .values(closedAt, new Timestamp(now.getMillis()), GeoUtils.DEFAULT_COORD_VALUE,
                        GeoUtils.DEFAULT_COORD_VALUE, GeoUtils.DEFAULT_COORD_VALUE, GeoUtils.DEFAULT_COORD_VALUE,
                        userId, tags)
                .executeWithKey(changesets.id);

        if ((changesetId == Long.MAX_VALUE) || (changesetId < 1)) {
            throw new RuntimeException("Invalid changesetId = " + changesetId +
                    ".  mapId = " + mapId + ", userId = " + userId);
        }

        return changesetId;
    }

    /**
     * Determines whether the changeset is available for update
     *
     * TODO: verify user updating changeset is the same one that created it; otherwise return 409
     */
    public void verifyAvailability() {
        // see comments in isOpen method for why ChangesetDao is not used here anymore
        boolean changesetExists = createQuery(mapId).from(changesets).where(changesets.id.eq(getId())).fetchCount() > 0;

        if (!changesetExists) {
            // I haven't been able to explicit find in the OSM docs or code what
            // type of response is returned here, but a 404 seems to make sense.
            throw new IllegalStateException("Changeset to be updated does not exist with ID: "
                    + getId() + ".  Please create the " + "changeset first.");
        }

        // this handles checking changeset expiration
        if (!isOpen()) {
            // this needs to be retrieved again to refresh the data
            Changesets changesetRecord = getChangesetUsing(mapId, getId());

            throw new IllegalStateException("The changeset with ID: " + getId() +
                    " was closed at " + changesetRecord.getClosedAt());
        }
    }

    /**
     * Determines whether the current number of changes associated with this
     * changeset plus some new set of changes exceeds the maximum allowed
     * threshold.
     *
     * @return true; if the changeset entity count is exceeded; false otherwise
     */
    public boolean requestChangesExceedMaxElementThreshold(Document changesetDiffDoc) {
        int newChangeCount;

        try {
            newChangeCount = XPathAPI.selectNodeList(changesetDiffDoc, "//osmChange/*/node").getLength()
                    + XPathAPI.selectNodeList(changesetDiffDoc, "//osmChange/*/way").getLength()
                    + XPathAPI.selectNodeList(changesetDiffDoc, "//osmChange/*/relation").getLength();
        }
        catch (TransformerException e) {
            throw new RuntimeException("Error accessing changesetDiffDoc using XPathAPI!", e);
        }

        Changesets changeset = getChangesetUsing(mapId, getId());

        return (newChangeCount + changeset.getNumChanges()) > Integer.parseInt(MAXIMUM_CHANGESET_ELEMENTS);
    }
}
