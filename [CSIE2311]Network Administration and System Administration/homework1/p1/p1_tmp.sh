#! /usr/bin/env bash

argc=$#
arg_it=0
arr=()
check_st=0
check_end=0
st=-1
end=-1
cur_in=""
function print_error() {
    echo "usage: ./p1.sh -s <Unix timestamp > -e <Unix timestamp > <input file>"
    echo "ex: ./p1.sh -s 1133642864 -e 1133643505 log1.txt log3.txt"
    exit
}
if [[ argc -le 4 ]]; then
    print_error
fi
while [[ arg_it -ne argc ]]; do
    ((arg_it+=1))
    str=${!arg_it}
    if [[ arg_it -lt 5 ]]; then
        if [[ $((arg_it%2)) -eq 1 ]]; then
            if [[ "${!arg_it}" != "-s" ]] && [[ "${!arg_it}" != "-e" ]]; then
                print_error
            elif [[ check_st -eq 1 ]] && [[ "${!arg_it}" == "-s" ]]; then
                print_error
            elif [[ check_end -eq 1 ]] && [[ "${!arg_it}" == "-e" ]]; then
                print_error
            else
                if [[ "${!arg_it}" == "-s" ]]; then
                    check_st=1
                    cur_in="s"
                else
                    check_end=1
                    cur_in="e"
                fi
            fi
        else
            if [[ ! -z "${str##*[!0-9]*}" ]]; then
                if [[ "${cur_in}" == "s" ]]; then
                    st=${!arg_it}
                else
                    end=${!arg_it}
                fi
            else
                print_error
            fi
        fi
        if [[ arg_it -eq 4 ]] && [[ st -gt end ]]; then
            print_error
        fi
    else
        if [[ ${!arg_it} =~ ^- ]]; then
            print_error
        else
            arr+=(${!arg_it})
        fi
    fi
done


log_arr=""
tmp=""
for file in ${arr[@]}; do
    while IFS= read -r line; do
        tmp=${line:1:24}
        tmp=$(date -d "${tmp}" '+%s')
        if [[ tmp -ge st ]] && [[ tmp -le end ]]; then
            tmp=$tmp${line:26}
            log_arr=$log_arr$tmp$'\n'
        fi
    done < ${file}
done
printf "${log_arr}" | sort