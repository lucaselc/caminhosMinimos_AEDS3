#!/usr/bin/env zsh

typeset -i i
function gen() {
    i+=1
    ../gen $1 test-$i.txt
}

for x in $(seq 4)
do
    x=$((10**x))
    gen $x
    gen $((x*2))
    gen $((x*3))
    gen $((x*5))
    gen $((x*7))
done

    
