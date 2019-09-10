require "json"


def new_player category
    return {
        "c" => category,
        "s" => Random.rand(30) - 15,
        "p" => Random.rand(28) + 2,
        "n" => Random.rand
    }
end


def generate_data
    outlet = []

    20.times do
        {
            "G" => 2,
            "Z" => 6,
            "M" => 6,
            "A" => 6,
            "T" => 1,
        }.each do |category, how_many|
            how_many.times do
                outlet << new_player(category)
            end
        end
    end

    return outlet
end


File.open("#{ARGV[0]}/fr_R.json", "w") do |fp|
    fp.write generate_data.to_json
end
