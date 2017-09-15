Feature: Review Conflated Layer with Active Reviews After Deleting Inputs

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I click Get Started
        Then I select the "sprocket" div

    Scenario: Import Input Datasets
        When I click on the "Datasets" option in the "settingsSidebar"
        And I context click "Import Single Dataset"
        And I click the first "dataset-option" list item
        And I wait 5 "seconds" to see "Import Multiple Datasets"
        Then I press "big.loud" span with text "Add Row"
        Then I select in row 0 the "File (osm,osm.zip,pbf)" option in the "Select Import Type" combobox
        And I select in row 0 the "/test-files/conflate/unified/AllDataTypesA.osm" dataset
        And I should see row 0 input "Save As" with value "AllDataTypesA"
        Then I fill row 0 input "Save As" with value "1AllDataTypes_Cucumber"
        Then I select in row 1 the "File (osm,osm.zip,pbf)" option in the "Select Import Type" combobox
        And I select in row 1 the "/test-files/conflate/unified/AllDataTypesB.osm" dataset
        And I should see row 1 input "Save As" with value "AllDataTypesB"
        Then I fill row 1 input "Save As" with value "2AllDataTypes_Cucumber"
        Then I press "big.loud" span with text "Import"
        And I should see "Initializing..."
        And I wait 2 "minutes" to see "Close"
        Then I press "big.loud" span with text "Close"
        Then I wait 2 "minutes" to not see "Import Multiple Datasets"
        Then I wait 2 "minutes" to see "1AllDataTypes_Cucumber"
        And I wait 2 "minutes" to see "2AllDataTypes_Cucumber"
        And I select the "sprocket" div

    Scenario: Conflate The Two Datasets
        When I press "Add Reference Dataset"
        And I click the "1AllDataTypes_Cucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "span.strong" element with text "1AllDataTypes_Cucumber"
        And I press "Add Secondary Dataset"
        And I click the "2AllDataTypes_Cucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "span.strong" element with text "2AllDataTypes_Cucumber"
        Then I should see "Conflate"
        And I press "Conflate"
        And I fill "saveAs" input with "3MergeDataTypes_Cucumber"
        And I scroll element into view and press "conflate2"
        Then I wait 30 "seconds" to see "Conflating …"
        Then I wait 3 "minutes" to see "3MergeDataTypes_Cucumber"
        And I wait 30 "seconds" to not see "Please wait while panning to review item."
        And I click the "trash" button
        And I accept the alert

    Scenario: Delete Input Datasets
        When I select the "sprocket" div
        Then I click the "1AllDataTypes_Cucumber" Dataset and the "2AllDataTypes_Cucumber" Dataset
        And I context click the "2AllDataTypes_Cucumber" Dataset
        And I click the "Delete" context menu item
        And I wait
        And I accept the alert
        Then I should not see "1AllDataTypes_Cucumber" dataset after 30 "seconds"
        Then I should not see "2AllDataTypes_Cucumber" dataset after 30 "seconds"

    Scenario: Add conflated dataset with Reviews
        When I select the "sprocket" div
        And I press "Add Reference Dataset"
        And I click the "3MergeDataTypes_Cucumber" Dataset
        And I press "Add Layer"
        And I wait
        And I accept the alert
        Then I wait 30 "seconds" to see "span.strong" element with text "3MergeDataTypes_Cucumber"
        Then I should see "Complete Review"
        And I click the "Complete Review" link
        And I should see "There are 8 reviews"
        And I click the "Resolve all remaining reviews" link
        And I wait 5 seconds
        And I click the "trash" button
        And I wait 5 seconds
        And I select the "sprocket" div
        And I click the "3MergeDataTypes_Cucumber" Dataset
        And I context click the "3MergeDataTypes_Cucumber" Dataset
        And I click the "Delete" context menu item
        And I accept the alert
        Then I should not see "3MergeDataTypes_Cucumber" dataset after 30 "seconds"