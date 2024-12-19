#!/usr/bin/env bash

cat <<__EOF__
\begin{latin}
\begin{table}[h]
\centering
\begin{tabular}{|l|c|c|c|}
\hline
precision & real & user & sys \\\\\hline\hline
__EOF__

to=/tmp/to.txt
for i in 1 0.1 0.01 0.001 0.0001 0.00001 0.000001 0.0000005 0.00000025 0.0000001; do
        sudo sync
        echo 3 | sudo tee /proc/sys/vm/drop_caches > /dev/null

        { time ./fouriersolver $@ -p $i; } 2> $to 1> /dev/null
        real=$(grep real $to | cut -f2)
        user=$(grep user $to | cut -f2)
        sys=$(grep sys $to | cut -f2)
        
        echo "$i & $real & $user & $sys \\\\ \hline"
done

cat <<__EOF__
\end{tabular}
\end{table}
\end{latin}
__EOF__

rm -rf $to

