#! /bin/bash
# check the input format
declare -i host_num
declare -i player_num
declare -i lucky_num
if [ ${1} == "-m" ]; then
    host_num=${2}
elif [ ${1} == "-n" ]; then
    player_num=${2}
else
    lucky_num=${2}
fi
if [ ${3} == "-m" ]; then
    host_num=${4}
elif [ ${3} == "-n" ]; then
    player_num=${4}
else
    lucky_num=${4}
fi
if [ ${5} == "-m" ]; then
    host_num=${6}
elif [ ${5} == "-n" ]; then
    player_num=${6}
else
    lucky_num=${6}
fi
#build up the combinations
count=0
comb=()
for ((a=1; a<=${player_num}; a++)); do
    for ((b=$((a+1)); b<=${player_num}; b++)); do
        for ((c=$((b+1)); c<=${player_num}; c++)); do
            for ((d=$((c+1)); d<=${player_num}; d++)); do
                for ((e=$((d+1)); e<=${player_num}; e++)); do
                    for ((f=$((e+1)); f<=${player_num}; f++)); do
                        for ((g=$((f+1)); g<=${player_num}; g++)); do
                            for ((h=$((g+1)); h<=${player_num}; h++)); do
                                ((count++))
                                comb[${count}]="${a} ${b} ${c} ${d} ${e} ${f} ${g} ${h}\n"
                            done
                        done
                    done
                done
            done
        done
    done
done
# for ((i=1; i<=${count}; i++)); do
#     echo -e ${comb[${i}]}
# done
# init fifo
if [ ${host_num} -gt ${count} ]; then
    host_num=${count}
fi
for ((i=0; i<=${host_num}; i++)); do
    mkfifo fifo_${i}.tmp
done
fd=()
for ((i=1; i<=${host_num}; i++)); do
    exec {t}<>fifo_${i}.tmp
    fd[$i]=${t}
done

# init score
score=()
for ((i=1; i<=${count}; i++)); do
    score[$i]=0
done

#assign every host a combination first
for ((i=1; i<=${host_num}; i++)); do
    ./host -m ${i} -d 0 -l ${lucky_num} &
    echo -e ${comb[$((i))]} >&${fd[${i}]}
done
exec <fifo_0.tmp
for ((i=$((${host_num}+1)); i<=${count}; i++)); do
    read host_id
    for ((j=0; j<8; j++)); do
        read player_id add
        score[player_id]=$((${score[player_id]}+${add}))
    done
    echo -e ${comb[$((i))]} >&${fd[${host_id}]}
done
for ((i=1; i<=${host_num}; i++)); do
    read host_id
    for ((j=0; j<8; j++)); do
        read player_id add
        score[$player_id]=$((${score[player_id]}+${add}))
    done
    echo -e "-1 -1 -1 -1 -1 -1 -1 -1\n" >&${fd[${host_id}]}
done
rank=()
for ((i=1; i<=${player_num}; i++)); do
    rank[${i}]=1
    for ((j=1; j<=${player_num}; j++)); do
        if [ ${score[${j}]} -gt ${score[${i}]} ]; then
            rank[${i}]=$((${rank[${i}]}+1))
        fi
    done
done
for ((i=1; i<=${player_num}; i++)); do
    echo ${i} ${rank[${i}]}
done
# for ((i=1; i<=${player_num}; i++)); do
#     echo player:${i} score:${score[${i}]}
# done
wait
rm -f fifo* 