package hoot.services.controllers.grail;

import static org.junit.Assert.assertEquals;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import org.junit.Test;

public class PullConnectedWaysTest {

    @Test
    public void testGetOsmXpath() throws IOException {
        InputStream is;

        //Read the crop.osm file
        is = new FileInputStream(new File("src/test/resources/hoot/services/controllers/grail/crop.osm"));
        List<Long> nodeIds = PullConnectedWaysCommand.getOsmXpath(is, "/osm/node/@id");
        assertEquals(77, nodeIds.size());
        is.close();

        is = new FileInputStream(new File("src/test/resources/hoot/services/controllers/grail/crop.osm"));
        List<Long> wayIds = PullConnectedWaysCommand.getOsmXpath(is, "/osm/way/@id");
        assertEquals(0, wayIds.size());
        is.close();

        //Read the reference.osm file
        is = new FileInputStream(new File("src/test/resources/hoot/services/controllers/grail/reference.osm"));
        nodeIds = PullConnectedWaysCommand.getOsmXpath(is, "/osm/node/@id");
        assertEquals(91, nodeIds.size());
        is.close();

        is = new FileInputStream(new File("src/test/resources/hoot/services/controllers/grail/reference.osm"));
        wayIds = PullConnectedWaysCommand.getOsmXpath(is, "/osm/way/@id");
        assertEquals(6, wayIds.size());
        is.close();
    }

}
