

for((m=1; m>=0; m=m-1)); do
    for((i=5; i<=15; i=i+5)); do
        for((j=10; j<=30; j=j+10)); do
            echo $i $j $m
            ./simulation 50 $i 10 $j $m
        done
    done
done

