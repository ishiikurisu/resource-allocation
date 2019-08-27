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


def s p
    validate($m, p) ? score(p) : -1.0/0.0
end


def f p, i
    if i >= $fr_r.length
        return p, s(p)
    end

    p1, s1 = f p + [$fr_r[i]], i+1
    p2, s2 = f p, i+1
    if s1 > s2
        return p1, s1
    else
        return p2, s2
    end
end


if __FILE__ == $0
    input_folder = ARGV[0]
    $fr_r = load "#{input_folder}/fr_R.json"
    $m = load "#{input_folder}/M.json"

    best_p, _ = f [], 0

    puts "best P:"
    puts best_p
end
