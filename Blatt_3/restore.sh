#!/usr/bin/bash

function restoreList(){

    #Zahelfolge als uebergabeparameter, Zahlenfolge in der txt datei suche, im ordner wiederherstellen
    # echo "Gebe Liste aus";
    # cat ~/.TrashBin/.dir;

    echo " Datei wird wiederhergestellt"
    restore
    
}



function restore(){
    
    
    QUELLE="`grep -w "$FILE" ~/.TrashBin/.dir`";
    set -- $QUELLE

    # Zugriff auf erste Stringkette
    NAME=$1;

    # Zugriff auf dritte Stringkette
    PFAD=$3
    PFADTRIMMED=${PFAD%/*}
    
    # Wenn Ordner existiert dann Vollgas sonst nichts machen
    if [[ -d $PFADTRIMMED ]]
        then
        echo " der Pfad existiert, man  kann weitermachen!!!"
        echo "$NAME ist der Input"
        echo "$PFAD der original Pfad"
        echo "$PFADTRIMMED ist der getrimmte  Pfad"

        # Schiebe zurueck in den Pfad wo er herkanm
        mv ~/.TrashBin/"$FILE" $NAME

        # Benenne die Datei die nun wo sie herkam wieder um wie sie urspruenglich hiesz
        mv $NAME $PFAD


        # neuen Pfad fuer lokale Datei names tmp erstellen
        tmp=/home/$USERNAME/.TrashBin/tmp
        # erstellen der Datei tmp im Pfad von oben
        touch=$tmp

        # Erzeugt eine temporärere Datei, die anschließend die Ausgangsdatei ersetzt.
        sed -i `$QUELLE/d` /home/$USERNAME/.TrashBin/.dir
    
        grep -v "$QUELLE" "/home/$USERNAME/.TrashBin/.dir" > "$tmp"
        mv "$tmp" "/home/$USERNAME/.TrashBin/.dir"

        # Wieso will der den Name ausfuehren? fragen!
        else
        echo "Pfad existiert nicht!!!, gib einen korrekten Pfad ein!!!"
    fi

}


#FILE=$1;

for i in $@;
do
    FILE=$i; # file=$1 vorher
    restoreList;
done

