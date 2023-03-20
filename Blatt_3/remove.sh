#!/usr/bin/bash

echo "Hallo $USERNAME";

# Zaehler der bei mehreren loeschvorgaengen inkrementiert wird
 ZAEHLER=1;

# Erzeuge variablen zum erstellen der Ordner
 DIR=/home/$USERNAME/.TrashBin/.dir
 MOV=/home/$USERNAME/.TrashBin




# Wenn Ordner exisitert dann gut, ansonsten erstell einen neuen
 makeDirIfNotExist(){
    if ! [[ -a $DIR ]]
    then
        echo "$DIR wird erstellt"
        touch $DIR
    fi
}

# Wenn .dir existiert dann gut, ansonsten erstell einen neuen
makeIfTrashNotExist(){
	if ! [[ -d $MOV ]]
    then
		echo "$MOV wird erstellt"
		mkdir $MOV
	fi
}



# Funktion loeschen einer Datei
function remove(){
    
    # Uebergabeparameter wird lokal gespeichert
    KOPIE=$1;

    # Name der Datei wird zwischengespeichert und ein alias versehen
    NAME=`date +%y%m%d%H%M%S_$$_$ZAEHLER.dat`

    # Testen der Eingabe, klappt :)
    echo "$FILE wurde geloescht"

    # Aktuelle Datei die reinkommt in alia-schreibweise umbenennen
    mv $FILE $NAME
    
    # Datei in den Trashbin verschieben
    mv $NAME /home/$USERNAME/.TrashBin
    
    # Zaehler inkrementiert
    ((ZAEHLER++));
    
    #Datei in .dir schreiben ---------
    echo "$FILE $NAME $FULLPATH" >> ~/.TrashBin/.dir

}


# Funktionsaufruf nach jedem Start ob File/Ordner existiert
makeIfTrashNotExist
makeDirIfNotExist

# "Main Methode wo alles aufgerufen wird"
for i in $@;
do
    
    FILE=$i; # file=$1 vorher
    FULLPATH=`realpath $FILE`

    if [[ -a $FILE ]]
        then
        remove
    else
        echo "eingegebene Datei existiert nicht!!!"
    fi
done

