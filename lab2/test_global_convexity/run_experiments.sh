INPUT_LIST=("kroA200.tsp" "kroB200.tsp")

for START_VERTEX in $(seq 0 999)
do
    OUTPUT_FILENAME="output-${START_VERTEX}.json"
    for INPUT_FILENAME in "${INPUT_LIST[@]}"
    do
        echo $INPUT_FILENAME, $START_VERTEX
        ../main.out -solver greedy-ls \
                -in ../data/$INPUT_FILENAME \
                -out results/${INPUT_FILENAME%.tsp}/$OUTPUT_FILENAME \
                -start-vertex $START_VERTEX \
                -init-sol-gen random-walk
    done
done