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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.conflation;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;


public class ConflateUtilsTest {

    @Test
    public void isAtLeastOneLayerOsmApiDb() {
        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("1");
        conflateParams.setInputType2("OSM_API_DB");
        conflateParams.setInput2("-1");
        conflateParams.setOutputName("OutputLayer");
        conflateParams.setCollectStats(false);
        conflateParams.setAdvancedOptions("-D convert.bounding.box=0,0,0,0");

        assertTrue(ConflateUtils.isAtLeastOneLayerOsmApiDb(conflateParams));

        conflateParams.setInputType1("OSM_API_DB");
        conflateParams.setInputType2("DB");

        assertTrue(ConflateUtils.isAtLeastOneLayerOsmApiDb(conflateParams));

        conflateParams.setInputType1("OSM_API_DB");
        conflateParams.setInputType2("OSM_API_DB");

        assertTrue(ConflateUtils.isAtLeastOneLayerOsmApiDb(conflateParams));

        conflateParams.setInputType1("DB");
        conflateParams.setInputType2("DB");

        assertFalse(ConflateUtils.isAtLeastOneLayerOsmApiDb(conflateParams));
    }

    @Test
    public void isFirstLayerOsmApiDb() {
        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("1");
        conflateParams.setInputType2("OSM_API_DB");
        conflateParams.setInput2("-1");
        conflateParams.setOutputName("OutputLayer");
        conflateParams.setCollectStats(false);
        conflateParams.setAdvancedOptions("-D convert.bounding.box=0,0,0,0");

        assertFalse(ConflateUtils.isFirstLayerOsmApiDb(conflateParams));

        conflateParams.setInputType1("OSM_API_DB");
        conflateParams.setInputType2("DB");

        assertTrue(ConflateUtils.isFirstLayerOsmApiDb(conflateParams));
    }

    @Test
    public void isSecondLayerOsmApiDb() {
        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("1");
        conflateParams.setInputType2("OSM_API_DB");
        conflateParams.setInput2("-1");
        conflateParams.setOutputName("OutputLayer");
        conflateParams.setCollectStats(false);
        conflateParams.setAdvancedOptions("-D convert.bounding.box=0,0,0,0");

        assertTrue(ConflateUtils.isSecondLayerOsmApiDb(conflateParams));

        conflateParams.setInputType1("OSM_API_DB");
        conflateParams.setInputType2("DB");

        assertFalse(ConflateUtils.isSecondLayerOsmApiDb(conflateParams));
    }

    @Test(expected = IllegalArgumentException.class)
    public void validateOsmApiDbConflateParamsWithException() {
        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("1");
        conflateParams.setInputType2("OSM_API_DB");
        conflateParams.setInput2("-1");
        conflateParams.setOutputName("OutputLayer");
        conflateParams.setCollectStats(false);
        conflateParams.setAdvancedOptions("-D convert.bounding.box=0,0,0,0");

        ConflateUtils.validateOsmApiDbConflateParams(conflateParams);
    }

    @Test
    public void validateOsmApiDbConflateParamsWithoutException() {
        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("1");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2("-1");
        conflateParams.setOutputName("OutputLayer");
        conflateParams.setCollectStats(false);
        conflateParams.setAdvancedOptions("-D convert.bounding.box=0,0,0,0");

        ConflateUtils.validateOsmApiDbConflateParams(conflateParams);
    }
}