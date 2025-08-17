#!/bin/bash


S21_CAT="./s21_cat"


compare_output() {
    local flag=$1
    local file=$2


    cat $flag "$file" > original_output.txt
    $S21_CAT $flag "$file" > custom_output.txt 

    
    if diff -u original_output.txt custom_output.txt > /dev/null; then
        echo "Тест [Флаги: $flag, Файл: $file]: Успешно"
    else
        echo "Тест [Флаги: $flag, Файл: $file]: Ошибка"
        echo "Различия:"
        diff -u original_output.txt custom_output.txt
    fi


    rm original_output.txt custom_output.txt 
}

files=(
    "tests/test_1_cat.txt"     
    "tests/test_2_cat.txt"
    "tests/test_3_cat.txt"
    "tests/test_4_cat.txt"
    "tests/test_5_cat.txt"
    "tests/test_case_cat.txt"
)

flags=( # для linux "--number" "--number-nonblank" "--squeeze-blank"
    ""
    "-b"
    "-n"
    "-s"
    "-e"
    "-t"
    "-v"
)


for file in "${files[@]}"; do 
    for flag in "${flags[@]}"; do
        if [[ -f $file ]]; then
            compare_output "$flag" "$file"
        else
            echo "Файл не найден: $file"
        fi
    done
    echo "- - - - - - - - - - - - - - - - - - - - - - - - - - -"
done
