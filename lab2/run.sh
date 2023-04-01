#./run.sh steepest-ls data/kroA100.tsp 92
# ./main.out -solver $1 -in $2 -out output1.json -start-vertex $3 -init-sol-gen greedy-cycle -neigh N1 -iterations 1
./main.out -solver $1 -in $2 -out output2.json -start-vertex $3 -init-sol-gen greedy-cycle -neigh N2 -iterations 1
python3 visualization.py data/kroA100.csv output1.json result1.pdf
python3 visualization.py data/kroA100.csv output2.json result2.pdf
open -a Preview result1.pdf result2.pdf
 