

for (( c=1; c<=40; c = c+1 ))
do
	echo hello world $c
	mpic++ parallel.cpp -o parallel
	mpirun -np $c ./parallel
done