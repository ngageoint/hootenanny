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
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.TreeMap;
import java.util.Vector;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.QTuple;
import com.mysema.query.types.expr.BooleanExpression;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db2.FolderMapMappings;
import hoot.services.db2.Folders;
import hoot.services.db2.Maps;
import hoot.services.db2.QChangesets;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentRelations;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.db2.QCurrentWays;
import hoot.services.db2.QUsers;
import hoot.services.geo.BoundingBox;
import hoot.services.geo.zindex.Range;
import hoot.services.geo.zindex.ZCurveRanger;
import hoot.services.geo.zindex.ZValue;
import hoot.services.models.dataset.FolderRecord;
import hoot.services.models.dataset.FolderRecords;
import hoot.services.models.dataset.LinkRecord;
import hoot.services.models.dataset.LinkRecords;
import hoot.services.models.osm.Element.ElementType;


/**
 * Represents the model for an Hootenanny OSM map
 *
 * When modifying the node query by bounds, make sure it is using the index on
 * the tile id. The index scan only seems to trigger if the number of rows
 * returned by the query is <= 1% of the total rows. The execution plan can be
 * checked in postgres with EXPLAIN ANALYZE. Possibly, after query changes you
 * may have to force it to use the tile index by doing something like:
 * 
 * SELECT * FROM (SELECT ... tile_id stuff) WHERE lat <= BLAH AND lat >= BLAH
 * and lon <= ...
 * 
 * ...although I haven't had to do that yet.
 * 
 * //TODO: This class needs to conduct its queries within a read-only
 * transaction so that it doesn't read invalid information while writers are
 * writing elements at the same time it queries elements out.
 */
public class Map extends Maps {
    private static final Logger log = LoggerFactory.getLogger(Map.class);

    private static QCurrentNodes currentnodes = QCurrentNodes.currentNodes;

    private Connection conn;

    public static long testDelayMilliseconds = -1;

    // I want to retrieve nodes, ways, and relations with one query and return a
    // single
    // set of records. However, there is no type information in the records and
    // the fields between
    // them aren't unique (way and relation have the same database field names),
    // so there is no
    // way for the response writer to know what type of element to write.
    // Instead, I'm doing
    // separate queries for each element type and returning them in a collection
    // grouped by element
    // type.
    private java.util.Map<ElementType, java.util.Map<Long, Tuple>> elementIdsToRecordsByType;

    private BoundingBox bounds;

    public Map(final long id, Connection conn) {
        super();

        setId(id);
        this.conn = conn;
    }

    /*
     * Retrieves all ranges of quad tiles that fall within the bounds
     */
    private static Vector<Range> getTileRanges(final BoundingBox bounds) throws NumberFormatException, IOException {
        log.debug("Retrieving tile ranges...");
        int queryDimensions = Integer.parseInt(HootProperties.getPropertyOrDefault("mapQueryDimensions"));
        ZCurveRanger ranger = new ZCurveRanger(new ZValue(queryDimensions, 16,
                // use y, x ordering here
                new double[] { -1 * BoundingBox.LAT_LIMIT, -1 * BoundingBox.LON_LIMIT },
                new double[] { BoundingBox.LAT_LIMIT, BoundingBox.LON_LIMIT }));
        return ranger.decomposeRange(bounds.toZIndexBox(), 1);
    }

    /*
     * Returns the SQL where condition for the calculated tile ranges
     */
    private static BooleanExpression getTileWhereCondition(final Vector<Range> tileIdRanges) {
        List<BooleanExpression> tileConditions = new ArrayList<BooleanExpression>();
        for (Range range : tileIdRanges) {
            QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
            tileConditions.add(currentNodes.tile.goe(range.getMin()).and(currentNodes.tile.loe(range.getMax())));
        }
        BooleanExpression combinedTileCondition = null;
        for (BooleanExpression tileCondition : tileConditions) {
            if (combinedTileCondition == null) {
                combinedTileCondition = tileCondition;
            }
            else {
                combinedTileCondition = combinedTileCondition.or(tileCondition);
            }
        }
        return combinedTileCondition;
    }

