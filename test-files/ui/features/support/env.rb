require 'rubygems'
require 'capybara'
require 'capybara/dsl'
require 'rspec'
require 'capybara/webkit'
require 'capybara-screenshot/cucumber'
require 'selenium-webdriver'

# Chrome instance with timeout is set to 100 seconds
Capybara.register_driver :selenium do |app|
  client = Selenium::WebDriver::Remote::Http::Default.new
  client.timeout = 150
  Capybara::Selenium::Driver.new(app, :browser => :chrome, :http_client => client)
end

Capybara.javascript_driver = :selenium
Capybara.default_driver = :selenium

Capybara.run_server = false
Capybara.default_selector = :css
Capybara.default_max_wait_time = 10

module Helpers
  def without_resynchronize
    page.driver.options[:resynchronize] = false
    yield
    page.driver.options[:resynchronize] = true
  end
  def include_hidden_fields
    Capybara.ignore_hidden_elements = false
    yield
    Capybara.ignore_hidden_elements = true
  end
end

World(Capybara::DSL, Helpers)

# Keep only the screenshots generated from the last failing test suite
Capybara::Screenshot.prune_strategy = :keep_last_run
