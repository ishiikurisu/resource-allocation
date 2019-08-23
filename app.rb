require "json"


def load f
  JSON.parse File.read f
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
    best_p = nil
    best_score = -1.0/0.0

    fr_r.combination(m["T"]) do |p|
        if validate(m, p) && score(p) > best_score
            best_p = p
            best_score = score(p)
        end
    end

    return best_p
end


if __FILE__ == $0
    input_folder = ARGV[0]
    fr_r = load "#{input_folder}/fr_R.json"
    m = load "#{input_folder}/M.json"

    best_p = analyze fr_r, m

    puts "best P:"
    puts best_p
end
