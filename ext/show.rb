#!/usr/bin/env ruby
require "statistics2.so"
if $0 == __FILE__
  if ARGV.empty?
    puts "Example:"
    puts "        #$0 normaldist 0.01"
    puts "        #$0 pf_x 2 3 0.01"
    exit
  end
  p Statistics2.send(ARGV[0], *ARGV[1..-1].map{|x| eval(x)})
end
