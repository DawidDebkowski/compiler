Wszelkie zażalenia zgłaszać do profesora Gębali, że nie wykrył, że ten
kompilator jest sklejony z taśmy.

Niektóre rozwiązania i części kodu są pozostałościami po nieudanych pomysłach,
których z braku chęci i czasu nie chciało mi się czyścić.

Makefile:
make test-all   # compiles

make test CATEGORY=category  # (ex. basic); doesn't compile

make 

Jeżeli cię interesuje porównanie logów to możesz wygenerować sobie 
dla swojego kompilatora mój log (jak np test-bs-all, albo test-bp-all)
i zrobić python compare_logs.py log1 log2

