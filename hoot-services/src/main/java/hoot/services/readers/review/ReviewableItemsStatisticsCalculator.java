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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.readers.review;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Set;

import hoot.services.db.DbUtils;

import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QReviewItems;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.Map;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.review.ReviewableItemsStatistics;
import hoot.services.validators.osm.MapValidator;
import hoot.services.validators.review.ReviewMapValidator;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.expr.BooleanExpression;

/**
 * Calculates statistics on a set of reviewable data
 */
public class ReviewableItemsStatisticsCalculator
{
  private static final Logger log = LoggerFactory
      .getLogger(ReviewableItemsStatisticsCalculator.class);

  private static final QReviewItems remviewItems = QReviewItems.reviewItems;
  private static final QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;
  private long mapId;

  private Connection conn;

  public ReviewableItemsStatisticsCalculator(final Connection conn, final String mapId,
      final boolean reviewDataMustBePrepared) throws Exception
  {

    this.conn = conn;
    if (reviewDataMustBePrepared)
    {
      // Check to see if the map exists in the maps table. (404); input mapId
      // may be a map ID or a
      // map name; this will throw if it doesn't find the map and review data
      // hasn't been prepared
      // for it
      this.mapId = (new ReviewMapValidator(conn).verifyMapPrepared(mapId));
    }
    else
    {
      // Check to see if the map exists in the maps table. (404);
      this.mapId = (new MapValidator(conn).verifyMapExists(mapId));
    }

    log.debug("Number of elements with review tags for map ID: " + mapId + " = "
        + getNumItemsWithReviewTags());
  }

  /**
   * Returns statistics about a set of reviewable conflated data items for a
   * particular map
   *
   * @param reviewScoreThresholdMinimum
   *          lower review score an item may have and be included in the
   *          calculate statistics; 0 to 1
   * @param geospatialBounds
   *          determines the geospatial bounds a result must reside within to be
   *          included in the calculated statistics; if null then
   *          geospatialBounds = world
   * @return a set of reviewable item statistics
   * @throws Exception
   */
  public ReviewableItemsStatistics getStatistics(final double reviewScoreThresholdMinimum,
      final BoundingBox geospatialBounds) throws Exception
  {
    ReviewableItemsStatistics stats = new ReviewableItemsStatistics();

    // the number of reviewed items should always be determined regardless of
    // filters, so we know
    // know how many items in the layer have been reviewed over time

    final long numReviewedItems = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
        .from(remviewItems)
        .join(elementIdMappings)
        .on(elementIdMappings.elementId.eq(remviewItems.reviewableItemId))
        .where(
            remviewItems.reviewStatus.eq(DbUtils.review_status_enum.reviewed)
                .and(remviewItems.mapId.eq(mapId)).and(elementIdMappings.mapId.eq(mapId))).count();

    int numReviewableItems = 0;
    java.util.Map<ElementType, Set<Long>> elementIdsWithinBoundsByType = null;
    if (!geospatialBounds.equals(BoundingBox.worldBounds()))
    {
      // retrieve the IDs of all elements by type from the map that fall within
      // the requested
      // geospatial bounds
      elementIdsWithinBoundsByType = (new Map(mapId, conn)).queryForElementIds(geospatialBounds);
    }

    for (ElementType elementType : ElementType.values())
    {
      if (!elementType.equals(ElementType.Changeset))
      {
        final Element prototype = ElementFactory.getInstance().create(mapId, elementType, conn);

        /*
         * Condition elementJoinCondition =
         * Tables.ELEMENT_ID_MAPPINGS.OSM_ELEMENT_ID
         * .eq(prototype.getElementIdField())
         * .and(prototype.getMapIdField().eq(mapId));
         */
        BooleanExpression elementJoinCondition = elementIdMappings.osmElementId.eq(
            prototype.getElementIdField());
        if (elementIdsWithinBoundsByType != null
            && elementIdsWithinBoundsByType.get(elementType).size() > 0)
        {
          assert (geospatialBounds != null && !geospatialBounds.equals(BoundingBox.worldBounds()));
          elementJoinCondition = elementJoinCondition.and(prototype.getElementIdField().in(
              elementIdsWithinBoundsByType.get(elementType)));
        }

        numReviewableItems += new SQLQuery(conn, DbUtils.getConfiguration(mapId))
            .from(remviewItems)
            .join(elementIdMappings)
            .on(elementIdMappings.elementId.eq(remviewItems.reviewableItemId))
            .where(
            //
                elementIdMappings.elementId
                    .eq(remviewItems.reviewableItemId)
                    .and(remviewItems.mapId.eq(mapId))
                    .and(elementIdMappings.mapId.eq(mapId))
                    .and(
                        elementIdMappings.osmElementType.eq(Element
                            .elementEnumForElementType(elementType))))
            .join(prototype.getElementTable())
            .on(elementJoinCondition)
            .where(
                remviewItems.reviewStatus.eq(DbUtils.review_status_enum.unreviewed).and(
                    remviewItems.reviewScore.goe(reviewScoreThresholdMinimum))).count();
      }
    }

    int numTotalItems = 0;
    for (ElementType elementType : ElementType.values())
    {
      if (!elementType.equals(ElementType.Changeset))
      {
        final Element prototype = ElementFactory.getInstance().create(mapId, elementType, conn);

        numTotalItems += new SQLQuery(conn, DbUtils.getConfiguration(mapId))
            .from(prototype.getElementTable()).count();
      }
    }

    stats.setMapId(mapId);
    stats.setNumReviewableItems(numReviewableItems);
    stats.setNumReviewedItems((int) numReviewedItems);
    stats.setNumTotalItems(numTotalItems);

    return stats;
  }

  private long getNumItemsWithReviewTags() throws Exception
  {
    long numReviewTags = 0;
    for (ElementType elementType : ElementType.values())
    {
      if (!elementType.equals(ElementType.Changeset))
      {
        final Element prototype = ElementFactory.getInstance().create(mapId, elementType, conn);
        String tableName = prototype.getElementTable().getTableName();

        String POSTGRESQL_DRIVER = "org.postgresql.Driver";
        Statement stmt = null;
        try
        {
          Class.forName(POSTGRESQL_DRIVER);

          stmt = conn.createStatement();

          String sql = "select count(*) from " + tableName + "_" + mapId + " where "
              + " tags->'hoot:review:needs' = 'yes'";
          stmt = conn.createStatement();
          ResultSet rs = stmt.executeQuery(sql);
          rs.next();
          numReviewTags += rs.getInt(1);

        }
        catch (Exception e)
        {
          // throw new Exception("Error inserting node.");
        }

        finally
        {
          // finally block used to close resources
          try
          {
            if (stmt != null)
              stmt.close();
          }
          catch (SQLException se2)
          {

          }// nothing we can do

        }// end try
      }
    }
    return numReviewTags;
  }
}
