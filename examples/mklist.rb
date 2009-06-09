#!/usr/bin/env ruby
cmd = "ruby #{File.expand_path(File.join(File.dirname(__FILE__), '/../test/sample_tbl.rb'))}"
param_tbl = {
  "norm" => "tbl-normal.tbl",
  "chi2" => "tbl-chi2.tbl",
  "t" => "tbl-t.tbl",
  "f 0.05" => "tbl-F50.tbl",
  "f 0.025" => "tbl-F25.tbl",
  "f 0.01" => "tbl-F10.tbl"
}

printf 'Generating tables'
param_tbl.each do |pa, tbl|
  printf '.'
  STDOUT.flush()
  `#{cmd} #{pa} > #{File.expand_path(File.join(File.dirname(__FILE__), tbl))}`
end
puts "finished."