    /*
     * Returns the geospatial where condition to apply with the tile conditions
     * to ensure nodes that fall outside the bounding box are not returned
     */
    private static BooleanExpression getGeospatialWhereCondition(final BoundingBox bounds) {
        QCurrentNodes nodes = QCurrentNodes.currentNodes;
        return nodes.longitude.goe(bounds.getMinLon()).and(nodes.latitude.goe(bounds.getMinLat()))
                .and(nodes.longitude.loe(bounds.getMaxLon())).and(nodes.latitude.loe(bounds.getMaxLat()));
    }

    private static void validateQueryBounds(final BoundingBox bounds) throws Exception {
        log.debug("Checking request bounds size...");
        double maxQueryAreaDegrees = Double.parseDouble(HootProperties.getPropertyOrDefault("maxQueryAreaDegrees"));
        double requestedArea = bounds.getArea();
        if (requestedArea > maxQueryAreaDegrees) {
            throw new Exception("The maximum bbox size is: " + maxQueryAreaDegrees + ", and your request was too "
                    + "large at " + requestedArea + " degrees.  Request a smaller area.");
        }
    }

    /*
     * This does a check to see how many nodes are in the query and throws an
     * exception if over the configured maximum. This only needs to be used for
     * actual map queries returning XML. If you're just doing a node count
     * check, then getNodeCount should be used.
     */
    private void validateNodeCount(final BooleanExpression combinedGeospatialCondition) throws Exception {
        log.debug("Retrieving node count...");

        QCurrentNodes curr_nodes = QCurrentNodes.currentNodes;
        final long nodeCount = new SQLQuery(conn, DbUtils.getConfiguration(getId())).from(curr_nodes)
                .where(combinedGeospatialCondition.and(curr_nodes.visible.eq(true))).count();

        // The max node count only applies to the nodes falling within the query
        // bounds, not
        // those that belong to ways that cross the query bounds but fall
        // outside of the query bounds,
        // even though those nodes are returned as well in the query.
        long maxQueryNodes = Long.parseLong(HootProperties.getPropertyOrDefault("maxQueryNodes"));
        if (nodeCount > maxQueryNodes) {
            throw new Exception("The maximum number of nodes that may be returned in a map query is " + maxQueryNodes
                    + ".  This query returned " + nodeCount + " nodes.  Please "
                    + "execute a query which returns fewer nodes.");
        }
    }

    public JSONObject retrieveNodesMBR(final BoundingBox bounds) throws Exception {
        JSONObject ret = new JSONObject();
        // get the intersecting tile ranges for the nodes
        final Vector<Range> tileIdRanges = getTileRanges(bounds);
        if (tileIdRanges.size() > 0) {
            BooleanExpression combinedGeospatialCondition = getTileWhereCondition(tileIdRanges).and(
                    getGeospatialWhereCondition(bounds));
            List<Tuple> geospatialQueryNodeResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                    .from(currentnodes)
                    .where(combinedGeospatialCondition.and(currentnodes.visible.eq(true)))
                    .list(currentnodes.longitude.max(), currentnodes.longitude.min(), currentnodes.latitude.max(),
                            currentnodes.latitude.min());

            Double maxLon = geospatialQueryNodeResults.get(0).get(0, Double.class);
            Double minLon = geospatialQueryNodeResults.get(0).get(1, Double.class);
            Double maxLat = geospatialQueryNodeResults.get(0).get(2, Double.class);
            Double minLat = geospatialQueryNodeResults.get(0).get(3, Double.class);
            ret.put("maxlon", maxLon);
            ret.put("minlon", minLon);
            ret.put("maxlat", maxLat);
            ret.put("minlat", minLat);
        }

        return ret;
    }

    public long getNodesCount(final BoundingBox bounds) throws Exception {
        long ret = 0;
        // get the intersecting tile ranges for the nodes
        final Vector<Range> tileIdRanges = getTileRanges(bounds);
        if (tileIdRanges.size() > 0) {
            BooleanExpression combinedGeospatialCondition = getTileWhereCondition(tileIdRanges).and(
                    getGeospatialWhereCondition(bounds));
            ret = new SQLQuery(conn, DbUtils.getConfiguration(getId())).from(currentnodes)
                    .where(combinedGeospatialCondition.and(currentnodes.visible.eq(true))).count();
        }

        return ret;
    }

