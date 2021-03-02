a=$(./randall 5 | wc -c); if [ $a -eq 5 ]; then echo "success"; else echo "fail"; fi
b=$(./randall -i mrand48_r 10  | wc -c); if [ $b -eq 10 ]; then echo "success"; else echo "fail"; fi
c=$(./randall -i mrand48_r -o 5 10  | wc -c); if [ $c -eq 10 ]; then echo "success"; else echo "fail"; fi
d=$(./randall -i mrand48_r -o stdio 10  | wc -c); if [ $d -eq 10 ]; then echo "success"; else echo "fail"; fi
e=$(./randall -i /dev/random -o stdio 100 | wc -c); if [ $e -eq 100 ]; then echo "success"; else echo "fail"; fi
f=$(./randall -i /dev/random  100 | wc -c); if [ $f -eq 100 ]; then echo "success"; else echo "fail"; fi
g=$(./randall -i rdrand 10| wc -c); if [ $g -eq 10 ]; then echo "success"; else echo "fail"; fi