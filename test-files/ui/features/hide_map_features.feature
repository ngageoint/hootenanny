Feature: Toggle/hide map features of conflated layer in review mode

  Scenario: I can hide map features in review mode
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "ToyTestACucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "ToyTestACucumber"
    And I press "Add Secondary Dataset"
    And I click the "TTCauseReviewCucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "TTCauseReviewCucumber"
    Then I should see "Conflate"
    And I press "Conflate"
    And I fill "saveAs" input with "HideMapFeaturesCucumber"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 5 "minutes" to see "HideMapFeaturesCucumber"
    Then I wait 30 "seconds" to see "Complete Review"
    Then I wait 1 "minutes" to see "Exiting the review session"
    Then I context click "HideMapFeaturesCucumber"
    And I click the "div" with text "Zoom to Layer"
    Then I should see land use areas on the map
    And I should see service roads on the map
    And I should see paths on the map
    Then I wait 1 "minutes" to not see "Exiting the review session"
    Then I click to expand Map Data
    Then I click the "Map Features" link
    Then I press span with text "Landuse Features"
    Then I should not see land use areas on the map
    Then I hover over "#map"
    Then I press span with text "Service Roads"
    Then I should not see service roads on the map
    Then I press span with text "Paths"
    Then I should not see paths on the map
    Then I press span with text "Landuse Features"
    Then I should see land use areas on the map
    Then I hover over "#map"
    Then I press span with text "Service Roads"
    Then I should see service roads on the map
    Then I press span with text "Paths"
    Then I should see paths on the map
    Then I click the "Map Features" link
    Then I click the "trash" button
    And I accept the alert
    And I wait 5 "seconds" to not see "HideMapFeaturesCucumber"
