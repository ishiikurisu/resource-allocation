require "json"

if __FILE__ == $0
    source_folder = ARGV[0]

    fr_r = JSON.parse File.read "#{source_folder}/fr_R.json"
    m = JSON.parse File.read "#{source_folder}/M.json"
    File.open("#{source_folder}/input.dat", 'w') do |fp|
        fp.write "Q #{m['Q'].length}\n"
        m['Q'].each do |k, v|
            fp.write "#{k} #{v}\n"
        end
        fp.write "T #{m['T']}\n"
        fp.write "W #{m['W']}\n"
        fp.write "F #{fr_r.length}\n"
        fr_r.each do |r|
            fp.write "C #{r['c']}\n"
            fp.write "S #{r['s']}\n"
            fp.write "P #{r['p']}\n"
        end
        fp.write "X 0"
    end
end
