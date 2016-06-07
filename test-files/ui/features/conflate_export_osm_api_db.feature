Feature: Conflate and Export OSM API Database Dataset

    Scenario: Conflate an OSM API Database Dataset with a Hootenanny Dataset and Export the Conflated Result Back Into the OSM API Database
    	Given I am on Hootenanny
    	And I resize the window
    	And I click Get Started
    	And I press "Add Reference Dataset"
    	And I click the "OSM_API_DB_osmapi_test" Dataset
    	And I press "Add Layer"
    	Then I wait 30 "seconds" to see "OSM_API_DB_osmapi_test"
    	And I press "Add Secondary Dataset"
    	And I click the "AllDataTypesBCucumber" Dataset
    	And I press "Add Layer"
    	Then I wait 30 "seconds" to see "AllDataTypesBCucumber"
    	Then I should see "Conflate"
    	And I press "Conflate"
        #Then I fill "saveAs" input with "OSM_API_DB_AllDataTypes"
        # This was only working some of the time when used with filling the "saveAs" input, so stopped filling the input and 
        # now set it manually in Conflate.js instead.
    	And I scroll element into view and press "conflate2"
    	Then I wait 30 "seconds" to see "Conflating …"
    	Then I wait 3 "minutes" to see "osmapi_test_AllDataTypesBCucumber"
    	Then I should see "Complete Review"
    	Then I wait 30 "seconds" to see "Reviews remaining:"
    	Then I should see "Complete Review"
    	And I click the "Complete Review" link
    	Then I should see "Resolve all remaining reviews"
    	And I press "Resolve all remaining reviews"
    	Then I wait 30 "seconds" to not see "Reviews remaining:"
    	Then I should see "All Reviews Resolved!" 
        # It's actually easier, given existing test code, to jump over to the Datasets tab to test the export.
        # Technically, the export should also be tested from the left hand pane, since the code paths are
        # different, but I couldn't get a test export from there to work due to the fact that the file export 
        # type control is not a combobox there like it is when exporting from the Datasets tab.
        Then I select the "sprocket" div
        When I click the "osmapi_test_AllDataTypesBCucumber" Dataset
        And I context click the "osmapi_test_AllDataTypesBCucumber" Dataset
        And I click the "Export" context menu item
        Then I should see element "[id='fileExportOutputName']" with value "osmapi_test_AllDataTypesBCucumber"
        When I select the "OSM API Database" option in the "File Geodatabase" combobox
        And I press "Export" big loud span
        # TODO: this doesn't work
        # Then I wait 30 "seconds" to see "Successful Export to an OSM API Database"

    Scenario: Conflate With an OSM API Database Dataset as the Secondary Layer
    	Given I am on Hootenanny
    	And I resize the window
    	And I click Get Started
    	And I press "Add Reference Dataset"
    	And I click the "DcTigerRoadsCucumber" Dataset
    	And I press "Add Layer"
    	Then I wait 30 "seconds" to see "DcTigerRoadsCucumber"
    	And I press "Add Secondary Dataset"
    	And I click the "OSM_API_DB_osmapi_test" Dataset
    	And I press "Add Layer"
    	Then I wait 30 "seconds" to see "OSM_API_DB_osmapi_test"
    	Then I should see "Conflate"
    	And I press "Conflate"
        # TODO: this doesn't work
        # Then I wait 30 "seconds" to see "OSM API database not allowed as secondary layer input"
        # Then I should see "OSM API database not allowed as secondary layer input"

    Scenario: Export Dataset into an OSM API Database With Conflicts
    	Given I am on Hootenanny
    	And I resize the window
    	And I click Get Started
    	And I press "Add Reference Dataset"
    	And I click the "OSM_API_DB_osmapi_test" Dataset
    	And I press "Add Layer"
    	Then I wait 30 "seconds" to see "OSM_API_DB_osmapi_test"
    	And I press "Add Secondary Dataset"
    	And I click the "DcTigerRoadsCucumber" Dataset
    	And I press "Add Layer"
    	Then I wait 30 "seconds" to see "DcTigerRoadsCucumber"
    	Then I should see "Conflate"
    	And I press "Conflate"
    	And I scroll element into view and press "conflate2"
    	Then I wait 30 "seconds" to see "Conflating …"
    	Then I wait 3 "minutes" to see "osmapi_test_DcTigerRoadsCucumber"
    	Then I should see "Complete Review"
    	Then I wait 30 "seconds" to see "Reviews remaining:"
    	Then I should see "Complete Review"
    	And I click the "Complete Review" link
    	Then I should see "Resolve all remaining reviews"
    	And I press "Resolve all remaining reviews"
    	Then I wait 30 "seconds" to not see "Reviews remaining:"
    	Then I should see "All Reviews Resolved!"  
        # See corresponding explanation in the first scenario for why the Datasets tab is switched to here.
    	Then I select the "sprocket" div
        When I click the "osmapi_test_DcTigerRoadsCucumber" Dataset
        And I context click the "osmapi_test_DcTigerRoadsCucumber" Dataset
        And I click the "Export" context menu item
        Then I should see element "[id='fileExportOutputName']" with value "osmapi_test_DcTigerRoadsCucumber"
        When I select the "OSM API Database" option in the "File Geodatabase" combobox
        And I press "Export" big loud span
        # TODO: this doesn't work
        # Then I wait 30 "seconds" to see "The changeset will not be written because conflicts exist in the target database"

    

