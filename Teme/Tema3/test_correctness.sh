#!/bin/bash

make clean
make
mkdir out

echo "Testing single filter..."
for N in `seq 1 8`; do
    for filter in "smooth" "blur" "emboss" "sharpen" "mean"; do
        for image in "darth-vader" "despicable-me" "landscape" "macro" "rorschach"; do
            mpirun -np $N tema3 "input_files/PGM/$image.pgm" "out/$image-$filter-out.pgm" $filter
            compare -verbose -metric PSNR "out/$image-$filter-out.pgm" "ref/pgm/$image-$filter.pgm" "plm.pnm" &>/dev/null && echo "[N = $N] Success $image $filter" || echo "[N = $N] Fail $image $filter"
        done

        for image in "baby-yoda" "landscape" "lena" "macro" "scroll-of-truth"; do
            mpirun -np $N tema3 "input_files/PNM/$image.pnm" "out/$image-$filter-out.pnm" $filter
            compare -verbose -metric PSNR "out/$image-$filter-out.pnm" "ref/pnm/$image-$filter.pnm" "plm.pnm" &>/dev/null && echo "[N = $N] Success $image $filter" || echo "[N = $N] Fail $image $filter"
        done
    done

    echo "Done testing single filter."

    echo "Testing bssembssem..."

    for image in "darth-vader" "despicable-me" "landscape" "macro" "rorschach"; do
        mpirun -np $N tema3 "input_files/PGM/$image.pgm" "out/$image-bssembssem-out.pgm" "blur" "smooth" "sharpen" "emboss" "mean" "blur" "smooth" "sharpen" "emboss" "mean"
        compare -verbose -metric PSNR "out/$image-bssembssem-out.pgm" "ref/pgm/$image-bssembssem.pgm" "plm.pnm" &>/dev/null && echo "[N = $N] Success $image bssembssem" || echo "[N = $N] Fail $image bssembssem"
    done

    for image in "baby-yoda" "landscape" "lena" "macro" "scroll-of-truth"; do
        mpirun -np $N tema3 "input_files/PNM/$image.pnm" "out/$image-bssembssem-out.pnm" "blur" "smooth" "sharpen" "emboss" "mean" "blur" "smooth" "sharpen" "emboss" "mean"
        compare -verbose -metric PSNR "out/$image-bssembssem-out.pnm" "ref/pnm/$image-bssembssem.pnm" "plm.pnm" &>/dev/null && echo "[N = $N] Success $image bssembssem" || echo "[N = $N] Fail $image bssembssem"
    done

    echo "Done testing bssembssem."
done

rm -rf out
