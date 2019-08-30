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


def write_matrix m
    m.each do |row|
        puts row.map { |e| e == -1.0/0.0 ? 'nil' : e }.join("\t")
    end
end


def calculate x, y, memo, prop
    if (x-1 < 0) || (y-1 < 0)
        memo[y][x] = -1.0/0.0
    else
        adding_player = [memo[y-1][x-1], 0].max
        not_adding_player = [memo[y][x-1], 0].max
        memo[y][x] = [
            $fr_r[y][prop] + adding_player,
            not_adding_player,
        ].max
    end

    return memo
end


def analyze
    # populating memo
    limit = $fr_r.length
    for y in 0...$fr_r.length
        for x in 0 ... $fr_r.length
            calculate x, y, $score_memo, 's'
            calculate x, y, $money_memo, 'p'
        end
    end

    write_matrix $score_memo
    write_matrix $money_memo

    # TODO filter scores by money

    # TODO select possible scores for my money
end


def prepare_memo
    memo = []
    $fr_r.length.times do
        a = []
        $m["T"].times do
            a << nil
        end
        memo << a
    end
    return memo
end


if __FILE__ == $0
    input_folder = ARGV[0]
    $fr_r = load "#{input_folder}/fr_R.json"
    $m = load "#{input_folder}/M.json"
    $score_memo = prepare_memo
    $money_memo = prepare_memo

    best_p = analyze

    puts "best P:"
    puts best_p
end
