# vim: ft=ruby
#

#If not using rack-jekyll
#require 'blog'
#run Sinatra::Application

#if using rack-jekyll
require 'yaml'
require 'rack/jekyll'
run Rack::Jekyll.new
