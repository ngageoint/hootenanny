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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2017 Maxar (http://www.maxar.com/)
 */
package hoot;

import java.io.IOException;

import junit.framework.TestCase;

public class MultiaryUtilitiesTest extends TestCase
{
  public void setUp()
  {
    HootSwig.init();
  }

  public byte[] decodeBase64(String str)
  {
    return java.util.Base64.getDecoder().decode(str);
  }

  public String encodeBase64(byte[] bytes)
  {
    return new String(java.util.Base64.getEncoder().encode(bytes));
  }

  /**
   * Create the elements from the base64 version of the PBF data. JSON equivalent is provided in
   * the comments for convenience.
   *
   * This adds 3 elements, two are conflated into a single element so the result contains two
   * elements.
   */
  public void testConflateCluster() throws IOException
  {
    TestUtils.resetEnvironment();

    java.util.ArrayList<byte[]> elements = new java.util.ArrayList<byte[]>();

    // { 'elements': [ { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0,
    //   'tags': { 'amenity': 'pub', 'name': 'My Restaurant' } } ] }
    elements.add(java.util.Base64.getDecoder().decode("AAAAVQo5CgAKBG5hbWUKDU15IFJlc3RhdXJhbnQKB2FtZW5pdHkKA3B1YgoOZXJyb3I6Y2lyY3VsYXIKAjE1EhgKFggBEgMBAwUaAwIEBkCAtIkTSP+Nzhw="));

    // {"version": 0.6,"generator": "Hootenanny","elements": [
    // {"type":"node","id":-3,"lat":2,"lon":-3,"tags":
    //   {"hoot:hash":"sha1sum:c40b57f15ac814272b1b4cd42302c29d15cab9d9","is_in:country":"Yemen","name":"My Restaurant","source:url":"http://wikimapia.org/36956748/15-43","amenity":"pub","error:circular":"15"}}]
    // }
    elements.add(java.util.Base64.getDecoder().decode("AAAA4Aq9AQoACglob290Omhhc2gKMHNoYTFzdW06YzQwYjU3ZjE1YWM4MTQyNzJiMWI0Y2Q0MjMwMmMyOWQxNWNhYjlkOQoNaXNfaW46Y291bnRyeQoFWWVtZW4KBG5hbWUKDU15IFJlc3RhdXJhbnQKCnNvdXJjZTp1cmwKI2h0dHA6Ly93aWtpbWFwaWEub3JnLzM2OTU2NzQ4LzE1LTQzCgdhbWVuaXR5CgNwdWIKDmVycm9yOmNpcmN1bGFyCgIxNRIeChwIARIGAQMFBwkLGgYCBAYICgxAgLSJE0j/jc4c"));

    // { 'elements': [ { 'type': 'node', 'id': -1, 'lat': 3.0, 'lon': -3.0,
    //   'tags': { 'amenity': 'pub', 'name': 'My Restaurant' } } ] }
    elements.add(java.util.Base64.getDecoder().decode("AAAAVQo5CgAKBG5hbWUKDU15IFJlc3RhdXJhbnQKB2FtZW5pdHkKA3B1YgoOZXJyb3I6Y2lyY3VsYXIKAjE1EhgKFggBEgMBAwUaAwIEBkCAjs4cSP+Nzhw="));

    java.util.ArrayList<MultiaryElement> result = MultiaryUtilities.conflateCluster(elements);

    assertEquals(2, result.size());
    assertEquals("sha1sum:537bc7a383257c8c1cdf69b461af87344a0a0ba5", result.get(0).getHash());
    assertEquals("AAAApwqGAQoACglob290Omhhc2gKMHNoYTFzdW06NTM3YmM3YTM4MzI1N2M4YzFjZGY2OWI0NjFhZjg3MzQ0YTBhMGJhNQoEbmFtZQoNTXkgUmVzdGF1cmFudAoHYW1lbml0eQoDcHViCg5lcnJvcjpjaXJjdWxhcgoCMTUKC2hvb3Q6c3RhdHVzCgExEhwKGggFEgUBAwUHCRoFAgQGCApAgI7OHEj/jc4c", encodeBase64(result.get(0).getPayload()));
    assertEquals(result.get(0).getBounds().getMinX(), -3.0018, 0.0001);
    assertEquals(result.get(0).getBounds().getMaxX(), -2.9981, 0.0001);
    assertEquals(result.get(0).getBounds().getMinY(), 2.9982, 0.0001);
    assertEquals(result.get(0).getBounds().getMaxY(), 3.0017, 0.0001);

    assertEquals("sha1sum:bd4ba434e321fd3eb0d1597044b962412067fd8a", result.get(1).getHash());
    assertEquals("AAABMwqMAgoACglob290Omhhc2gKMHNoYTFzdW06YmQ0YmE0MzRlMzIxZmQzZWIwZDE1OTcwNDRiOTYyNDEyMDY3ZmQ4YQoLc291cmNlOmhhc2gKMHNoYTFzdW06YzQwYjU3ZjE1YWM4MTQyNzJiMWI0Y2Q0MjMwMmMyOWQxNWNhYjlkOQoNaXNfaW46Y291bnRyeQoFWWVtZW4KBG5hbWUKDU15IFJlc3RhdXJhbnQKCnNvdXJjZTp1cmwKI2h0dHA6Ly93aWtpbWFwaWEub3JnLzM2OTU2NzQ4LzE1LTQzCgdhbWVuaXR5CgNwdWIKDmVycm9yOmNpcmN1bGFyCgIxNQoLaG9vdDpzdGF0dXMKATMSIgogCAESCAEDBQcJCw0PGggCBAYICgwOEECAtIkTSP+Nzhw=", encodeBase64(result.get(1).getPayload()));
    assertEquals(result.get(1).getBounds().getMinX(), -3.0018, 0.0001);
    assertEquals(result.get(1).getBounds().getMaxX(), -2.9982, 0.0001);
    assertEquals(result.get(1).getBounds().getMinY(), 1.9982, 0.0001);
    assertEquals(result.get(1).getBounds().getMaxY(), 2.0018, 0.0001);
  }

  /**
   * Create the elements from the base64 version of the PBF data. JSON equivalent is provided in
   * the comments for convenience.
   *
   */
  public void testFindMatches() throws IOException
  {
    // { 'elements': [ { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0,
    //   'tags': { 'amenity': 'pub', 'name': 'My Restaurant' } } ] }
    byte[] checkElement = java.util.Base64.getDecoder().decode("AAAAVQo5CgAKBG5hbWUKDU15IFJlc3RhdXJhbnQKB2FtZW5pdHkKA3B1YgoOZXJyb3I6Y2lyY3VsYXIKAjE1EhgKFggBEgMBAwUaAwIEBkCAtIkTSP+Nzhw=");

    java.util.ArrayList<byte[]> againstElements = new java.util.ArrayList<byte[]>();

    // {"version": 0.6,"generator": "Hootenanny","elements": [
    // {"type":"node","id":-3,"lat":2,"lon":-3,"tags":
    //   {"hoot:hash":"sha1sum:c40b57f15ac814272b1b4cd42302c29d15cab9d9","is_in:country":"Yemen","name":"My Restaurant","source:url":"http://wikimapia.org/36956748/15-43","amenity":"pub","error:circular":"15"}}]
    // }
    againstElements.add(java.util.Base64.getDecoder().decode("AAAA4Aq9AQoACglob290Omhhc2gKMHNoYTFzdW06YzQwYjU3ZjE1YWM4MTQyNzJiMWI0Y2Q0MjMwMmMyOWQxNWNhYjlkOQoNaXNfaW46Y291bnRyeQoFWWVtZW4KBG5hbWUKDU15IFJlc3RhdXJhbnQKCnNvdXJjZTp1cmwKI2h0dHA6Ly93aWtpbWFwaWEub3JnLzM2OTU2NzQ4LzE1LTQzCgdhbWVuaXR5CgNwdWIKDmVycm9yOmNpcmN1bGFyCgIxNRIeChwIARIGAQMFBwkLGgYCBAYICgxAgLSJE0j/jc4c"));

    // { 'elements': [ { 'type': 'node', 'id': -1, 'lat': 3.0, 'lon': -3.0,
    //   'tags': { 'amenity': 'pub', 'name': 'My Restaurant' } } ] }
    againstElements.add(java.util.Base64.getDecoder().decode("AAAAVQo5CgAKBG5hbWUKDU15IFJlc3RhdXJhbnQKB2FtZW5pdHkKA3B1YgoOZXJyb3I6Y2lyY3VsYXIKAjE1EhgKFggBEgMBAwUaAwIEBkCAjs4cSP+Nzhw="));

    java.util.ArrayList<MultiarySimpleMatch> matches = MultiaryUtilities.findMatches(checkElement,
      againstElements);

    assertEquals(1, matches.size());
    assertEquals(0, matches.get(0).getNeighborIndex());
    assertEquals(0.757575, matches.get(0).getScore(), 0.001);
  }

  /**
   * Verify reading and writing to the envelope.
   */
  public void testMultiaryElement()
  {
    double e = 0.0001;
    MultiaryElement me = new MultiaryElement();
    me.setBounds(new Envelope(1, 2, 3, 4));
    assertEquals(me.getBounds().getMinX(), 1, e);
    assertEquals(me.getBounds().getMaxX(), 2, e);
    assertEquals(me.getBounds().getMinY(), 3, e);
    assertEquals(me.getBounds().getMaxY(), 4, e);
  }
}
