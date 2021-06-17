Feature: Advanced Conflation Options

    Scenario: I can add data
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

    Scenario: I can set up standard conflation parameters
        Then I should see "Conflate"
        And I press "Conflate"
        And I fill "saveAs" input with "Merged_AllDataTypes_Advanced"

    Scenario: I set up the Advanced Conflation Options menu
        When I select the "Reference" option in "#containerofConfType"
        Then I click the "►" link
        And I should see "Advanced Conflation Options"

    Scenario Outline: Check for Invalid Inputs in Reference Advanced Options
        Then I should see "<option name>"
        Then I click on "#<option id>"
        And I should see element "#<element>" with no value and placeholder <placeholder>
        When I fill "#<element>" with "abc"
        When I press enter in the "#<element>" input
        Then I should see an invalid input warning for "#<element>"
        When I fill "#<element>" with ""
        When I press tab in the "#<element>" input
        And I should see element "#<element>" with no value and placeholder <placeholder>
        When I fill "#<element>" with "-100"
        When I press enter in the "#<element>" input
        Then I should see an invalid input warning for "#<element>"
        When I fill "#<element>" with ""
        When I press tab in the "#<element>" input
        And I should see element "#<element>" with no value and placeholder <placeholder>
        When I fill "#<element>" with "!@("
        When I press enter in the "#<element>" input
        Then I should see an invalid input warning for "#<element>"
        When I fill "#<element>" with ""
        When I press tab in the "#<element>" input
        And I should see element "#<element>" with no value and placeholder <placeholder>
        Then I click on "#<option id>"

        Examples:
            | option name | option id | element | placeholder |
            | Cleaning Options | hoot_cleaning_options_label | small_way_merger_threshold | 15 |
            | General Conflation Options | hoot_general_conflation_options_label | unify_optimizer_time_limit | 60 |
            | General Conflation Options | hoot_general_conflation_options_label | element_cache_size_node | 10000000 |
            | General Conflation Options | hoot_general_conflation_options_label | element_cache_size_relation | 2000000 |
            | General Conflation Options | hoot_general_conflation_options_label | element_cache_size_way | 2000000 |
            | River Options | hoot_river_options_label | river_way_angle_sample_distance | "20.0" |
            #TODO: fix me
            #| Waterway Options | hoot_waterway_options_label | waterway_way_matcher_heading_delta | "150.0" |
            | River Options | hoot_river_options_label | river_rubber_sheet_minimum_ties | 5 |

        Scenario: Open options that require checkbox for input
            Then I should see "Rubber Sheeting Options"
            Then I click on "#hoot_rubber_sheeting_options_label"
            Then I should see checkbox "Enabled" checked
            Then I should see checkbox "Don't Move Reference Layer" checked
            Then I should see "River Options"
            Then I click on "#hoot_river_options_label"
            Then I uncheck the "River Auto Calculate Search Radius" checkbox

            Scenario Outline: Check for Invalid Inputs that require checkboxes
                And I should see element "#<element>" with no value and placeholder <placeholder>
                When I fill "#<element>" with <invalid argument>
                When I press enter in the "#<element>" input
                Then I should see an invalid input warning for "#<element>"
                When I fill "#<element>" with ""
                When I press tab in the "#<element>" input

                Examples:
                    | element | placeholder | invalid argument |
                    | rubber_sheet_minimum_ties | 4 | "abcdef" |
                    | rubber_sheet_minimum_ties | 4 | "-10000" |
                    | rubber_sheet_minimum_ties | 4 | "&*^@(!" |

    #TODO: this test needs to be re-enabled; it was already mostly disabled, so I disabled the rest of it completely
    #Scenario: Test Cookie Cutter Options that are different than Reference or Average Options
        #Then I press "big.loud-red" span with text "Cancel"
        #Then I click the "►" link
        #And I should see "Advanced Conflation Options"
        #And I should see "Road Options"
        #And I click on "#hoot_road_options_label"
        #And I should see checkbox "Enabled" enabled
        #And I should see element "#hoot_road_opt_engine" with no value and placeholder "Network"

        # Scenario Outline:
        #     When I select the "<conflation option>" in the "div.contain.combobox-caret"
        #     And I should see element "#hoot_road_opt_engine" with value "<conflation option>"
        #     And I should see element "#<element>" with no value and placeholder <placeholder>
        #     When I fill "#<element>" with "abc"
        #     When I press enter in the "#<element>" input
        #     Then I should see an invalid input warning for "#<element>"
        #     When I fill "#<element>" with ""
        #     When I press tab in the "#<element>" input
        #     And I should see element "#<element>" with no value and placeholder <placeholder>
        #     When I fill "#<element>" with "-10000000"
        #     When I press enter in the "#<element>" input
        #     Then I <might> see an invalid input warning for "#<element>"
        #     When I fill "#<element>" with ""
        #     When I press tab in the "#<element>" input
        #     And I should see element "#<element>" with no value and placeholder <placeholder>
        #     When I fill "#<element>" with "!@("
        #     When I press enter in the "#<element>" input
        #     Then I should see an invalid input warning for "#<element>"
        #     When I fill "#<element>" with ""
        #     When I press tab in the "#<element>" input
        #     And I should see element "#<element>" with no value and placeholder <placeholder>
        #     Then I click on "#<option id>"


        #     Examples:
        #         | conflation option | option id | element | placeholder | might |
        #         | Unify | hoot_road_options_label | search_radius_highway | "-1" | should |
        #         | Unify | hoot_road_options_label | highway_matcher_heading_delta | "5.0" | should|
        #         | Unify | hoot_road_options_label | highway_matcher_max_angle | 60 | should |
        #         | Unify | hoot_road_options_label | way_merger_min_split_size | 5 | should |

        #need to also add examples for Differential

        # now test cookie cutter options
        # this can probable be reduced to another scenario outline
        # And I click on "#hoot_road_options_label"
        # And I should see "Cookie Cutter & Horizontal Options"
        # And I click on "#horizontal_conflation_options_group"
        # And I should see "Cookie Cutter Alpha Shape Buffer"
        # And I should see element "#horizontal_cookie_cutter_alpha_shape_buffer" with no value and placeholder "0"
        # When I fill "#horizontal_cookie_cutter_alpha_shape_buffer" with "abc"
        # When I press enter in the "#horizontal_cookie_cutter_alpha_shape_buffer" input
        # Then I should see an invalid input warning for "#horizontal_cookie_cutter_alpha_shape_buffer"
        # When I fill "#horizontal_cookie_cutter_alpha_shape_buffer" with ""
        # When I press tab in the "#horizontal_cookie_cutter_alpha_shape_buffer" input
        # And I should see element "#horizontal_cookie_cutter_alpha_shape_buffer" with no value and placeholder "0"
        # When I fill "#horizontal_cookie_cutter_alpha_shape_buffer" with "-10000000"
        # When I press enter in the "#horizontal_cookie_cutter_alpha_shape_buffer" input
        # Then I "should_not" see an invalid input warning for "#horizontal_cookie_cutter_alpha_shape_buffer"
        # When I fill "#horizontal_cookie_cutter_alpha_shape_buffer" with ""
        # When I press tab in the "#horizontal_cookie_cutter_alpha_shape_buffer" input
        # And I should see element "#horizontal_cookie_cutter_alpha_shape_buffer" with no value and placeholder "0"
        # When I fill "#horizontal_cookie_cutter_alpha_shape_buffer" with "!@("
        # When I press enter in the "#horizontal_cookie_cutter_alpha_shape_buffer" input
        # Then I should see an invalid input warning for "#horizontal_cookie_cutter_alpha_shape_buffer"
        # When I fill "#horizontal_cookie_cutter_alpha_shape_buffer" with ""
        # When I press tab in the "#horizontal_cookie_cutter_alpha_shape_buffer" input
        # And I should see element "#horizontal_cookie_cutter_alpha_shape_buffer" with no value and placeholder "0"
