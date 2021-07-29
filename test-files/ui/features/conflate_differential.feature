Feature: Conflate differential

  Scenario: I can conflate data with Differential Conflation
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "DiffInput1Cucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "DiffInput1Cucumber"
    And I press "Add Secondary Dataset"
    And I click the "DiffInput2Cucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "DiffInput2Cucumber"
    Then I should see "Conflate"
    And I press "Conflate"
    And I fill "saveAs" input with "DiffConflateCucumber"
    And I select the exact "Differential" option in "#containerofConfType"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 10 "minutes" to see "DiffConflateCucumber"
    Then I click the "trash" button
    And I accept the alert
    And I wait 5 "seconds" to not see "DiffConflateCucumber"
