@javascript
Feature: Hootenanny Clip Dataset Test
    As a software tester
    I want to test the clip tool
    So I can ensure that it is working properly

    Scenario: Clip Feature
        Given I am on Hootenanny
        And I click Get Started
        When I click the "Add Reference Dataset" link
        And I click the "DcGisRoadsCucumber" Dataset
        Then I click first "Add Layer"
        Then I wait 30 "seconds" to see "DcGisRoadsCucumber"
        Then I press "Tools"
        And I hover over ".tools-clip"
        And I should see "Clip to Visual Extent"
        And I should see "Clip to Bounding Box"
        Then I click the "map" at "0","0"
        Then I click the "8" key
        And I click the "map" at "100","100"
        And I click the "map" at "-100","-100"
        Then I wait 5 "seconds" to see "Clip Data to Bounding Box"
        And I fill "LayerName" input with "CucumberClip_BB"
        And I press "Clip" big loud span
        Then I wait 15 "seconds" to see "has been submitted."
        Then I wait 15 "seconds" to see "Success:"
        Then I remove the first layer
        Then I close the UI alert
        When I click the "Add Reference Dataset" link
        And I click the "DcGisRoadsCucumber" Dataset
        Then I click first "Add Layer"
        Then I wait 30 "seconds" to see "DcGisRoadsCucumber"
        Then I click the "9" key
        Then I wait 5 "seconds" to see "Clip Data to Visual Extent"
        And I fill "LayerName" input with "CucumberClip_VE"
        Then I press "Clip" big loud span
        Then I wait 15 "seconds" to see "has been submitted."
        Then I wait 15 "seconds" to see "Success:"
