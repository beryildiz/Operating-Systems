#!/bin/bash

#Uebergabeparemeter muss $FILE sein damit stat der Befehl stat korrekt arbeiten kann


# -a prueft ob der uebergebene Pfad existiert
 function checkFile(){
    if [ -a  $FILE ]
    then 
        echo "$FILE existiert"
    else
        echo "$FILE existiert nicht"
    fi
}

# -d / -f prueft ob der uebergebene Pfad ein/e Datei/Ordner ist
 function isFileOrFolder(){
    if [ -d  $FILE ]
    then
        echo " $FILE ist ein Ordner"
    elif [ -f  $FILE ]
    then
        echo "$FILE ist eine Datei"
    else
        echo "$FILE ist kein gueltiger Pfad"
    fi
}


# -h prueft ob pruft ob der uebergebene Pfad ein symbolischer Link ist
 function isLink(){
    if [ -h  $FILE ]
    then
        echo "$FILE ist ein symbolischer Link"
    else
        echo "$FILE ist kein symbolischer Link"
    fi
}


# -O prueft ob ich mit der UserID uebereinstimme
# mit stat -c greifen wir auf die Metadaten Owner zu fuer das ausgeben in der Konsole
function isOwner(){
    if [ -O  $FILE ] && [ -f  $FILE ]
    then
         echo "Besitzer von der Datei $FILE: $(stat -c '%U' $FILE)"
    elif [ -O  $FILE ] && [ -d  $FILE ]
    then
        echo "Besitzer des Ordners $FILE: $(stat -c '%U' $FILE)"
    else
        echo "$FILE gehoert mir nicht"
    fi
}


# $1 ist der Uebergebene Parameter in der Konsole. z.B source dummy.sh test_1.txt
# Ausfuehrung ohne Schleife
## FILE=$1


# cat gibt die Datei aus
# Doppelte Klammern schaffen einen eigenen LeseScope?

function iterateTextFiles(){
    if [[ $FILE == *.txt ]]
then
        echo "Text ausgeben (y/n)"
        read INPUT

    if [ $INPUT == y ]
        then
            cat $FILE
        fi
    fi
}


for i in $@;
do 
    FILE=$i

    echo "-------------------------------"
    checkFile 
    isFileOrFolder
    isLink  
    isOwner 
    iterateTextFiles
done



