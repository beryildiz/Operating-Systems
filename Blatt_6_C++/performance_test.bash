#
# Skript zum Testen des Bots:
# Startet den Bot auf Datei testSites_10_2019.txt, 
# Delay fuer Zugriff auf Seite wird auf 100 ms eingestellt, 
# gelesene Daten werden in Verzeichnis 'download' geschrieben. 
#
FILE="testSites_10_2020.txt"
D_PATH="download"

for i in {1..10}
do
    ./simple_bot $FILE --thread-count 1 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --thread-count 2 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --thread-count 5 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --thread-count 10 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --thread-count 15 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --thread-count 20 --webreq-path $D_PATH
    make clean_html
    sleep 10

    ./simple_bot $FILE --webreq-delay 300 --thread-count 1 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 300 --thread-count 2 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 300 --thread-count 5 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 300 --thread-count 10 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 300 --thread-count 15 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 300 --thread-count 20 --webreq-path $D_PATH
    make clean_html
    sleep 10

    ./simple_bot $FILE --webreq-delay 500 --thread-count 1 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 500 --thread-count 2 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 500 --thread-count 5 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 500 --thread-count 10 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 500 --thread-count 15 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 500 --thread-count 20 --webreq-path $D_PATH
    make clean_html
    sleep 10

    ./simple_bot $FILE --webreq-delay 1000 --thread-count 1 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 1000 --thread-count 2 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 1000 --thread-count 5 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 1000 --thread-count 10 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 1000 --thread-count 15 --webreq-path $D_PATH
    make clean_html
    sleep 10
    ./simple_bot $FILE --webreq-delay 1000 --thread-count 20 --webreq-path $D_PATH
    make clean_html
    sleep 10
done