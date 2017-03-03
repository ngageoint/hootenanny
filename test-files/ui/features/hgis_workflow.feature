Feature: HGIS Imagery Validation Workflow

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started
        Then I select the "sprocket" div
        When I click on the "Datasets" option in the "settingsSidebar"

    Scenario: Filter non-HGIS POIs
        When I click the "BrazilMergedCucumber" Dataset
        And I context click the "BrazilMergedCucumber" Dataset
        And I click the "Filter non-HGIS POIs" context menu item
        Then I append "FilterName" input with "BrazilFilterCucumber"
        And I press "big.loud" span with text "Run"
        And I wait 1 "minutes" to not see "Output Name"

    Scenario: Prepare for Validation
        When I click the "BrazilFilterCucumber" Dataset
        And I context click the "BrazilFilterCucumber" Dataset
        And I click the "Prepare for Validation" context menu item
        Then I append "ValidationName" input with "BrazilValidateCucumber"
        And I press "big.loud" span with text "Run"
        And I wait 1 "minutes" to not see "Output Name"

    Scenario: Add dataset to map and validate features
        When I select the "sprocket" div
        And I press "Add Reference Dataset"
        And I click the "BrazilValidateCucumber" Dataset
        And I press "Add Layer"
        And I wait 30 seconds
        And I accept the alert
        Then I wait 30 "seconds" to see "Validation items remaining: 2"
        When I press "Assessed"
        And I wait 30 "seconds" to see "Validation items remaining: 1"
        Then I press "Reported"
        And I wait 30 "seconds" to see "Validation complete"
        And I should not see "Validation items remaining"

    Scenario: Remove the layer from the map
        Then I click the "trash" button
        And I accept the alert
