Feature: Generate Conflation Report

    Scenario: Enable test mode
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started
        When I select the "sprocket" div
        When I click on the "About" option in the "settingsSidebar"
        And I should see checkbox "Enable Test Mode" unchecked
        And I check the "Enable Test Mode" checkbox
        And I select the "sprocket" div

    Scenario: Add and Conflate Data
        And I press "Add Reference Dataset"
        And I click the "AllDataTypesACucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesACucumber"
        And I press "Add Secondary Dataset"
        And I click the "AllDataTypesBCucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesBCucumber"
        Then I should see "Conflate"
        And I press "Conflate"
        And I fill "saveAs" input with "Conflate_Report"
        And I select the "true" option in "#containerofisGenerateReport"
        And I scroll element into view and press "conflate2"
        Then I wait 30 "seconds" to see "Conflating …"
        Then I wait 3 "minutes" to see "Conflate_Report"
        Then I should see "Complete Review"
        And I click the "Complete Review" link
        Then I should see "Resolve all remaining reviews"
        And I press "Resolve all remaining reviews"
        When I click on "input#export_data"
        And I click on "input#exit_conflate"

    Scenario: Check out the Report
        When I select the "sprocket" div
        And I click on the "Reports" option in the "settingsSidebar"
        And I should see "Conflate_Report"
        When I click the "up" icon under the "Conflate_Report" link
        Then I press "round" span with text "Download"
        And I wait 30 seconds
        Then the download file "Conflate_Report.pdf" should exist

    Scenario: Delete the Datasets
        When I click the "trash" icon under the "Conflate_Report" link
        And I wait
        And I accept the alert
        Then I click on the "Datasets" option in the "settingsSidebar"
        When I click the "Conflate_Report" Dataset
        And I context click the "Conflate_Report" Dataset
        And I click the "Delete" context menu item
        And I wait
        And I accept the alert
        And I wait 30 "seconds" to not see "Conflate_Report"
        