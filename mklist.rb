#!/usr/bin/env ruby
cmd = "ruby sample-tbl.rb"
param_tbl = [["norm", "tbl-nomral.tbl"],
           ["chi2", "tbl-chi2.tbl"],
           ["t", "tbl-t.tbl"],
           ["f 0.05", "tbl-F50.tbl"],
           ["f 0.025", "tbl-F25.tbl"],
           ["f 0.01", "tbl-F10.tbl"]]

unless dir = ARGV.shift
  puts "Usage: $0 DIR"
  puts "-- This script creates some tables of distributions:"
  param_tbl.each do |mod, tbl|
    puts "    " + mod + " => " + tbl
  end
  exit
end

unless File.directory? dir
  system("mkdir #{dir}")
end

param_tbl.each do |pa, tbl|
  file = File.join(dir, tbl)
  system("#{cmd} #{pa} > #{file}")
end
