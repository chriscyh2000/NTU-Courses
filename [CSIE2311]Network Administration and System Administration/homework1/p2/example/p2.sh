#! /usr/bin/env bash

root=$1
argc=$#
layer=9223372036854775807
rtag=0
atag=0
itag=0
stag=0
cdir=$(pwd)
comp=""
i_arr=()
visited=()
checker_visited=()
function my_print() {
    local item link link_inode
    item=$1
    if [[ stag -eq 1 ]]; then
        if [[ -L "${item}" ]]; then
            if [[ my_layer -eq 0 ]]; then
                echo "${item} -> $(readlink ${item})"
            else
                echo " ${item} -> $(readlink ${item})"
            fi
        else
            if [[ my_layer -eq 0 ]]; then
                echo "${item}"
            else
                echo " ${item}"
            fi
        fi
    else
        if [[ -L "${item}" ]]; then
            link=$(readlink ./${item})
            link_inode=$(stat -c "%i" ./${link})
            if printf '%s\n' "${visited[@]}" | grep -Fxq ${link_inode}; then
                if [[ my_layer -eq 0 ]]; then
                    if [[ my_layer -eq layer ]]; then
                        echo "${item}"
                    else
                        echo "${item} (loop)"
                    fi
                else
                    if [[ my_layer -eq layer ]]; then
                        echo " ${item}"
                    else
                        echo " ${item} (loop)"
                    fi
                fi
            else
                if [[ my_layer -eq 0 ]]; then
                    echo "${item}"
                else
                    echo " ${item}"
                fi
            fi
        else
            if [[ my_layer -eq 0 ]]; then
                echo "${item}"
            else
                echo " ${item}"
            fi
        fi
    fi
}
function my_tree() {
    local content cur_dir my_layer i_check cur_inode next_inode link_inode link
    cur_dir=$1
    my_layer=$2
    i_check=0
    cur_inode=$(stat -c "%i" ./${cur_dir})
    if [[ -L "${cur_dir}" ]]; then
        link=$(readlink ./${item})
        cur_inode=$(stat -c "%i" ./${link})
    fi
    visited+=("${cur_inode}")
    if [[ itag -eq 1 ]]; then
        if [[ "$(echo ${cur_dir} | grep ${comp})" != "" ]]; then
            i_check=1
        fi
        if [[ i_check -eq 0 ]]; then
            if printf '%s\n' "${i_arr[@]}" | grep -Fxq ${cur_inode}; then
                i_check=1
            fi
        fi
        if [[ my_layer -eq 0 ]]; then
            i_check=1
        fi
        if [[ i_check -eq 0 ]]; then
            return
        fi
    fi
    # echo ${cur_dir}
    if [[ layer -lt my_layer ]]; then
        return
    fi
    cd ${cur_dir}

    # -a
    if [[ atag -eq 1 ]]; then
        content=$(ls -a)
        content=( "${content[@]/"."}" )
        content=( "${content[@]/".."}" )
    else
        content=$(ls)
    fi

    str=""
    for item in ${content[@]}; do
        str=${str}${item}$'\n'
    done

    #-r
    if [[ rtag -eq 1 ]]; then
        str=$(echo -n "${str}" | LC_ALL=C sort -r)
    else
        str=$(echo -n "${str}" | LC_ALL=C sort)
    fi
    OLD_IFS=${IFS}
    IFS=$'\n'
    content=($str)
    IFS=${OLD_IFS}

    for item in ${content[@]}; do
        next_inode=$(stat -c "%i" ./${item})
        if [[ itag -eq 0 ]]; then
            printf "| "
            for ((i = 0 ; i < ${my_layer} ; i++)); do
                echo -n "---"
            done
            my_print ${item}
        elif [[ "$(echo ${item} | grep ${comp})" != "" ]]; then
            printf "| "
            for ((i = 0 ; i < ${my_layer} ; i++)); do
                echo -n "---"
            done
            my_print ${item}
        elif printf '%s\n' "${i_arr[@]}" | grep -Fxq ${next_inode}; then
            printf "| "
            for ((i = 0 ; i < ${my_layer} ; i++)); do
                echo -n "---"
            done
            my_print ${item}
        fi
        if [[ stag -eq 0 ]] && [[ -L "${item}" ]]; then
            link=$(readlink ./${item})
            link_inode=$(stat -c "%i" ./${link})
            if printf '%s\n' "${visited[@]}" | grep -Fxq ${link_inode}; then
                link_inode=""
            else
                my_tree ${item} $((${my_layer}+1)) 
            fi
        elif [[ -d "${item}" ]] && [[ ! -L "${item}" ]]; then
            my_tree ${item} $((${my_layer}+1)) 
        fi
    done
    cd ..
    visited=("${visited[@]/${cur_inode}}")
}
function checker() {
    local content cur_dir my_layer add cur_inode link
    add=0
    cur_dir=$1
    my_layer=$2
    cur_inode=$(stat -c "%i" ./${cur_dir})
    if [[ -L "${cur_dir}" ]]; then
        link=$(readlink ./${item})
        cur_inode=$(stat -c "%i" ./${link})
    fi
    if [[ layer -lt my_layer ]]; then
        return 0;
        return 
    fi
    if printf '%s\n' "${checker_visited[@]}" | grep -Fxq ${cur_inode}; then
        return 0;
        return 
    fi
    cd ${cur_dir}
    checker_visited+=("${cur_inode}")
    # -a
    if [[ atag -eq 1 ]]; then
        content=$(ls -a)
        content=( "${content[@]/"."}" )
        content=( "${content[@]/".."}" )
    else
        content=$(ls)
    fi
    for item in ${content[@]}; do
        if [[ "$(echo ${item} | grep ${comp})" != "" ]]; then
            add=1
        fi
        if [[ -d "${item}" ]]; then
            checker ${item} $((${my_layer}+1))
            tmp_add=$?
            if [[ tmp_add -eq 1 ]]; then
                add=1
            fi
        fi
    done
    if [[ add -eq 1 ]]; then
        my_i=$(stat -c "%i" .)
        i_arr+=("${my_i}")
        cd ..
        return 1;
        return
    fi
    cd ..
    checker_visited=("${checker_visited[@]/${cur_inode}}")
    return 0;
}
start=2
if [[ "$1" =~ ^- ]]; then
    start=1
    root=.
fi
if [[ $# -eq 0 ]]; then
    root=.
fi
for ((i = ${start} ; i <= ${argc} ; i++)); do
    if [[ ${!i} == "-l" ]]; then
        ((i+=1))
        layer=${!i}
    elif [[ ${!i} == "-a" ]]; then
        atag=1
    elif [[ ${!i} == "-r" ]]; then
        rtag=1
    elif [[ ${!i} == "-s" ]]; then
        stag=1
    elif [[ ${!i} == "-i" ]]; then
        itag=1
        ((i+=1))
        comp=${!i}
    fi
done

if [[ itag -eq 1 ]]; then
    checker ${root} 0 
fi
cd ${cdir}
echo ${root}
my_tree $root 0