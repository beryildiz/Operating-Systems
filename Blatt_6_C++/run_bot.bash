#
# Skript zum Testen des Bots:
# Startet den Bot auf Datei testSites_10_2019.txt, 
# Delay fuer Zugriff auf Seite wird auf 100 ms eingestellt, 
# gelesene Daten werden in Verzeichnis 'download' geschrieben. 
#
./simple_bot testSites_10_2020.txt --webreq-delay 100 --thread-count 20 --webreq-path download
