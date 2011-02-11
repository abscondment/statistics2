$:.unshift File.dirname(__FILE__)
$:.unshift File.join(File.dirname(__FILE__), '..', 'ext/statistics2')
$:.unshift File.join(File.dirname(__FILE__), '..', 'lib')

$test = true

require 'test/unit' unless defined?(Hoe)
require 'sample_tbl'
require 'lib/statistics2'

class T_Statistics2 < Test::Unit::TestCase
  $mod = Statistics2
  
  def inv(s); "p" + s; end

  def test_inv_normal
    delta = 1.0e-6
    meth = "normal___x"
    norm_tbl(0.00001, 4.0) do |x|
      pr = $mod.send(meth, x)
      x0 = $mod.send(inv(meth), pr)
      assert_in_delta x0/x, 1.0, delta
    end
  end

  def test_inv_chi
    delta = 1.0e-4
    meth = "chi2_x"
    chi2_tbl() do |n, pr|
      x = $mod.send(inv(meth), n, pr)
      pr0 = $mod.send(meth, n, x)
      assert_in_delta pr0/pr, 1.0, delta
    end
  end

  def test_inv_t
    delta = 1.0e-3
    meth = "tx__x"
    t_tbl() do |n, pr|
      x = $mod.send(inv(meth), n, pr)
      pr0 = $mod.send(meth, n, x)
      assert_in_delta pr0/pr, 1.0, delta
    end
  end

  def test_inv_f
    delta = 1.0e-3
    meth = "f_x"
    [0.01, 0.025, 0.05].each do |opt|
      f_tbl(opt) do |n1, n2, pr|
        x = $mod.send(inv(meth), n1, n2, pr)
        pr0 = $mod.send(meth, n1, n2, x)
        assert_in_delta pr0/pr, 1.0, delta
      end
    end
  end
end
