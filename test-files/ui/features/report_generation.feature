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
        And I should see "Generate Report?"
        And I select the "true" option in "#containerofisGenerateReport"
        And I press tab
        And I press tab
        And I press enter
        Then I wait 30 "seconds" to see "Conflating …"
        Then I wait 3 "minutes" to see "Conflate_Report"
        Then I should see "Complete Review"
        And I click the "Complete Review" link
        Then I should see "Resolve all remaining reviews"
        And I press "Resolve all remaining reviews"

    Scenario: Check out the Report
        When I select the "sprocket" div
        And I click on the "Reports" option in the "settingsSidebar"
        And I should see "Conflate_Report"
        And I click the "_icon.up" button
        Then I press "Download"
        And I wait 30 seconds
        Then the download file "Conflate_Report.pdf" should exist