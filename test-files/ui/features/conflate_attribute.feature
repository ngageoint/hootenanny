Feature: Attribute conflation

  Scenario: I can conflate using the attribute conflation type
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "ToyTestBCucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "ToyTestBCucumber"
    And I press "Add Secondary Dataset"
    And I click the "ToyTestACucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "ToyTestACucumber"
    Then I should see "Conflate"
    And I press "Conflate"
    And I fill "saveAs" input with "TTRoadsAttributeConflationCucumber"
    And I select the "Attribute" option in "#containerofConfType"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 4 "minutes" to see "TTRoadsAttributeConflationCucumber"
    Then I click the "trash" button
    And I accept the alert
    And I wait 5 "seconds" to not see "TTRoadsAttributeConflationCucumber"
