#!/usr/bin/env ruby
$test ||= false
if $test
  def nop(*a); ""; end
  alias _sprintf nop
  alias _printf nop
  alias _puts nop
else
  alias _sprintf sprintf
  alias _printf printf
  alias _puts puts
end

def norm_line(x, n)
  s = sprintf("%1.1f|", x)
  (0.00).step(0.09, 0.01) do |y|
    s << _sprintf(" %1.#{n}f", yield(x + y))
  end
  s
end

def norm_tbl(s, e, ln = nil, tn = nil, &b)
  n = 4
  unless ln
    _printf("    " + (" "*(n-1) + "%1.2f")*10 + "\n", * (0..9).map {|x| x*0.01})
  end
  i = 0
  (s.to_f).step(e.to_f, 0.1) do |x|
    next if ln && ln.to_i != i+1
    _puts norm_line(x, n, &b)
    i += 1
  end
end

def chi2_tbl(ln = nil, tn = nil)
  pers = [0.995, 0.99, 0.975, 0.95, 0.05, 0.025, 0.01, 0.005]
  arbi = (1..30).to_a + [40, 60, 80, 100]
  form = "  %7.5f"
  unless ln
    _printf("     "); pers.each do |a|; _printf(form, a); end;  _puts
  end
  arbi.each_with_index do |n, i|
    next if ln && ln.to_i != i+1
    _printf("%4d|", n) unless tn
    pers.each_with_index do |a, j|
      next if tn && tn.to_i != j+1
      form = case n
             when 1; a >= 0.95 ? " %.4e" : " %6.3f"
             when 2..5; a >= 0.95 ? "  %7.5f" : "  %7.3f"
             when 6..24; "  %7.3f"
             else; "  %7.2f" # 26..100
             end
      _printf(form, yield(n, a))
    end
    _puts
  end
end

def t_tbl(ln = nil, tn = nil)
  pers = [0.5, 0.4, 0.3, 0.2, 0.1, 0.05, 0.02, 0.01, 0.001]
  arbi = (1..30).to_a + [40, 60, 120]#, 9999]
  form = " %7.3f"
  unless ln
    _printf("     "); pers.each do |a|; _printf(form, a); end;  _puts
  end
  arbi.each_with_index do |n, i|
    next if ln && ln.to_i != i+1
    _printf("%4d|", n) unless tn
    pers.each_with_index do |a, j|
      next if tn && tn.to_i != j+1
      _printf(form, yield(n, a))
    end
    _puts
  end
end

def f_tbl(a, k1 = nil, k2 = nil)
  arbi1 = (1..10).to_a + [12, 15, 20, 24, 30, 40, 60, 120]#, 9999]
  arbi2 = (1..30).to_a + [40, 60, 120]#, 9999]
  unless k1
    _printf("     "); arbi1.each do |n1|; _printf(" %4d", n1); end;  _puts
  end
  arbi2.each do |n2|
    form = n2 == 1 ? " %4d" : n2 == 2 ? " %4.1f" : " %4.2f"
    next if k2 && k2.to_i != n2
    _printf("%4d|", n2) unless k2
    arbi1.each do |n1|
      next if k1 && k1.to_i != n1
      _printf(form, yield(n1, n2, a.to_f))
    end
    _puts
  end
end

def show_tbl(mod, dist, *opts)
  case dist
  when nil, "norm"
    norm_tbl(0.0, 3.1, *opts) do |x|
      mod.normal___x(x)
    end
  when "chi", "chi2"
    chi2_tbl(*opts) do |n, x|
      mod.pchi2_x(n, x)
    end
  when "t"
    t_tbl(*opts) do |n, x|
      mod.ptx__x(n, x)
    end
  when "f"
    f_tbl(*opts) do |n1, n2, a|
      mod.pf_x(n1, n2, a)
    end
  end
end


if $0 == __FILE__
  if ARGV.empty?
    puts "-- This script makes tables of distributions"
    puts "Example:"
    puts "      #$0 norm"
    puts "      #$0 chi2"
    puts "      #$0 t"
    puts "      #$0 f 0.01"
    exit
  end
  
  $:.unshift File.dirname(__FILE__)
  $:.unshift File.join(File.dirname(__FILE__), '..', 'lib')
  $:.unshift File.join(File.dirname(__FILE__), '..', 'ext')

  require 'statistics2'
  show_tbl(Statistics2, *ARGV)
end
