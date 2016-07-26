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

import java.sql.Connection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.TreeMap;

import org.json.simple.JSONObject;
import org.postgresql.util.PGobject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.QTuple;
import com.mysema.query.types.expr.BooleanExpression;

import hoot.services.db2.Maps;
import hoot.services.db2.QChangesets;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentRelations;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.db2.QCurrentWays;
import hoot.services.db2.QMaps;
import hoot.services.db2.QUsers;
import hoot.services.geo.BoundingBox;
import hoot.services.geo.zindex.Range;
import hoot.services.geo.zindex.ZCurveRanger;
import hoot.services.geo.zindex.ZValue;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.utils.DbUtils;
import hoot.services.utils.DbUtils.nwr_enum;
import hoot.services.utils.PostgresUtils;


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
    private static final Logger logger = LoggerFactory.getLogger(Map.class);
    private static final QCurrentNodes currentNodes = QCurrentNodes.currentNodes;

    private final Connection conn;
    private BoundingBox bounds;

    public Map(long id, Connection conn) {
        setId(id);
        this.conn = conn;
    }

    /*
     * Retrieves all ranges of quad tiles that fall within the bounds
     */
    private static List<Range> getTileRanges(BoundingBox bounds) {
        logger.debug("Retrieving tile ranges...");
        int queryDimensions = Integer.parseInt(MAP_QUERY_DIMENSIONS);
        ZCurveRanger ranger = new ZCurveRanger(new ZValue(queryDimensions, 16,
        // use y, x ordering here
                new double[] { -1 * BoundingBox.LAT_LIMIT, -1 * BoundingBox.LON_LIMIT }, new double[] {
                        BoundingBox.LAT_LIMIT, BoundingBox.LON_LIMIT }));
        return ranger.decomposeRange(bounds.toZIndexBox(), 1);
    }

    /*
     * Returns the SQL where condition for the calculated tile ranges
     */
    private static BooleanExpression getTileWhereCondition(List<Range> tileIdRanges) {
        List<BooleanExpression> tileConditions = new ArrayList<>();
        for (Range range : tileIdRanges) {
            tileConditions.add(currentNodes.tile.goe(range.getMin()).and(currentNodes.tile.loe(range.getMax())));
        }

        BooleanExpression combinedTileCondition = null;
        for (BooleanExpression tileCondition : tileConditions) {
            combinedTileCondition = (combinedTileCondition == null) ? tileCondition : combinedTileCondition.or(tileCondition);
        }

        return combinedTileCondition;
    }

    /*
     * Returns the geospatial where condition to apply with the tile conditions
     * to ensure nodes that fall outside the bounding box are not returned
     */
    private static BooleanExpression getGeospatialWhereCondition(BoundingBox bounds) {
        return currentNodes.longitude.goe(bounds.getMinLon()).and(currentNodes.latitude.goe(bounds.getMinLat()))
                .and(currentNodes.longitude.loe(bounds.getMaxLon())).and(currentNodes.latitude.loe(bounds.getMaxLat()));
    }

    private static void validateQueryBounds(BoundingBox bounds) {
        logger.debug("Checking request bounds size...");
        double maxQueryAreaDegrees = Double.parseDouble(MAP_QUERY_AREA_DEGREES);
        double requestedArea = bounds.getArea();
        if (requestedArea > maxQueryAreaDegrees) {
            throw new IllegalArgumentException("The maximum bbox size is: " + maxQueryAreaDegrees +
                    ", and your request was too large at " + requestedArea + " degrees.  Request a smaller area.");
        }
    }

    /*
     * This does a check to see how many nodes are in the query and throws an
     * exception if over the configured maximum. This only needs to be used for
     * actual map queries returning XML. If you're just doing a node count
     * check, then getNodeCount should be used.
     */
    private void validateNodeCount(BooleanExpression combinedGeospatialCondition) {
        logger.debug("Retrieving node count...");

        long nodeCount = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                .from(currentNodes)
                .where(combinedGeospatialCondition.and(currentNodes.visible.eq(true)))
                .count();

        // The max node count only applies to the nodes falling within the query
        // bounds, not those that belong to ways that cross the query bounds but fall
        // outside of the query bounds, even though those nodes are returned as well in the query.
        long maxQueryNodes = Long.parseLong(MAX_QUERY_NODES);
        if (nodeCount > maxQueryNodes) {
            throw new IllegalArgumentException("The maximum number of nodes that may be returned in a map query is " +
                    maxQueryNodes + ".  This query returned " + nodeCount + " nodes.  Please " +
                    "execute a query which returns fewer nodes.");
        }
    }

    public JSONObject retrieveNodesMBR(BoundingBox bounds) {
        JSONObject ret = new JSONObject();

        // get the intersecting tile ranges for the nodes
        List<Range> tileIdRanges = getTileRanges(bounds);
        if (!tileIdRanges.isEmpty()) {
            BooleanExpression combinedGeospatialCondition = getTileWhereCondition(tileIdRanges).and(
                    getGeospatialWhereCondition(bounds));

            List<Tuple> geospatialQueryNodeResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                    .from(currentNodes)
                    .where(combinedGeospatialCondition.and(currentNodes.visible.eq(true)))
                    .list(currentNodes.longitude.max(), currentNodes.longitude.min(), currentNodes.latitude.max(),
                            currentNodes.latitude.min());

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

    public long getNodesCount(BoundingBox bounds) {
        long ret = 0;

        // get the intersecting tile ranges for the nodes
        List<Range> tileIdRanges = getTileRanges(bounds);
        if (!tileIdRanges.isEmpty()) {
            BooleanExpression combinedGeospatialCondition = getTileWhereCondition(tileIdRanges).and(
                    getGeospatialWhereCondition(bounds));
            ret = new SQLQuery(conn, DbUtils.getConfiguration(getId())).from(currentNodes)
                    .where(combinedGeospatialCondition.and(currentNodes.visible.eq(true))).count();
        }

        return ret;
    }

    public JSONObject retrieveANode(BoundingBox bounds) {
        JSONObject ret = new JSONObject();

        // get the intersecting tile ranges for the nodes
        List<Range> tileIdRanges = getTileRanges(bounds);
        if (!tileIdRanges.isEmpty()) {
            BooleanExpression combinedGeospatialCondition = getTileWhereCondition(tileIdRanges).and(
                    getGeospatialWhereCondition(bounds));

            List<Tuple> geospatialQueryNodeResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                    .from(currentNodes).where(combinedGeospatialCondition.and(currentNodes.visible.eq(true))).limit(1)
                    .list(currentNodes.longitude, currentNodes.latitude);

            Double lon = geospatialQueryNodeResults.get(0).get(0, Double.class);
            Double lat = geospatialQueryNodeResults.get(0).get(1, Double.class);
            ret.put("lon", lon);
            ret.put("lat", lat);
        }

        return ret;
    }

    private java.util.Map<ElementType, java.util.Map<Long, Tuple>> retrieveElements(
            BooleanExpression combinedGeospatialCondition) {
        // There is probably a way to get all this information by executing fewer queries...

        // if the limit hasn't been exceeded, query out all nodes which fall
        // within the geospatial bounds, are visible, and belong to this map; join in user info
        logger.debug("Retrieving node records within the query bounds...");

        QChangesets changesets = QChangesets.changesets;
        QUsers users = QUsers.users;

        java.util.Map<Long, Tuple> geospatialQueryNodeResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                .from(currentNodes)
                .join(changesets).on(currentNodes.changesetId.eq(changesets.id))
                .join(users).on(changesets.userId.eq(users.id))
                .where(combinedGeospatialCondition.and(currentNodes.visible.eq(true)))
                .orderBy(currentNodes.id.asc())
                .map(currentNodes.id, new QTuple(currentNodes, users, changesets));

        // I want to retrieve nodes, ways, and relations with one query and return a
        // single set of records. However, there is no type information in the records and
        // the fields between them aren't unique (way and relation have the same database field names),
        // so there is no way for the response writer to know what type of element to write.
        // Instead, I'm doing separate queries for each element type and returning them in a collection
        // grouped by element type.
        // JDBC prepared statement number of parameters has a hard limit: 32767
        java.util.Map<ElementType, java.util.Map<Long, Tuple>> elementIdsToRecordsByType = new HashMap<>();

        elementIdsToRecordsByType.put(ElementType.Node, geospatialQueryNodeResults);

        if (!geospatialQueryNodeResults.isEmpty()) {
            // get all ways which have way nodes with IDs in the previously
            // queried node results, are visible, and belong to this map; join in user info
            logger.debug("Retrieving way IDs within the query bounds...");
            QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;

            List<Long> nodesList = new ArrayList<>(geospatialQueryNodeResults.keySet());

            int totalNodeCnt = nodesList.size();
            int nQueryPageSize = 32000;
            int pageCnt = (int) Math.floor(totalNodeCnt / nQueryPageSize);

            List<Long> wayIds = new ArrayList<>();
            for (int i = 0; i <= pageCnt; i++) {
                int iStart = i * nQueryPageSize;
                int iEnd = iStart + nQueryPageSize;

                List<Long> pageList = nodesList.subList(iStart, (i < pageCnt) ? iEnd : totalNodeCnt);

                if ((pageList == null) || (pageList.isEmpty())) {
                    continue;
                }
                List<Long> pageWayIds = new SQLQuery(conn, DbUtils.getConfiguration(getId())).from(currentWayNodes)
                        .where(currentWayNodes.nodeId.in(pageList)).list(currentWayNodes.wayId);

                // TODO: should this be an assert instead? Regardless, fix the error handling.
                if (!wayIds.addAll(pageWayIds)) {
                    // error
                }
            }

            if (!wayIds.isEmpty()) {
                // from the way results, filter down those that are visible and
                // belong to this map; join in
                // user info
                logger.debug("Retrieving ways within the query bounds...");
                QCurrentWays currentWays = QCurrentWays.currentWays;

                java.util.Map<Long, Tuple> wayResults = null;
                int totalWayCnt = wayIds.size();
                int wayPageCnt = (int) Math.floor(totalWayCnt / nQueryPageSize);

                for (int i = 0; i <= wayPageCnt; i++) {
                    int iStart = i * nQueryPageSize;
                    int iEnd = iStart + nQueryPageSize;

                    List<Long> pageList = wayIds.subList(iStart, (i < wayPageCnt) ? iEnd : totalWayCnt);

                    if ((pageList == null) || (pageList.isEmpty())) {
                        continue;
                    }

                    if (i == 0) {
                        wayResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                .from(currentWays)
                                .join(changesets).on(currentWays.changesetId.eq(changesets.id))
                                .join(users).on(changesets.userId.eq(users.id))
                                .where(currentWays.visible.eq(true).and(currentWays.id.in(pageList)))
                                .orderBy(currentWays.id.asc())
                                .map(currentWays.id, new QTuple(currentWays, users, changesets));
                    }
                    else {
                        wayResults.putAll(new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                .from(currentWays)
                                .join(changesets).on(currentWays.changesetId.eq(changesets.id))
                                .join(users).on(changesets.userId.eq(users.id))
                                .where(currentWays.visible.eq(true).and(currentWays.id.in(pageList)))
                                .orderBy(currentWays.id.asc())
                                .map(currentWays.id, new QTuple(currentWays, users, changesets)));
                    }
                }

                elementIdsToRecordsByType.put(ElementType.Way, wayResults);

                // retrieve all way nodes for the previously retrieved ways
                logger.debug("Retrieving additional way nodes IDs within the query bounds...");
                Set<Long> wayNodeIds = new HashSet<>();
                for (int i = 0; i <= wayPageCnt; i++) {
                    try {
                        List<Long> pageList;
                        int iStart = i * nQueryPageSize;

                        if (i < wayPageCnt) {
                            int iEnd = iStart + nQueryPageSize;
                            pageList = wayIds.subList(iStart, iEnd);
                        }
                        else {
                            pageList = wayIds.subList(iStart, totalWayCnt);
                        }

                        if ((pageList == null) || (pageList.isEmpty())) {
                            continue;
                        }

                        wayNodeIds.addAll(new HashSet<>(new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                .from(currentWayNodes)
                                .where(currentWayNodes.wayId.in(pageList))
                                .list(currentWayNodes.nodeId)));
                    }
                    catch (Exception ex) {
                        logger.error(ex.getMessage(), ex);
                    }
                }

                long numWayNodes = wayNodeIds.size();
                logger.debug("Found {} way nodes.", numWayNodes);

                // retrieve all corresponding nodes to the IDs in the wayNodeIds
                // collection that don't fall within the query bounds (all the way nodes referenced by
                // all the ways minus the nodes falling within the query bounds that have already been
                // added);
                // join in user info;
                // add the resulting nodes to the nodes collection
                Set<Long> geospatialQueryNodeResultIds = new HashSet<>(geospatialQueryNodeResults.keySet());
                long numGeospatialQueryNodeResults = geospatialQueryNodeResultIds.size();
                logger.debug("There were {} nodes returned in the geospatial query.", numGeospatialQueryNodeResults);
                Set<Long> additionalNodeIds = new HashSet<>(wayNodeIds);
                additionalNodeIds.removeAll(geospatialQueryNodeResultIds);

                logger.debug("Subtracting the geospatial query nodes from the way nodes collection results in a total " +
                        "of {} nodes remaining to be added to the nodes collection.", additionalNodeIds.size());
                if (!additionalNodeIds.isEmpty()) {
                    logger.debug("Retrieving nodes falling outside of the query bounds but belonging to a " +
                            "retrieved way...");

                    java.util.Map<Long, Tuple> additionalNodeResults = null;

                    List<Long> additionalNodeIdsList = new ArrayList<>(additionalNodeIds);

                    int totalANILCnt = additionalNodeIdsList.size();
                    int anilPageCnt = (int) Math.floor(totalANILCnt / nQueryPageSize);

                    for (int i = 0; i <= anilPageCnt; i++) {
                        int iStart = i * nQueryPageSize;
                        int iEnd = iStart + nQueryPageSize;

                        List<Long> pageList = additionalNodeIdsList.subList(iStart, (i < anilPageCnt) ? iEnd : totalANILCnt);

                        if ((pageList == null) || (pageList.isEmpty())) {
                            continue;
                        }

                        if (i == 0) {
                            additionalNodeResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                    .from(currentNodes)
                                    .join(changesets).on(currentNodes.changesetId.eq(changesets.id))
                                    .join(users).on(changesets.userId.eq(users.id))
                                    .where(currentNodes.visible.eq(true).and(currentNodes.id.in(pageList)))
                                    .orderBy(currentNodes.id.asc())
                                    .map(currentNodes.id, new QTuple(currentNodes, users, changesets));
                        }
                        else {
                            additionalNodeResults.putAll(new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                    .from(currentNodes)
                                    .join(changesets).on(currentNodes.changesetId.eq(changesets.id))
                                    .join(users).on(changesets.userId.eq(users.id))
                                    .where(currentNodes.visible.eq(true).and(currentNodes.id.in(pageList)))
                                    .orderBy(currentNodes.id.asc())
                                    .map(currentNodes.id, new QTuple(currentNodes, users, changesets)));
                        }
                    }

                    java.util.Map<Long, Tuple> nodes = elementIdsToRecordsByType.get(ElementType.Node);
                    nodes.putAll(additionalNodeResults);

                    // want to keep the nodes ordered by ID, so add them back in
                    // with a TreeMap
                    elementIdsToRecordsByType.put(ElementType.Node, new TreeMap<>(nodes));
                }
            }
            else {
                elementIdsToRecordsByType.put(ElementType.Way, new HashMap<Long, Tuple>());
            }

            /*
             * retrieve all relations that reference the nodes or ways
             * previously retrieved
             */
            logger.debug("Retrieving relations IDs within the query bounds...");

            Set<Long> nodesset = elementIdsToRecordsByType.get(ElementType.Node).keySet();
            Set<Long> wayset = elementIdsToRecordsByType.get(ElementType.Way).keySet();

            if ((nodesset != null) && (nodesset.isEmpty())) {
                nodesset = new HashSet<>();
                nodesset.add(-1L);
            }

            if ((wayset != null) && (wayset.isEmpty())) {
                wayset = new HashSet<>();
                wayset.add(-1L);
            }

            Set<Long> relationIds = new HashSet<>();
            if (!elementIdsToRecordsByType.get(ElementType.Node).keySet().isEmpty()) {
                List<Long> nodessetList = new ArrayList<>(nodesset);

                int totalNodeSetCnt = nodessetList.size();
                int nodesetPageCnt = (int) Math.floor(totalNodeSetCnt / nQueryPageSize);
                Set<Long> nodeSetRelationIds = new HashSet<>();
                QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
                for (int i = 0; i <= nodesetPageCnt; i++) {
                    List<Long> pageList;
                    int iStart = i * nQueryPageSize;

                    if (i < nodesetPageCnt) {
                        int iEnd = iStart + nQueryPageSize;
                        pageList = nodessetList.subList(iStart, iEnd);
                    }
                    else {
                        pageList = nodessetList.subList(iStart, totalNodeSetCnt);
                    }

                    if ((pageList == null) || (pageList.isEmpty())) {
                        continue;
                    }
                    nodeSetRelationIds.addAll(new HashSet<>(new SQLQuery(conn, DbUtils
                            .getConfiguration(getId()))
                            .from(currentRelationMembers)
                            .where(currentRelationMembers.memberId.in(pageList).and(
                                    currentRelationMembers.memberType.eq(nwr_enum.node)))
                            .list(currentRelationMembers.relationId)));
                }

                relationIds.addAll(nodeSetRelationIds);

                List<Long> waysetList = new ArrayList<>(wayset);
                int totalWaySetCnt = waysetList.size();
                int waysetPageCnt = (int) Math.floor(totalWaySetCnt / nQueryPageSize);
                Set<Long> waySetRelationIds = new HashSet<>();

                for (int i = 0; i <= waysetPageCnt; i++) {
                    int iStart = i * nQueryPageSize;
                    int iEnd = iStart + nQueryPageSize;

                    List<Long> pageList = waysetList.subList(iStart, (i < waysetPageCnt) ? iEnd : totalWaySetCnt);

                    if ((pageList == null) || (pageList.isEmpty())) {
                        continue;
                    }

                    waySetRelationIds.addAll(new HashSet<>(
                            new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                    .from(currentRelationMembers)
                                    .where(currentRelationMembers.memberId.in(pageList).and(
                                            currentRelationMembers.memberType.eq(nwr_enum.way)))
                                    .list(currentRelationMembers.relationId)));
                }

                relationIds.addAll(waySetRelationIds);
            }

            if (!relationIds.isEmpty()) {
                logger.debug("Retrieving relations within the query bounds...");

                QCurrentRelations currentRelations = QCurrentRelations.currentRelations;

                java.util.Map<Long, Tuple> relationResults = null;

                List<Long> relationIdsList = new ArrayList<>(relationIds);

                int totalRelationIdsListCnt = relationIdsList.size();
                int relationIdsPageCnt = (int) Math.floor(totalRelationIdsListCnt / nQueryPageSize);

                for (int i = 0; i <= relationIdsPageCnt; i++) {
                    int iStart = i * nQueryPageSize;
                    int iEnd = iStart + nQueryPageSize;

                    List<Long> pageList = relationIdsList.subList(iStart, (i < relationIdsPageCnt) ? iEnd : totalRelationIdsListCnt);

                    if ((pageList == null) || (pageList.isEmpty())) {
                        continue;
                    }

                    if (i == 0) {
                        relationResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                .from(currentRelations)
                                .join(changesets).on(currentRelations.changesetId.eq(changesets.id))
                                .join(users).on(changesets.userId.eq(users.id))
                                .where(currentRelations.visible.eq(true).and(currentRelations.id.in(pageList)))
                                .orderBy(currentRelations.id.asc())
                                .map(currentRelations.id, new QTuple(currentRelations, users, changesets));
                    }
                    else {
                        relationResults.putAll(new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                                .from(currentRelations)
                                .join(changesets).on(currentRelations.changesetId.eq(changesets.id))
                                .join(users).on(changesets.userId.eq(users.id))
                                .where(currentRelations.visible.eq(true).and(currentRelations.id.in(pageList)))
                                .orderBy(currentRelations.id.asc())
                                .map(currentRelations.id, new QTuple(currentRelations, users, changesets)));
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
     * @return a collection of elements mapped to their IDs, grouped by element type
     */
    public java.util.Map<ElementType, java.util.Map<Long, Tuple>> query(BoundingBox bounds) {
        // validateQueryBounds(bounds);

        // get the intersecting tile ranges for the nodes
        List<Range> tileIdRanges = getTileRanges(bounds);
        java.util.Map<ElementType, java.util.Map<Long, Tuple>> elementIdsToRecordsByType = new HashMap<>();
        if (!tileIdRanges.isEmpty()) {
            BooleanExpression combinedGeospatialCondition = getTileWhereCondition(tileIdRanges).and(
                    getGeospatialWhereCondition(bounds));
            validateNodeCount(combinedGeospatialCondition);
            elementIdsToRecordsByType = retrieveElements(combinedGeospatialCondition);
        }

        return elementIdsToRecordsByType;
    }

    private java.util.Map<ElementType, Set<Long>> retrieveElementIds(BooleanExpression combinedGeospatialCondition) {
        // if the limit hasn't been exceeded, query out all nodes which fall
        // within the geospatial bounds, are visible, and belong to this map
        logger.debug("Retrieving IDs of nodes within the query bounds...");

        Set<Long> nodeIds = new HashSet<>(new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                .from(currentNodes)
                .where(combinedGeospatialCondition.and(currentNodes.visible.eq(true)))
                .list(currentNodes.id));

        java.util.Map<ElementType, Set<Long>> elementIdsByType = new HashMap<>();
        elementIdsByType.put(ElementType.Node, nodeIds);

        if (!nodeIds.isEmpty()) {
            // get all ways which have way nodes that belong to the previously
            // queried node results, are visible, and belong to this map; join in user info
            logger.debug("Retrieving IDs of ways within the query bounds...");
            QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;

            Set<Long> wayIds = new HashSet<>(new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                    .from(currentWayNodes)
                    .where(currentWayNodes.nodeId.in(nodeIds))
                    .list(currentWayNodes.wayId));

            elementIdsByType.put(ElementType.Way, wayIds);

            /*
             * retrieve all relations that reference the nodes or ways
             * previously retrieved
             */
            logger.debug("Retrieving relations IDs within the query bounds...");

            Set<Long> nodesIds = elementIdsByType.get(ElementType.Node);
            Set<Long> waysIds = elementIdsByType.get(ElementType.Way);

            // if the Set is empty the in statement blows up..
            if (nodesIds.isEmpty()) {
                nodesIds.add(-1L);
            }

            if (waysIds.isEmpty()) {
                waysIds.add(-1L);
            }

            QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
            Set<Long> relationIds = new HashSet<>(new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                    .from(currentRelationMembers)
                    .where(currentRelationMembers.memberId
                            .in(nodesIds)
                            .and(currentRelationMembers.memberType.eq(nwr_enum.node))
                            .or(currentRelationMembers.memberId.in(waysIds).and(
                                    currentRelationMembers.memberType.eq(nwr_enum.way))))
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
     */
    public java.util.Map<ElementType, Set<Long>> queryForElementIds(BoundingBox bounds) {
        // TODO: add transaction; verify that no other writes are seen during
        // this transaction

        validateQueryBounds(bounds);

        // get the intersecting tile ranges for the nodes
        List<Range> tileIdRanges = getTileRanges(bounds);
        java.util.Map<ElementType, Set<Long>> elementIdsByType = new HashMap<>();

        if (!tileIdRanges.isEmpty()) {
            BooleanExpression combinedGeospatialCondition = getTileWhereCondition(tileIdRanges).and(
                    getGeospatialWhereCondition(bounds));
            validateNodeCount(combinedGeospatialCondition);
            elementIdsByType = retrieveElementIds(combinedGeospatialCondition);
        }

        return elementIdsByType;
    }

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
        List<MapLayer> mapLayerList = new ArrayList<>();
        
        boolean osmApiDbEnabled = Boolean.parseBoolean(OSM_API_DB_ENABLED);
        
        if (osmApiDbEnabled) {
            // add a OSM API db dummy record for the UI for conflation involving OSM API db data
            MapLayer mapLayer = new MapLayer();
            mapLayer.setId(-1); // using id = -1 to identify the OSM API db source layer in the ui
            mapLayer.setName("OSM_API_DB_" + OSM_API_DB_NAME);
            mapLayerList.add(mapLayer);
        }

        for (Maps mapLayerRecord : mapLayerRecords) {
            MapLayer mapLayer = new MapLayer();
            mapLayer.setId(mapLayerRecord.getId());
            mapLayer.setName(mapLayerRecord.getDisplayName());
            mapLayer.setDate(mapLayerRecord.getCreatedAt());

            if (osmApiDbEnabled) {
                java.util.Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) mapLayerRecord.getTags());
                //This tag, set during conflation, is what indicates whether a conflated dataset
                //had any osm api db source data in it.  That is the requirement to export back
                //into an osm api db.
                if (tags.containsKey("osm_api_db_export_time")) {
                    mapLayer.setCanExportToOsmApiDb(true);
                }
            }

            mapLayerList.add(mapLayer);
        } 
        
        mapLayers.setLayers(mapLayerList.toArray(new MapLayer[mapLayerList.size()]));

        return mapLayers;
    }

    /**
     * Return the map's bounds
     * 
     * @return a bounding box
     */
    public BoundingBox getBounds() {
        if (bounds == null) {
            Tuple nodeResults = new SQLQuery(conn, DbUtils.getConfiguration(getId()))
                    .from(currentNodes)
                    .list(currentNodes.longitude.max(), currentNodes.longitude.min(), currentNodes.latitude.max(),
                            currentNodes.latitude.min()).get(0);

            bounds = new BoundingBox(nodeResults.get(1, Double.class), nodeResults.get(3, Double.class),
                    nodeResults.get(0, Double.class), nodeResults.get(2, Double.class));
        }
        return bounds;
    }

    public static boolean mapExists(long id, Connection conn) {
        return new SQLQuery(conn, DbUtils.getConfiguration()).from(QMaps.maps).where(QMaps.maps.id.eq(id)).count() > 0;
    }
}