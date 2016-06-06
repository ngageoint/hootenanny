Feature: OSM API Database Conflation and Export

    Scenario: Conflate an OSM API Database Dataset with Hootenanny Dataset and Export the Conflated Result back into the OSM API Database
    	Given I am on Hootenanny
    	And I resize the window
    	And I click Get Started
    	And I press "Add Reference Dataset"
    	And I click the "OSM_API_Database_osmapi_..." Dataset
    	And I press "Add Layer"
    	Then I wait 30 "seconds" to see "OSM_API_Database_osmapi_test"
    	And I press "Add Secondary Dataset"
    	And I click the "AllDataTypesBCucumber" Dataset
    	And I press "Add Layer"
    	Then I wait 30 "seconds" to see "AllDataTypesBCucumber"
    	Then I should see "Conflate"
    	And I press "Conflate"
        Then I fill "saveAs" input with "OSM_API_Database_AllDataTypes_Cucumber"
    	And I scroll element into view and press "conflate2"
    	Then I wait 30 "seconds" to see "Conflating …"
    	Then I wait 3 "minutes" to see "OSM_API_Database_AllDataTypes_Cucumber"
    	Then I should see "Complete Review"
    	Then I wait 30 "seconds" to see "Reviews remaining:"
    	Then I should see "Hide Table"
    	Then I should see "Previous"
    	Then I should see "Next"
    	Then I should see "Resolved"
    	Then I should see "Complete Review"
    	And I click the "Complete Review" link
    	Then I should see "Resolve all remaining reviews"
    	And I press "Resolve all remaining reviews"
    	Then I wait 30 "seconds" to not see "Reviews remaining:"
    	Then I should not see "Hide Table"
    	Then I should not see "Previous"
    	Then I should not see "Next"
    	Then I should see "All Reviews Resolved!"  
    	#Then I should see "Export Data"
    	And I press "Export Data"
    	Then I should see element "[id='fileExportOutputName']" with value "OSM_API_Database_AllDataTypes_Cucumber"
    	When I select the "OSM API Database" option in the "File Geodatabase" combobox
        And I press "Export" big loud span
        And I wait 30 seconds
        Then I should see "Successful Export to an OSM API Database"

    Scenario: Conflate With OSM API Database Dataset as Secondary Layer
    	Given I am on Hootenanny
    	And I resize the window
    	And I click Get Started
    	And I press "Add Reference Dataset"
    	And I click the "DcTigerRoadsCucumber" Dataset
    	And I press "Add Layer"
    	Then I wait 30 "seconds" to see "DcTigerRoadsCucumber"
    	And I press "Add Secondary Dataset"
    	And I click the "OSM_API_Database_osmapi_..." Dataset
    	And I press "Add Layer"
    	Then I wait 30 "seconds" to see "OSM_API_Database_osmapi_test"
    	Then I should see "Conflate"
    	And I press "Conflate"
    	Then I wait 30 "seconds" to see "OSM API database not allowed as secondary layer input."

    Scenario: Export Dataset into OSM API Database With Conflicts
    	Given I am on Hootenanny
    	And I resize the window
    	And I click Get Started
    	And I press "Add Reference Dataset"
    	And I click the "OSM_API_Database_osmapi_..." Dataset
    	And I press "Add Layer"
    	Then I wait 30 "seconds" to see "OSM_API_Database_osmapi_test"
    	And I press "Add Secondary Dataset"
    	And I click the "DcTigerRoadsCucumber" Dataset
    	And I press "Add Layer"
    	Then I wait 30 "seconds" to see "DcTigerRoadsCucumber"
    	Then I should see "Conflate"
    	And I press "Conflate"
        Then I fill "saveAs" input with "OSM_API_Database_DcRoads_Cucumber"
    	And I scroll element into view and press "conflate2"
    	Then I wait 30 "seconds" to see "Conflating …"
    	Then I wait 3 "minutes" to see "OSM_API_Database_DcRoads_Cucumber"
    	Then I should see "Complete Review"
    	Then I wait 30 "seconds" to see "Reviews remaining:"
    	Then I should see "Hide Table"
    	Then I should see "Previous"
    	Then I should see "Next"
    	Then I should see "Resolved"
    	Then I should see "Complete Review"
    	And I click the "Complete Review" link
    	Then I should see "Resolve all remaining reviews"
    	And I press "Resolve all remaining reviews"
    	Then I wait 30 "seconds" to not see "Reviews remaining:"
    	Then I should not see "Hide Table"
    	Then I should not see "Previous"
    	Then I should not see "Next"
    	Then I should see "All Reviews Resolved!"  
    	#Then I should see "Export Data"
    	And I press "Export Data"
    	Then I should see element "[id='fileExportOutputName']" with value "OSM_API_Database_AllDataTypes_Cucumber"
    	When I select the "OSM API Database" option in the "File Geodatabase" combobox
        And I press "Export" big loud span
        And I wait 30 seconds
        Then I should see "The changeset will not be written because conflicts exist in the target database."

