clear 

# traitement si l'entrée est un .adb
if [[ "$1" == *.adb ]]
then
    file=${1:0:`expr index "$1" '.*\(\.adb\)'`}
    type=ada
elif [[ "$1" == *.c ]]
then
    file=${1:0:`expr index "$1" '.*\(\.c\)'`}
    type=c
else
    file=$1
    if [[ -a "$file.c" ]]
    then
        type=c
    elif [[ -a "$file.adb" ]]
    then
        type=ada
    else
        exit 1 -p "type non reconnu par le compiler"
    fi
fi


if [[ "$type" == ada ]]
then
    echo "file : $file.abd"
elif [[ $type == c ]]
then
    echo "file : $file.c"
else 
    exit 1 -p "type non reconnu par le compiler"
fi


echo "========================================================"
echo "Début de la Compilation"
echo

if [[ "$type" == ada ]]
then
    gnat make $file.adb -gnata -gnatwa -gnatwu -g
    compile_success=$?
elif [[ $type == c ]]
then
    gcc $file.c -o $file -Wall -Wpedantic -fsanitize:address,undefined -Wextra
    compile_success=$?
else 
    exit 1 -p "type non reconnu par le compiler"
fi



echo

if [ $compile_success -a $file ]
then
    echo $compile_success 
    if [ ! -a $file ]
    then
        echo le compilateur ne fournit pas d\'éxécutable
    fi
    if [ ! $compile_success ]
    then 
        echo gnat ne compile pas
    fi
    echo "========================================================"
    echo "Début de nettoyage"
    echo

    gnat clean $file

    echo
    echo "Fin du nettoyage"
    echo "========================================================"
    echo "Fin du scripte."
    echo 
    exit 1
else
    echo "Fin de la compilation"
    echo "========================================================"
    echo "Debut de Valgrind"
    echo
fi

valgrind ./$file --leak-check=full | grep "no leaks are possible"


echo
echo "Fin de Valgrind"
echo "========================================================"
echo "Début de l'éxécution"
echo

./$file

echo
echo "Fin de l'éxécution"
echo "========================================================"
echo "Début de nettoyage"
echo

gnat clean $file

echo
echo "Fin du nettoyage"
echo "========================================================"
echo "Fin du scripte"
echo