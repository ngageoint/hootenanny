Feature: Translation Assistant

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I click Get Started
        When I select the "sprocket" div

    Scenario: Build an OSM translation mapping for shapefile
        Then I click on the "Translation Assistant" option in the "settingsSidebar"
        Then I upload a zipped shapefile
        Then I wait 30 "seconds" to see "1 of 16 Attributes"
        Then I select the "forwardarrow" div
        Then I click the "plus" classed link under "add-mapping"
        And I fill input under "lookup" with "cityname"
        Then I press tab in the "[placeholder='Search tag']" input
        And I should see "cityname"
        Then I select the "removetag" div
        And I should not see "cityname"
        Then I press "Ignore"
        Then I press "Ignore"
        Then I press "Ignore"
        Then I should see "FCC"
        Then I click the "plus" classed link under "add-mapping"
        And I fill input under "lookup" with "high"
        Then I press the down arrow key on "[placeholder='Search tag']"
        Then I press the down arrow key on "[placeholder='Search tag']"
        And I should see element "[placeholder='Search tag']" with value "aeroway:runway_highend:slope:closure"
        And I fill input under "lookup" with "highway"
        Then I press enter in the "[placeholder='Search tag']" input
        Then I select the "linktag" div
        Then I select the "maptag" div
        Then I select the first "combobox-caret" div
        Then I press the down arrow key on "[id='preset-input-665334034']"
        And I should see element "[id='preset-input-665334034']" with value "crossing"
        Then I press the down arrow key on "[id='preset-input-665334034']"
        Then I press the down arrow key on "[id='preset-input-665334034']"
        Then I press the down arrow key on "[id='preset-input-665334034']"
        Then I press the down arrow key on "[id='preset-input-665334034']"
        Then I press enter in the "[id='preset-input-665334034']" input
        And I should see element "[id='preset-input-665334034']" with value "primary"
        Then I type "secondary" in input "preset-input-665334065"
        Then I press enter in the "[id='preset-input-665334065']" input
        Then I type "roa" in input "preset-input-665334096"
        Then I press enter in the "[id='preset-input-665334096']" input
        Then I click the "plus" classed link under "add-mapping"
        And I fill input under "lookup" with "source:name"
        Then I press enter in the "[placeholder='Search tag']" input
        Then I select the "linktag" div
        Then I type "quickbird" in input "preset-input--118997366"
        Then I click on "a.next"
        Then I should see "ID"
        Then I select the "backarrow" div
        Then I should see "FCC"
        And I should see "A20"
        And I should see element "[id='preset-input-665334034']" with value "primary"
        Then I select the "forwardarrow" div
        Then I press the right arrow key
        And I should see "LEN"
        Then I press the left arrow key
        And I should see "ID"
        Then I select the "attributes-option" div
        Then I click the "div.attributes-option.attributes-hover" with text "☐ NAME_ALT"
        Then I click the "plus" classed link under "add-mapping"
        And I fill input under "lookup" with "name_alt"
        Then I press enter in the "[placeholder='Search tag']" input
        Then I click on "a.next"
        Then I press "Save Translation"
        And I wait 5 seconds
        Then the download file pattern "*-translation.js" should exist
        Then I accept the alert
        Then I select the "x" div

    Scenario: Build an TDS translation mapping for gdb
        Then I click on the "Translation Assistant" option in the "settingsSidebar"
        When I choose "TDSv61" radio button
        Then I upload a zipped folder of shapefiles
        And I wait
        And I should see element "[id='preset-input-layer']" with value "cali-test"
        Then I type "cali-fake" in input "preset-input-layer"
        Then I press tab in the "[id='preset-input-layer']" input
        And I should see element "[id='preset-input-layer']" with value "cali-fake-points"
        And I should see "1 of 3 Attributes"
        Then I press "Ignore"
        And I should see "name"
        Then I click the "plus" classed link under "add-mapping"
        And I fill input under "lookup" with "name"
        And I press escape in the "[placeholder='Search tag']" input
        And I should not see "Aerodrome Official Name"
        Then I click the "plus" classed link under "add-mapping"
        And I fill input under "lookup" with "name"
        Then I press the down arrow key on "[placeholder='Search tag']"
        Then I press the up arrow key on "[placeholder='Search tag']"
        Then I press the down arrow key on "[placeholder='Search tag']"
        Then I press the down arrow key on "[placeholder='Search tag']"
        Then I press the down arrow key on "[placeholder='Search tag']"
        Then I press enter in the "[placeholder='Search tag']" input
        And I should see "Geographic Name Information : Full Name"
        Then I click on "a.next"
        Then I press "Save Translation"
        Then I should see "A mapping for Feature Code is required"
        Then I select the "backarrow" div
        Then I click the "plus" classed link under "add-mapping"
        And I fill input under "lookup" with "Feature Code"
        Then I press enter in the "[placeholder='Search tag']" input
        Then I select the last "linktag" div
        Then I select the last "maptag" div
        Then I select the last "removemaptag" div
        Then I select the last "linktag" div
        Then I type "AP030" in input "preset-input-1313887511"
        Then I press enter in the "[id='preset-input-1313887511']" input
        Then I click on "a.next"
        Then I click the "plus" classed link under "add-mapping"
        And I fill input under "lookup" with "Feature Function"
        Then I select the first "search-result" div
        And I should see "Cave Information : Feature Function"
        Then I press the right arrow key
        Then I press the right arrow key
        Then I press the right arrow key
        Then I press the left arrow key
        Then I press the left arrow key
        Then I press the left arrow key
        Then I press "Save Translation"
        And I wait 5 seconds
        Then the download file pattern "*-translation.js" should exist
        Then I accept the alert
        Then I close the modal

    Scenario: Test an invalid dataset
        Then I click on the "Translation Assistant" option in the "settingsSidebar"
        Then I upload an invalid dataset
        Then I wait 30 "seconds" to see "Job failed"
