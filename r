clear
# minsup=5%
#time ./mug -fibm2.ascii -s10 -aN -n
#time ./mug -fibm4.ascii -s20 -aN -n
#time ./mug -fibm6.ascii -s30 -aN -n
#time ./mug -fibm8.ascii -s40 -aN -n
#time ./mug -fibm10.ascii -s50 -aN -n
#real    0m3.513s
#real    0m4.584s
#real    0m7.263s
#real    0m8.388s
#real    0m9.846s

# minsup=8%
#time ./mug -fibm2.ascii -s14 -aN -n
#time ./mug -fibm4.ascii -s28 -aN -n
#time ./mug -fibm6.ascii -s42 -aN -n
#time ./mug -fibm8.ascii -s56 -aN -n
#time ./mug -fibm10.ascii -s70 -aN -n
#real    0m2.184s
#real    0m3.891s
#real    0m4.423s
#real    0m5.299s
#real    0m7.132s

# minsup=4.2%, maxsup=80%
#time ./mug -fibm2.ascii -s84 -x1600 -aU -n
#time ./mug -fibm4.ascii -s168 -x3200 -aU -n
#time ./mug -fibm6.ascii -s254 -x4800 -aU -n
#time ./mug -fibm8.ascii -s338 -x6400 -aU -n
#time ./mug -fibm10.ascii -s420 -x8000 -aU -n
#real    0m2.690s
#real    0m2.756s
#real    0m3.028s
#real    0m3.427s
#real    0m3.849s

# minsup=4.2%, maxsup=90%
#time ./mug -fibm2.ascii -s84 -x1800 -aU -n
#time ./mug -fibm4.ascii -s168 -x3600 -aU -n
#time ./mug -fibm6.ascii -s254 -x5400 -aU -n
#time ./mug -fibm8.ascii -s338 -x7200 -aU -n
#time ./mug -fibm10.ascii -s420 -x9000 -aU -n
#real    0m3.085s
#real    0m3.258s
#real    0m3.543s
#real    0m3.933s
#real    0m4.471s

time ./mug -fibm1.ascii -s60 -x1000 -aC -n
time ./mug -fibm2.ascii -s120 -x2000 -aC -n
time ./mug -fibm3.ascii -s180 -x3000 -aC -n
time ./mug -fibm4.ascii -s240 -x4000 -aC -n
time ./mug -fibm5.ascii -s300 -x5000 -aC -n


#time ./mug -ftest.ascii -s1 -aN -c
#time ./mug -fchess.ascii -s2000 -aN -n
#time ./mug  -i6 -t9 -d0.45 -s1 -m10 -aN
