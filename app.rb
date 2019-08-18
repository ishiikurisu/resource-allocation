require "json"


def load f
  JSON.parse File.read f
end


def combine xs
    1.upto(xs.size).flat_map do |n|
        xs.combination(n).to_a
    end
end


def price p
    p.map { |e| e["p"] }.inject(0, :+)
end


def score p
    p.map { |e| e["s"] }.inject(0, :+)
end


def validate m, p
    is_q = m["Q"].map { |k, v| v == p.filter { |e| e["c"] == k }.length }.all?
    is_t = p.length == m["T"]
    is_w = price(p) < m["W"]
    is_q && is_t && is_w
end


def analyze fr_r, m
    combine(fr_r).filter { |p|
        validate(m, p)
    }.sort { |a, b|
        score(b) <=> score(a)
    }.first
end


if __FILE__ == $0
    input_folder = ARGV[0]
    fr_r = load "#{input_folder}/fr_R.json"
    m = load "#{input_folder}/M.json"

    best_p = analyze fr_r, m

    puts "best P:"
    puts best_p
end
