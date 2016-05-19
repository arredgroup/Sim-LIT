#!/bin/bash

lossrate=10
nfile=1

./genPackLoss -h 128 -w 128 -hb 8 -wb 8 -b 1 -ipath ./ -loss-rate $lossrate -s 120

mv loss-file.in\_$lossrate\_1.in loss-file\_$lossrate\_1.in

let nfile=$nfile+1

while [ $nfile -le 2000 ]; do

	./genPackLoss -h 128 -w 128 -hb 8 -wb 8 -b 1 -ipath ./ -loss-rate $lossrate -s 120
        fdupes ./ > duplicated.tmp
        fdupes_duplicated=$(wc -l duplicated.tmp)
        array_count_duplicated=(${fdupes_duplicated// / })
        count_duplicated=${array_count_duplicated[0]}
        if [ $count_duplicated -ge 1 ]; then
          let nfile=$nfile-1
          rm loss-file.in_$lossrate_1.in
          echo "Hay duplicados"
        else
          mv loss-file.in\_$lossrate\_1.in loss-file\_$lossrate\_$nfile.in
          echo "No hay duplicados"
        fi
	
let nfile=$nfile+1
done

rm loss-file.in\_$lossrate\_1.in
rm duplicated.tmp


