require 'rubygems'
require 'capybara'
require 'capybara/dsl'
require 'rspec'
require 'capybara/webkit'
require 'capybara-screenshot/cucumber'
require 'selenium-webdriver'

# Chrome instance with timeout is set to 100 seconds
Capybara.register_driver :chrome_timeout do |app|
  client = Selenium::WebDriver::Remote::Http::Default.new
  client.read_timeout = 100
  Capybara::Selenium::Driver.new(app, :browser => :chrome, :http_client => client)
end

Capybara.register_driver :selenium do |app|
  Capybara::Selenium::Driver.new(app, :browser => :chrome)
end
Capybara.javascript_driver = :chrome_timeout
Capybara.default_driver = :chrome_timeout

Capybara.run_server = false
Capybara.default_selector = :css

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