    public JSONObject retrieveANode(final BoundingBox bounds) throws Exception {
        JSONObject ret = new JSONObject();
        // get the intersecting tile ranges for the nodes
        final Vector<Range> tileIdRanges = getTileRanges(bounds);
        if (tileIdRanges.size() > 0) {
            BooleanExpression combinedGeospatialCondition = getTileWhereCondition(tileIdRanges).and(
                    getGeospatialWhereCondition(bounds));

            QCurrentNodes currentnodes = QCurrentNodes.currentNodes;
            List<Tuple> geospatialQueryNodeResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                    .from(currentnodes).where(combinedGeospatialCondition.and(currentnodes.visible.eq(true))).limit(1)
                    .list(currentnodes.longitude, currentnodes.latitude);

            Double lon = geospatialQueryNodeResults.get(0).get(0, Double.class);
            Double lat = geospatialQueryNodeResults.get(0).get(1, Double.class);
            ret.put("lon", lon);
            ret.put("lat", lat);
        }

        return ret;
    }

    private java.util.Map<ElementType, java.util.Map<Long, Tuple>> retrieveElements(
            final BooleanExpression combinedGeospatialCondition) {
        // There is probably a way to get all this information by executing
        // fewer queries...

        // JDBC prepared statement number of parameters has a hard limit: 32767
        int nQueryPageSize = 32000;
        elementIdsToRecordsByType = new HashMap<ElementType, java.util.Map<Long, Tuple>>();

        // if the limit hasn't been exceeded, query out all nodes which fall
        // within the geospatial
        // bounds, are visible, and belong to this map; join in user info
        log.debug("Retrieving node records within the query bounds...");

        QCurrentNodes currentnodes = QCurrentNodes.currentNodes;
        QChangesets changesets = QChangesets.changesets;
        QUsers users = QUsers.users;

        java.util.Map<Long, Tuple> geospatialQueryNodeResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                .from(currentnodes).join(changesets).on(currentnodes.changesetId.eq(changesets.id)).join(users)
                .on(changesets.userId.eq(users.id))
                .where(combinedGeospatialCondition.and(currentnodes.visible.eq(true))).orderBy(currentnodes.id.asc())
                .map(currentnodes.id, new QTuple(currentnodes, users, changesets));

        elementIdsToRecordsByType.put(ElementType.Node, geospatialQueryNodeResults);

        List<Long> wayIds = new ArrayList<Long>();
        if (geospatialQueryNodeResults.size() > 0) {
            // get all ways which have way nodes with IDs in the previously
            // queried node results, are
            // visible, and belong to this map; join in user info
            log.debug("Retrieving way IDs within the query bounds...");
            QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;

            List<Long> nodesList = new ArrayList<Long>(geospatialQueryNodeResults.keySet());

            int totalNodeCnt = nodesList.size();
            int pageCnt = (int) Math.floor(totalNodeCnt / nQueryPageSize);

            for (int i = 0; i <= pageCnt; i++) {
                try {
                    List<Long> pageWayIds = null;
                    List<Long> pageList = null;
                    int iStart = i * nQueryPageSize;
                    int iEnd = iStart + nQueryPageSize;

                    if (i < pageCnt) {
                        pageList = nodesList.subList(iStart, iEnd);
                    }
                    else {
                        pageList = nodesList.subList(iStart, totalNodeCnt);
                    }

                    if (pageList == null || pageList.size() == 0) {
                        continue;
                    }
                    pageWayIds = new SQLQuery(conn, DbUtils.getConfiguration(getId())).from(currentWayNodes)
                            .where(currentWayNodes.nodeId.in(pageList)).list(currentWayNodes.wayId);
                    // TODO: should this be an assert instead? Regardless, fix
                    // the error handling.
                    if (wayIds.addAll(pageWayIds) == false) {
                        // error
                    }
                }
                catch (Exception ex) {
                    ex.getMessage();
                }
            }

            if (wayIds.size() > 0) {
                // from the way results, filter down those that are visible and
                // belong to this map; join in
                // user info
                log.debug("Retrieving ways within the query bounds...");
                QCurrentWays currentWays = QCurrentWays.currentWays;

                java.util.Map<Long, Tuple> wayResults = null;
                int totalWayCnt = wayIds.size();
                int wayPageCnt = (int) Math.floor(totalWayCnt / nQueryPageSize);

                for (int i = 0; i <= wayPageCnt; i++) {
                    try {
                        List<Long> pageList = null;
                        int iStart = i * nQueryPageSize;
                        int iEnd = iStart + nQueryPageSize;

                        if (i < wayPageCnt) {
                            pageList = wayIds.subList(iStart, iEnd);
                        }
                        else {
                            pageList = wayIds.subList(iStart, totalWayCnt);
                        }
                        if (pageList == null || pageList.size() == 0) {
                            continue;
                        }

                        if (i == 0) {
                            wayResults = new SQLQuery(conn, DbUtils.getConfiguration(getId())).from(currentWays)
                                    .join(changesets).on(currentWays.changesetId.eq(changesets.id)).join(users)
                                    .on(changesets.userId.eq(users.id))
                                    .where(currentWays.visible.eq(true).and(currentWays.id.in(pageList)))
                                    .orderBy(currentWays.id.asc())
                                    .map(currentWays.id, new QTuple(currentWays, users, changesets));
                        }
                        else {
                            assert (wayResults != null);
                            wayResults.putAll(new SQLQuery(conn, DbUtils.getConfiguration(getId())).from(currentWays)
                                    .join(changesets).on(currentWays.changesetId.eq(changesets.id)).join(users)
                                    .on(changesets.userId.eq(users.id))
                                    .where(currentWays.visible.eq(true).and(currentWays.id.in(pageList)))
                                    .orderBy(currentWays.id.asc())
                                    .map(currentWays.id, new QTuple(currentWays, users, changesets)));
                        }
                    }
                    catch (Exception ex) {
                        ex.getMessage();
                    }
                }

                elementIdsToRecordsByType.put(ElementType.Way, wayResults);

                // retrieve all way nodes for the previously retrieved ways
                log.debug("Retrieving additional way nodes IDs within the query bounds...");
                Set<Long> wayNodeIds = new HashSet<Long>();
                for (int i = 0; i <= wayPageCnt; i++) {
                    try {
                        List<Long> pageList = null;
                        int iStart = i * nQueryPageSize;
                        int iEnd = iStart + nQueryPageSize;

                        if (i < wayPageCnt) {
                            pageList = wayIds.subList(iStart, iEnd);
                        }
                        else {
                            pageList = wayIds.subList(iStart, totalWayCnt);
                        }

                        if (pageList == null || pageList.size() == 0) {
                            continue;
                        }

                        wayNodeIds.addAll(new HashSet<Long>(new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                .from(currentWayNodes).where(currentWayNodes.wayId.in(pageList))
                                .list(currentWayNodes.nodeId)));
                    }
                    catch (Exception ex) {
                        ex.getMessage();
                    }
                }

                assert (wayNodeIds.size() > 0);
                final long numWayNodes = wayNodeIds.size();
                log.debug("Found " + numWayNodes + " way nodes.");

                // retrieve all corresponding nodes to the IDs in the wayNodeIds
                // collection that don't
                // fall within the query bounds (all the way nodes referenced by
                // all the ways minus the
                // nodes falling within the query bounds that have already been
                // added); join in user info;
                // add the resulting nodes to the nodes collection
                final Set<Long> geospatialQueryNodeResultIds = new HashSet<Long>(geospatialQueryNodeResults.keySet());
                final long numGeospatialQueryNodeResults = geospatialQueryNodeResultIds.size();
                log.debug("There were " + numGeospatialQueryNodeResults + " nodes returned in the geospatial query.");
                Set<Long> additionalNodeIds = new HashSet<Long>(wayNodeIds);
                additionalNodeIds.removeAll(geospatialQueryNodeResultIds);
                assert (wayNodeIds.size() == numWayNodes);
                assert (geospatialQueryNodeResultIds.size() == numGeospatialQueryNodeResults);
                log.debug("Subtracting the geospatial query nodes from the way nodes collection results "
                        + "in a total of " + additionalNodeIds.size() + " nodes remaining to be added to the nodes "
                        + "collection.");
                if (additionalNodeIds.size() > 0) {
                    log.debug("Retrieving nodes falling outside of the query bounds but belonging to a "
                            + "retrieved way...");

                    java.util.Map<Long, Tuple> additionalNodeResults = null;

                    List<Long> additionalNodeIdsList = new ArrayList<Long>(additionalNodeIds);

                    int totalANILCnt = additionalNodeIdsList.size();
                    int anilPageCnt = (int) Math.floor(totalANILCnt / nQueryPageSize);

                    for (int i = 0; i <= anilPageCnt; i++) {
                        try {
                            List<Long> pageList = null;
                            int iStart = i * nQueryPageSize;
                            int iEnd = iStart + nQueryPageSize;

                            if (i < anilPageCnt) {
                                pageList = additionalNodeIdsList.subList(iStart, iEnd);
                            }
                            else {
                                pageList = additionalNodeIdsList.subList(iStart, totalANILCnt);
                            }

                            if (pageList == null || pageList.size() == 0) {
                                continue;
                            }

                            if (i == 0) {
                                additionalNodeResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                        .from(currentnodes).join(changesets)
                                        .on(currentnodes.changesetId.eq(changesets.id)).join(users)
                                        .on(changesets.userId.eq(users.id))
                                        .where(currentnodes.visible.eq(true).and(currentnodes.id.in(pageList)))
                                        .orderBy(currentnodes.id.asc())
                                        .map(currentnodes.id, new QTuple(currentnodes, users, changesets));
                            }
                            else {
                                assert (additionalNodeResults != null);
                                additionalNodeResults.putAll(new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                        .from(currentnodes).join(changesets)
                                        .on(currentnodes.changesetId.eq(changesets.id)).join(users)
                                        .on(changesets.userId.eq(users.id))
                                        .where(currentnodes.visible.eq(true).and(currentnodes.id.in(pageList)))
                                        .orderBy(currentnodes.id.asc())
                                        .map(currentnodes.id, new QTuple(currentnodes, users, changesets)));
                            }
                        }
                        catch (Exception ex) {
                            ex.getMessage();
                        }
                    }

                    java.util.Map<Long, Tuple> nodes = elementIdsToRecordsByType.get(ElementType.Node);
                    nodes.putAll(additionalNodeResults);
                    // want to keep the nodes ordered by ID, so add them back in
                    // with a TreeMap
                    elementIdsToRecordsByType.put(ElementType.Node, new TreeMap<Long, Tuple>(nodes));
                }
            }
            else {
                elementIdsToRecordsByType.put(ElementType.Way, new HashMap<Long, Tuple>());
            }

            /*
             * retrieve all relations that reference the nodes or ways
             * previously retrieved
             */
            log.debug("Retrieving relations IDs within the query bounds...");

            QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;

            Set<Long> relationIds = new HashSet<Long>();

            Set<Long> nodesset = elementIdsToRecordsByType.get(ElementType.Node).keySet();
            Set<Long> wayset = elementIdsToRecordsByType.get(ElementType.Way).keySet();

            if (nodesset != null && nodesset.size() == 0) {
                nodesset = new HashSet<Long>();
                nodesset.add(new Long(-1));
            }

            if (wayset != null && wayset.size() == 0) {
                wayset = new HashSet<Long>();
                wayset.add(new Long(-1));
            }
            if (elementIdsToRecordsByType.get(ElementType.Node).keySet().size() > 0) {
                List<Long> nodessetList = new ArrayList<Long>(nodesset);

                int totalNodeSetCnt = nodessetList.size();
                int nodesetPageCnt = (int) Math.floor(totalNodeSetCnt / nQueryPageSize);
                Set<Long> nodeSetRelationIds = new HashSet<Long>();
                for (int i = 0; i <= nodesetPageCnt; i++) {
                    try {
                        List<Long> pageList = null;
                        int iStart = i * nQueryPageSize;
                        int iEnd = iStart + nQueryPageSize;

                        if (i < nodesetPageCnt) {
                            pageList = nodessetList.subList(iStart, iEnd);
                        }
                        else {
                            pageList = nodessetList.subList(iStart, totalNodeSetCnt);
                        }

                        if (pageList == null || pageList.size() == 0) {
                            continue;
                        }
                        nodeSetRelationIds.addAll(new HashSet<Long>(new SQLQuery(conn, DbUtils
                                .getConfiguration(getId()))
                                .from(currentRelationMembers)
                                .where(currentRelationMembers.memberId.in(pageList).and(
                                        currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node)))
                                .list(currentRelationMembers.relationId)));
                    }
                    catch (Exception ex) {
                        ex.getMessage();
                    }
                }

                relationIds.addAll(nodeSetRelationIds);

                List<Long> waysetList = new ArrayList<Long>(wayset);
                int totalWaySetCnt = waysetList.size();
                int waysetPageCnt = (int) Math.floor(totalWaySetCnt / nQueryPageSize);
                Set<Long> waySetRelationIds = new HashSet<Long>();

                for (int i = 0; i <= waysetPageCnt; i++) {
                    try {
                        List<Long> pageList = null;
                        int iStart = i * nQueryPageSize;
                        int iEnd = iStart + nQueryPageSize;

                        if (i < waysetPageCnt) {
                            pageList = waysetList.subList(iStart, iEnd);
                        }
                        else {
                            pageList = waysetList.subList(iStart, totalWaySetCnt);
                        }

                        if (pageList == null || pageList.size() == 0) {
                            continue;
                        }

                        waySetRelationIds.addAll(new HashSet<Long>(
                                new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                        .from(currentRelationMembers)
                                        .where(currentRelationMembers.memberId.in(pageList).and(
                                                currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way)))
                                        .list(currentRelationMembers.relationId)));
                    }
                    catch (Exception ex) {
                        ex.getMessage();
                    }
                }

                relationIds.addAll(waySetRelationIds);
            }

