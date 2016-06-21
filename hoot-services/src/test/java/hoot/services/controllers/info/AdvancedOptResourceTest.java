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
package hoot.services.controllers.info;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Map;

import org.json.simple.JSONObject;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class AdvancedOptResourceTest {

    @Test
    @Category(UnitTest.class)
    public void testGetOptions() throws Exception {

        AdvancedOptResource adv = new AdvancedOptResource();
        Field field = adv.getClass().getDeclaredField("doc");
        field.setAccessible(true);
        field.set(adv, new JSONObject());

        Method getParseAsciidocMethod = AdvancedOptResource.class.getDeclaredMethod("parseAsciidoc");
        getParseAsciidocMethod.setAccessible(true);
        getParseAsciidocMethod.invoke(adv);

        JSONObject opt = (JSONObject) ((JSONObject) field.get(adv)).get("javascript.translator.path");
        Assert.assertEquals("A list of paths to include in the javascript translator search path.",
                            opt.get("Description").toString());

        JSONObject defList = (JSONObject) opt.get("Default List");

        for (Object o : defList.entrySet()) {
            Map.Entry pair = (Map.Entry) o;
            Assert.assertTrue(!pair.getKey().toString().isEmpty());
            Assert.assertTrue(pair.getValue().toString().isEmpty());
        }
    }
}
