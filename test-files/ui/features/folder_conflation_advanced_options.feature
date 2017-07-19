Feature: Folder and conflation options control

	Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I click Get Started
        Then I select the "sprocket" div

    Scenario: Invalid folder names will not be added
        When I click on the "Datasets" option in the "settingsSidebar"
        And I click the "import-add-folder" button
        And I fill "NewFolderName" input with "@#&%^"
        And I press "big.loud" span with text "Add Folder"
        Then I should see an invalid name warning for ".NewFolderName" input
        Then I close the modal
        Then I select the "sprocket" div

    Scenario: I can cancel selecting advanced conflation options
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started
        And I press "Add Reference Dataset"
        And I click the "AllDataTypesACucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "span.strong" element with text "AllDataTypesACucumber"
        And I press "Add Secondary Dataset"
        And I click the "AllDataTypesBCucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "span.strong" element with text "AllDataTypesBCucumber"
        Then I should see "Conflate"
        And I press "Conflate"
        And I fill "saveAs" input with "AllDataTypesAdvancedOptionsCucumber"
        And I press "confAdvOptsLnk"
        And I scroll "Cancel" element into view and press it
        And I scroll element into view and press "conflate2"
        Then I wait 30 "seconds" to see "Conflating …"
        Then I wait 3 "minutes" to see "AllDataTypesAdvancedOptionsCucumber"
        Then I click the "trash" button
        And I accept the alert
        And I wait 5 "seconds" to not see "AllDataTypesAdvancedOptionsCucumber"