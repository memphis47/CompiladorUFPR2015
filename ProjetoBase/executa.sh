echo "compilando..."
make -f Makefile
echo "gerando MEPA..."
./compilador $1 
if [ $? == 0 ]; then
    echo "montando..."
    as  mepa.s -o mepa.o
    if [ $? == 0 ]; then
        echo "ligando..."
        ld mepa.o -o mepa -lc -dynamic-linker /lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 
        if [ $? == 0 ]; then
            echo "Executando"
            ./mepa
        fi
    fi
    echo "FIM"
fi
