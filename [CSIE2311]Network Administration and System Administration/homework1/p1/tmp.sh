#! /usr/bin/env bash
file=$'hello\nworld\napple'
# echo "${file}" | sort


str="hel.ziplo"
# str=$(echo "${file}" | sort)
# OLD_IFS=${IFS}
# IFS=$'\n'
# arr=($str)
# for val in ${arr[@]}; do
#     echo ${val}
# done
if [[ ! -n "${str##*.zip*}" ]]; then
    echo ${str} yes
else 
    echo ${str} no
fi