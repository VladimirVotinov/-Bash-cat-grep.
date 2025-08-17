#!/bin/bash

S21_GREP="./s21_grep"

compare_output() {
    local flags=$1
    local pattern=$2
    local file=$3

    if [ -f $file ]; then
        grep $flags "\"$pattern\"" "$file" > original_output.txt
        $S21_GREP $flags "\"$pattern\"" "$file" > custom_output.txt

        
        if diff -u original_output.txt custom_output.txt > /dev/null; then
            echo "Тест [Флаги: \"$flags\" Шаблон: \"$pattern\" Файл: \"$file\"]: Успешно"
        else
            echo "Тест [Флаги: \"$flags\" Шаблон: \"$pattern\" Файл: \"$file\"]: Ошибка"
            echo "Различия:"
            diff -u original_output.txt custom_output.txt
        fi

        rm original_output.txt custom_output.txt
    else
        echo "Файл не найден: $file"
    fi
}

compare_output_f() {
    local flags=$1
    local file_pattern=$2
    local file=$3

    if [ -f $file ]; then
        grep $flags "$file_pattern" "$file" > original_output.txt
        $S21_GREP $flags "$file_pattern" "$file" > custom_output.txt

        
        if diff -u original_output.txt custom_output.txt > /dev/null; then
            echo "Тест [Флаги: \"$flags\" Шаблон из файла: \"$file_pattern\" Файл: \"$file\"]: Успешно"
        else
            echo "Тест [Флаги: \"$flags\" Шаблон из файла: \"$file_pattern\" Файл: \"$file\"]: Ошибка"
            echo "Различия:"
            diff -u original_output.txt custom_output.txt
        fi

        rm original_output.txt custom_output.txt
    else
        echo "Файл не найден: $file"
    fi
}


compare_output_e() {
    local flags=$1
    local pattern=$2
    local file=$3
    if [ -f $file ]; then
        grep $flags "\"$file_pattern\"" "$file" > original_output.txt
        $S21_GREP $flags "\"$file_pattern\"" "$file" > custom_output.txt

        
        if diff -u original_output.txt custom_output.txt > /dev/null; then
            echo "Тест [Флаги: \"$flags\" Шаблон из файла: \"$file_pattern\" Файл: \"$file\"]: Успешно"
        else
            echo "Тест [Флаги: \"$flags\" Шаблон из файла: \"$file_pattern\" Файл: \"$file\"]: Ошибка"
            echo "Различия:"
            diff -u original_output.txt custom_output.txt
        fi

        rm original_output.txt custom_output.txt
    else
        echo "Файл не найден: $file"
    fi
}

compare_multiple_patterns() {
    local file=$1
    local patterns=("${@:2}")
    local grep_flags=""
    local s21_grep_flags=""

    for pat in "${patterns[@]}"; do
        grep_flags+=" -e \"$pat\""
        s21_grep_flags+=" -e \"$pat\""
    done
    if [ -f $file ]; then
        grep $grep_flags "$file" > original_output.txt
        $S21_GREP $s21_grep_flags "$file" > custom_output.txt

        if diff -u original_output.txt custom_output.txt > /dev/null; then
            echo "Тест [Флаги: \"$s21_grep_flags\"  Шаблоны: \"${patterns[@]}\" Файл: \"$file\"]: Успешно"
        else
            echo "Тест [Флаги: \"$s21_grep_flags\"  Шаблоны: \"${patterns[@]}\" Файл: \"$file\"]: Ошибка"
            diff -u original_output.txt custom_output.txt
        fi

        rm original_output.txt custom_output.txt
     else
        echo "Файл не найден: $file"
    fi
}


files_to_test=(
    "tests/test_1_grep.txt"
    "tests/test_2_grep.txt"
    "tests/test_3_grep.txt"
)

patterns_to_test=(
    "lcs"
    "test"
    "pattern"
    "king"
)

flags_to_test=(
    ""
    "-e"
    "-i"
    "-v"
    "-c"
    "-l"
    "-n"
    "-h"
    "-s"
    "-f"
    "-o"
    "-iv"
    "-in"
    "-nv"
    "-ic"

    
)

for file in "${files_to_test[@]}"; do
    for flags in "${flags_to_test[@]}"; do
        if [[ $flags == "-f" ]]; then
            for file_pattern in "${file_patterns_to_test[@]}"; do
                if [[ -f $file ]]; then
                    compare_output_f "$flags" "$file_pattern" "$file"
                else
                    echo "Файл не найден: $file"
                fi
            done
        else
            for pattern in "${patterns_to_test[@]}"; do
                if [[ -f $file ]]; then
                    compare_output "$flags" "$pattern" "$file"
                else
                    echo "Файл не найден: $file"
                fi
            done
        fi
       
        if [[ $flags == "-e" ]]; then
            if [[ -f $file ]]; then
                compare_multiple_patterns "$file" "${patterns_to_test[@]}"
            else
                echo "Файл не найден: $file"
            fi
        fi
        
    done
    
done


