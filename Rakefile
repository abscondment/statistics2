# -*- ruby -*-

require 'rubygems'
gem 'hoe', '>= 2.1.0'
gem 'rake-compiler'
require 'hoe'
require "rake/extensiontask"

EXT = 'ext/statistics2/statistics2.so'

windows = RUBY_PLATFORM =~ /(mswin|mingw)/i
java    = RUBY_PLATFORM =~ /java/

Hoe.spec 'statistics2' do
  developer 'Shin-ichiro Hara', 'sinara@blade.nagaokaut.ac.jp'
  developer 'Brendan Ribera', 'brendan.ribera@gmail.com'
  
  self.readme_file  = 'README.rdoc'
  self.history_file = 'History.rdoc'
  self.extra_dev_deps << ['rake-compiler', '>= 0']
  self.url          = 'http://github.com/abscondment/statistics2'
  self.summary      = 'Statistical Distributions for Ruby. Based on Shin-ichiro Hara\'s original library, updated for Ruby 1.9'
  self.description  = 'Statistics2 is a module that provides normal, Chi-square, t- and F- probability distributions for Ruby. It is a fork/continuation of Shin-ichiro Hara\'s original code. It provides a native, compiled extension and a pure Ruby implementation.'

  # extension goodness
  self.clean_globs = ['ext/statistics2/Makefile',
                      'ext/statistics2/*.{o,so}',
                      'lib/statistics2/*.{o,so}',
                      'lib/statistics2/statistics2.rb']
  if java
    self.spec_extras = { :platform => 'java '}
    self.need_tar = false
    self.need_zip = false
  elsif !windows
    self.spec_extras[:extensions] = "ext/statistics2/extconf.rb"

    Rake::ExtensionTask.new('statistics2', spec) do |ext|
      ext.lib_dir = File.join('lib', 'statistics2')
    end
  end
end

Hoe.add_include_dirs '.'

if java || windows
  # TODO: compile others
  sh 'echo > lib/statistics2/statistics2.rb'
else
  sh 'rm -f lib/statistics2/statistics2.rb'
  Rake::Task[:test].prerequisites << :compile
end

task :default => [:test]
