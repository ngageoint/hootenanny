Feature: Toggle/hide map features of conflated layer in review mode

  Scenario: I can hide map features in review mode
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
    And I fill "saveAs" input with "HideMapFeaturesCucumber"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 3 "minutes" to see "HideMapFeaturesCucumber"
    Then I click to expand Map Data
    Then I click the "Map Features" link
    Then I should see "Traffic Roads"
    And I should see "Landuse Features"
    And I should see "Power Features"
    Then I click the "Map Features" link
    Then I should not see "Traffic Roads"
    And I should not see "Landuse Features"
    And I should not see "Power Features"
    Then I click the "trash" button
    And I accept the alert
    And I wait 5 "seconds" to not see "AllDataTypesMergedCucumber"