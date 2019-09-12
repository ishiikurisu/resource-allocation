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


def generate_variations p, r
    variations = [p]

    unless p.map { |x| x['n'] == r['n'] }.any?
        indexes = []
        p.each_index do |i|
            if p[i]['c'] == r['c']
                indexes << i
            end
        end

        indexes.each do |i|
            new_p = Array.new p
            new_p[i] = r
            variations << new_p
        end
    end

    return variations
end


# improves a valid p using r
def improve p, r, m
    return generate_variations(p, r).filter { |e| validate m, e }.max { |a, b| score(a) <=> score(b) }
end


# generates a valid p from fr_r and m
def generate fr_r, m
    p = []

    m['Q'].each do |q_k, q_v|
        p += fr_r.select { |r| r['c'] == q_k }.sort { |a, b| a['p'] <=> b['p'] }.take q_v
    end

    unless validate(m, p)
        raise 'invalid p to begin with'
    end

    return p
end


# that's the main procedure!
def analyze fr_r, m
    p = generate fr_r, m
    old_p = []

    while score(old_p) < score(p)
        old_p = Array.new p
        fr_r.each do |r|
            p = improve p, r, m
        end
    end

    return p
end


if __FILE__ == $0
    input_folder = ARGV[0]
    fr_r = load "#{input_folder}/fr_R.json"
    mm = load "#{input_folder}/M.json"

    best_p = mm.map { |m| analyze fr_r, m }.max { |a, b| score(a) <=> score(b) }

    puts "best P:"
    puts best_p
    puts "=> $#{price(best_p)}"
    puts "=> #{score(best_p)}p"
end
