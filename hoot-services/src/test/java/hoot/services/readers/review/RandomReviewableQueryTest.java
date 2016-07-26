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
package hoot.services.readers.review;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class RandomReviewableQueryTest {

    @Test
    @Category(UnitTest.class)
    public void testGetQuery() throws Exception {
        String expected = "select id as relid, tags->'hoot:review:sort_order' as seq from current_relations_10 "
                + "where tags->'hoot:review:needs' = 'yes' order by random() limit 1";
        RandomReviewableQuery q = new RandomReviewableQuery(null, 10);
        String actual = q.getQueryString();

        org.junit.Assert.assertEquals(expected, actual);
    }
}
