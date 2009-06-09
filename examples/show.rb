#!/usr/bin/env ruby
$:.unshift File.join(File.dirname(__FILE__), '..', 'lib')
require "statistics2"
if $0 == __FILE__
  if ARGV.empty?
    puts "Example:"
    puts "        #$0 normaldist 0.01"
    puts "        #$0 pf_x 2 3 0.01"
    exit
  end
  p Statistics2.send(ARGV[0], *ARGV[1..-1].map{|x| eval(x)})
end
