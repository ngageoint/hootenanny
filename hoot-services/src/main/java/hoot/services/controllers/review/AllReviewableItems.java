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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.review;

import java.util.Map;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import hoot.services.geo.BoundingBox;


public class AllReviewableItems implements ReviewQueryMapper {

    private long mapId;
    private boolean overflow;
    private Map<Long, ReviewableItemBboxInfo> reviewableItems;

    public AllReviewableItems(long mapid, Map<Long, ReviewableItemBboxInfo> reviewableItems) {
        this.mapId = mapid;
        this.reviewableItems = reviewableItems;
    }

    public long getMapId() {
        return mapId;
    }

    public void setMapId(long mapid) {
        this.mapId = mapid;
    }

    public Map<Long, ReviewableItemBboxInfo> getReviewableItems() {
        return reviewableItems;
    }

    public void setReviewableItems(Map<Long, ReviewableItemBboxInfo> reviewableItems) {
        this.reviewableItems = reviewableItems;
    }

    public boolean getOverflow() {
        return overflow;
    }

    public void setOverflow(boolean overflow) {
        this.overflow = overflow;
    }

    @Override
    public String toString() {
        JSONObject o = new JSONObject();
        o.put("mapid", mapId);
        o.put("reviewableItems", reviewableItems);
        return o.toJSONString();
    }

    public JSONObject toGeoJson() {
        JSONArray features = new JSONArray();

        if (reviewableItems != null) {
            for (Map.Entry<Long, ReviewableItemBboxInfo> pair : reviewableItems.entrySet()) {
                Long relId = pair.getKey();
                ReviewableItemBboxInfo info = pair.getValue();

                JSONObject feature = new JSONObject();
                feature.put("type", "Feature");

                JSONObject geometry = new JSONObject();
                geometry.put("type", "Point");
                BoundingBox bbx = info.getBbox();
                double[] coords = { bbx.getCenterX(), bbx.getCenterY() };
                geometry.put("coordinates", coords);
                feature.put("geometry", geometry);

                JSONObject prop = new JSONObject();
                String sClass = "open";
                if (info.getNeedReview().equalsIgnoreCase("no")) {
                    sClass = "resolved";
                }
                prop.put("class", sClass);
                prop.put("relationid", relId);
                feature.put("properties", prop);

                features.add(feature);
            }
        }

        JSONObject geoJson = new JSONObject();
        geoJson.put("type", "FeatureCollection");
        geoJson.put("features", features);

        return geoJson;
    }
}