            if (relationIds.size() > 0) {
                log.debug("Retrieving relations within the query bounds...");

                QCurrentRelations currentRelations = QCurrentRelations.currentRelations;

                java.util.Map<Long, Tuple> relationResults = null;

                List<Long> relationIdsList = new ArrayList<Long>(relationIds);

                int totalRelationIdsListCnt = relationIdsList.size();
                int relationIdsPageCnt = (int) Math.floor(totalRelationIdsListCnt / nQueryPageSize);

                for (int i = 0; i <= relationIdsPageCnt; i++) {
                    try {
                        List<Long> pageList = null;
                        int iStart = i * nQueryPageSize;
                        int iEnd = iStart + nQueryPageSize;

                        if (i < relationIdsPageCnt) {
                            pageList = relationIdsList.subList(iStart, iEnd);
                        }
                        else {
                            pageList = relationIdsList.subList(iStart, totalRelationIdsListCnt);
                        }

                        if (pageList == null || pageList.size() == 0) {
                            continue;
                        }

                        if (i == 0) {
                            relationResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                    .from(currentRelations).join(changesets)
                                    .on(currentRelations.changesetId.eq(changesets.id)).join(users)
                                    .on(changesets.userId.eq(users.id))
                                    .where(currentRelations.visible.eq(true).and(currentRelations.id.in(pageList)))
                                    .orderBy(currentRelations.id.asc())
                                    .map(currentRelations.id, new QTuple(currentRelations, users, changesets));
                        }
                        else {
                            assert (relationResults != null);
                            relationResults.putAll(new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                    .from(currentRelations).join(changesets)
                                    .on(currentRelations.changesetId.eq(changesets.id)).join(users)
                                    .on(changesets.userId.eq(users.id))
                                    .where(currentRelations.visible.eq(true).and(currentRelations.id.in(pageList)))
                                    .orderBy(currentRelations.id.asc())
                                    .map(currentRelations.id, new QTuple(currentRelations, users, changesets)));
                        }
                    }
                    catch (Exception ex) {
                        ex.getMessage();
                    }
                }

                elementIdsToRecordsByType.put(ElementType.Relation, relationResults);
            }
        }

        return elementIdsToRecordsByType;
    }

    /**
     * Executes a geospatial query for elements against the services database
     *
     * Bounds calculation:
     *
     * - All nodes that are inside a given bounding box and any relations that
     * reference them. - All ways that reference at least one node that is
     * inside a given bounding box, any relations that reference them [the
     * ways], and any nodes outside the bounding box that the ways may
     * reference. - All relations that reference one of the nodes, ways, or
     * relations included due to the above rules. (Does not apply recursively;
     * e.g. don't return every node and way the relations themselves
     * reference...only the ones in the query bounds).
     *
     * @param bounds
     *            geospatial bounds the returned nodes should fall within
     * @return a collection of elements mapped to their IDs, grouped by element
     *         type
     * @throws Exception
     *             if the number of nodes requested is larger than the maximum
     *             number allowed //TODO: get the readonly transaction working;
     *             see MapResourceTest::testReadTransactionWithoutFailure
     */
    public java.util.Map<ElementType, java.util.Map<Long, Tuple>> query(final BoundingBox bounds) throws Exception {
        //validateQueryBounds(bounds);

        // get the intersecting tile ranges for the nodes
        final Vector<Range> tileIdRanges = getTileRanges(bounds);
        java.util.Map<ElementType, java.util.Map<Long, Tuple>> elementIdsToRecordsByType = new HashMap<ElementType, java.util.Map<Long, Tuple>>();
        if (tileIdRanges.size() > 0) {
            BooleanExpression combinedGeospatialCondition = getTileWhereCondition(tileIdRanges).and(
                    getGeospatialWhereCondition(bounds));
            validateNodeCount(combinedGeospatialCondition);
            elementIdsToRecordsByType = retrieveElements(combinedGeospatialCondition);
        }

        return elementIdsToRecordsByType;
    }

    private java.util.Map<ElementType, Set<Long>> retrieveElementIds(final BooleanExpression combinedGeospatialCondition) {
        java.util.Map<ElementType, Set<Long>> elementIdsByType = new HashMap<ElementType, Set<Long>>();

        // if the limit hasn't been exceeded, query out all nodes which fall
        // within the geospatial
        // bounds, are visible, and belong to this map
        log.debug("Retrieving IDs of nodes within the query bounds...");

        QCurrentNodes currentnodes = QCurrentNodes.currentNodes;

        Set<Long> nodeIds = new HashSet<Long>(new SQLQuery(conn, DbUtils.getConfiguration(getId())).from(currentnodes)
                .where(combinedGeospatialCondition.and(currentnodes.visible.eq(true))).list(currentnodes.id));

        elementIdsByType.put(ElementType.Node, nodeIds);

        Set<Long> wayIds = null;
        if (nodeIds.size() > 0) {
            // get all ways which have way nodes that belong to the previously
            // queried node results, are
            // visible, and belong to this map; join in user info
            log.debug("Retrieving IDs of ways within the query bounds...");
            QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;

            wayIds = new HashSet<Long>(new SQLQuery(conn, DbUtils.getConfiguration(getId())).from(currentWayNodes)
                    .where(currentWayNodes.nodeId.in(nodeIds)).list(currentWayNodes.wayId));

            elementIdsByType.put(ElementType.Way, wayIds);

            /*
             * retrieve all relations that reference the nodes or ways
             * previously retrieved
             */
            log.debug("Retrieving relations IDs within the query bounds...");

            QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;

            Set<Long> nodesIds = elementIdsByType.get(ElementType.Node);
            Set<Long> waysIds = elementIdsByType.get(ElementType.Way);

            // if the Set is empty the in statement blows up..
            if (nodesIds.size() == 0) {
                nodesIds.add((long) -1);
            }

            if (waysIds.size() == 0) {
                waysIds.add((long) -1);
            }
            Set<Long> relationIds = new HashSet<Long>(new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                    .from(currentRelationMembers)
                    .where(currentRelationMembers.memberId
                            .in(nodesIds)
                            .and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node))
                            .or(currentRelationMembers.memberId.in(waysIds).and(
                                    currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way))))
                    .list(currentRelationMembers.relationId));

            elementIdsByType.put(ElementType.Relation, relationIds);
        }

        return elementIdsByType;
    }

    /**
     * Executes a geospatial query for element IDs against the services database
     *
     * Bounds calculation: see query
     *
     * - All nodes that are inside a given bounding box and any relations that
     * reference them. - All ways that reference at least one node that is
     * inside a given bounding box, any relations that reference them [the
     * ways], and any nodes outside the bounding box that the ways may
     * reference. - All relations that reference one of the nodes, ways, or
     * relations included due to the above rules. (Does not apply recursively;
     * e.g. don't return every node and way the relations themselves
     * reference...only the ones in the query bounds).
     *
     * @param bounds
     *            geospatial bounds the returned nodes should fall within
     * @return a collection of element IDs, grouped by element type
     * @throws Exception
     *             if the number of nodes requested is larger than the maximum
     *             number allowed //TODO: see query
     */
    public java.util.Map<ElementType, Set<Long>> queryForElementIds(final BoundingBox bounds) throws Exception {
        // TODO: add transaction; verify that no other writes are seen during
        // this transaction

        validateQueryBounds(bounds);
        // get the intersecting tile ranges for the nodes
        final Vector<Range> tileIdRanges = getTileRanges(bounds);
        java.util.Map<ElementType, Set<Long>> elementIdsByType = new HashMap<ElementType, Set<Long>>();
        if (tileIdRanges.size() > 0) {
            BooleanExpression combinedGeospatialCondition = getTileWhereCondition(tileIdRanges)
                    .and(getGeospatialWhereCondition(bounds));
            validateNodeCount(combinedGeospatialCondition);
            elementIdsByType = retrieveElementIds(combinedGeospatialCondition);
        }
        return elementIdsByType;
    }

    // TODO: use reflection to collapse the next few methods into one

    /**
     * Converts a set of map layer database records into an object returnable by
     * a web service
     *
     * @param mapLayerRecords
     *            set of map layer records
     * @return map layers web service object
     */
    public static MapLayers mapLayerRecordsToLayers(List<Maps> mapLayerRecords) {
        MapLayers mapLayers = new MapLayers();
        List<MapLayer> mapLayerList = new ArrayList<MapLayer>();
        for (Maps mapLayerRecord : mapLayerRecords) {
            MapLayer mapLayer = new MapLayer();
            mapLayer.setId(mapLayerRecord.getId());
            mapLayer.setName(mapLayerRecord.getDisplayName());
            mapLayer.setDate(mapLayerRecord.getCreatedAt());
            mapLayerList.add(mapLayer);
        }
        mapLayers.setLayers(mapLayerList.toArray(new MapLayer[] {}));
        return mapLayers;
    }

    /**
     * Converts a set of folder database records into an object returnable by a
     * web service
     *
     * @param folderRecordSet
     *            set of map layer records
     * @return folders web service object
     */
    public static FolderRecords mapFolderRecordsToFolders(List<Folders> folderRecordSet) {
        FolderRecords folderRecords = new FolderRecords();
        List<FolderRecord> folderRecordList = new ArrayList<FolderRecord>();
        for (Folders folderRecord : folderRecordSet) {
            FolderRecord folder = new FolderRecord();
            folder.setId(folderRecord.getId());
            folder.setName(folderRecord.getDisplayName());
            folder.setParentId(folderRecord.getParentId());
            folderRecordList.add(folder);
        }
        folderRecords.setFolders(folderRecordList.toArray(new FolderRecord[] {}));
        return folderRecords;
    }

    /**
     * Converts a set of database records into an object returnable by a web
     * service
     *
     * @param linkRecordSet
     *            set of map layer records
     * @return folders web service object
     */
    public static LinkRecords mapLinkRecordsToLinks(List<FolderMapMappings> linkRecordSet) {
        LinkRecords linkRecords = new LinkRecords();
        List<LinkRecord> linkRecordList = new ArrayList<LinkRecord>();
        for (FolderMapMappings linkRecord : linkRecordSet) {
            LinkRecord link = new LinkRecord();
            link.setId(linkRecord.getId());
            link.setFolderId(linkRecord.getFolderId());
            link.setMapId(linkRecord.getMapId());
            linkRecordList.add(link);
        }
        linkRecords.setLinks(linkRecordList.toArray(new LinkRecord[] {}));
        return linkRecords;
    }

    /**
     * Return the map's bounds
     *
     * @return a bounding box
     * @throws Exception
     */
    public BoundingBox getBounds() throws Exception {
        if (bounds == null) {
            final Tuple nodeResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                    .from(currentnodes)
                    .list(currentnodes.longitude.max(), currentnodes.longitude.min(), currentnodes.latitude.max(),
                            currentnodes.latitude.min()).get(0);
            bounds = new BoundingBox(nodeResults.get(1, Double.class), nodeResults.get(3, Double.class),
                    nodeResults.get(0, Double.class), nodeResults.get(2, Double.class));
        }
        return bounds;
    }
}