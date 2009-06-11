$:.unshift File.join(File.dirname(__FILE__), '..')

# Indicate that we should skip loading of the C extension.
module Statistics2
  NO_EXT = true
end

require 'statistics2'