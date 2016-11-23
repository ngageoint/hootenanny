Feature: Review Conflated Layer with Active Reviews After Deleting Inputs

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I click Get Started
        Then I select the "sprocket" div

    Scenario: Import Input Datasets
        When I click on the "Datasets" option in the "settingsSidebar"
        And I context click "Import Single Dataset"
        And I click the first "dataset-option" list item
        And I wait 5 "seconds" to see "Bulk Import Datasets"
        Then I press "big.loud" span with text "Add Row"
        Then I select in row 0 the "File (osm,osm.zip,pbf)" option in the "Select Import Type" combobox
        And I select in row 0 the "/test-files/UndividedHighway.osm" dataset
        And I should see row 0 input "Save As" with value "UndividedHighway"
        Then I fill row 0 input "Save As" with value "1UndividedHwy_Cuc"
        Then I select in row 1 the "File (osm,osm.zip,pbf)" option in the "Select Import Type" combobox
        And I select in row 1 the "/test-files/UndividedHighwayPreSplit.osm" dataset
        And I should see row 1 input "Save As" with value "UndividedHighwayPreSplit"
        Then I fill row 1 input "Save As" with value "2UndividedHwy_Cuc"
        Then I press "big.loud" span with text "Import"
        And I should see "Initializing..."
        And I wait 2 "minutes" to see "Close"
        Then I press "big.loud" span with text "Close"
        Then I wait 2 "minutes" to not see "Bulk Import Datasets"
        Then I wait 2 "minutes" to see "1UndividedHwy_Cuc"
        And I wait 2 "minutes" to see "2UndividedHwy_Cuc"
        And I select the "sprocket" div

    Scenario: Conflate The Two Datasets
        When I press "Add Reference Dataset"
        And I click the "1UndividedHwy_Cuc" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "1UndividedHwy_Cuc"
        And I press "Add Secondary Dataset"
        And I click the "2UndividedHwy_Cuc" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "2UndividedHwy_Cuc"
        Then I should see "Conflate"
        And I press "Conflate"
        And I fill "saveAs" input with "3MergeHwy_Cuc"
        And I scroll element into view and press "conflate2"
        Then I wait 30 "seconds" to see "Conflating …"
        Then I wait 3 "minutes" to see "3MergeHwy_Cuc"
        And I wait 30 "seconds" to not see "Please wait while panning to review item."
        And I click the "trash" button
        And I accept the alert

    Scenario: Delete Input Datasets
        When I select the "sprocket" div
        Then I click the "1UndividedHwy_Cuc" Dataset and the "2UndividedHwy_Cuc" Dataset
        And I context click the "2UndividedHwy_Cuc" Dataset
        And I click the "Delete" context menu item
        And I wait
        And I accept the alert
        Then I should not see "1UndividedHwy_Cuc" dataset after 30 "seconds"
        Then I should not see "2UndividedHwy_Cuc" dataset after 30 "seconds"

    Scenario: Add conflated dataset with Reviews
        When I select the "sprocket" div
        And I press "Add Reference Dataset"
        And I click the "3MergeHwy_Cuc" Dataset
        And I press "Add Layer"
        And I accept the alert
        Then I wait 15 "seconds" to see "span.strong" element with text "3MergeHwy_Cuc"
        Then I should see "Complete Review"
        And I click the "Complete Review" link
        And I should see "There are 1 reviews"
        And I click the "Resolve all remaining reviews" link
        And I wait 5 seconds
        And I click the "trash" button
        And I wait 5 seconds
        And I select the "sprocket" div
        And I click the "3MergeHwy_Cuc" Dataset
        And I context click the "3MergeHwy_Cuc" Dataset
        And I click the "Delete" context menu item
        And I accept the alert
        Then I should not see "3MergeHwy_Cuc" dataset after 30 "seconds"