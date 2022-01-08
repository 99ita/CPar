# CPar

Correr fazendo primeiro make e depois ./run

Bucket sort: Pega na lista de inteiros com N elementos e divide os elementos em N buckets
             cada bucket corresponde a um intervalo de valores o primeiro vai de 0 a X 
             e o ultimo vai de Y ao maior valor do array. 
             Depois cada bucket é sorted usando outro algoritmo (tipicamente insertion sort
             por se comportar como O(n) quando os elementos nao estao muito longe da posicao final). 
             No final percorre-se cada bucket por ordem e copia-se os valores para o array final

Plano:
    -Percorrer o array para encontrar o valor maximo* ou usar o MAX_RAND?

    -Buckets podem ser:
        -Arrays dinamicos: Chato de implementar e pode ser pouco eficiente pois, como lidamos com arrays 
                           pequenos, temos que estar sempre a copiar o array (assumindo que o array começa
                           com size 1 e dobra sempre que é necessario mais, poderia ser implementada outra 
                           solucao aqui)
        
        -Arrays Estaticos: Consome muita memória (O(n^2) em vez de O(n)) mas muito facil de implementar

        -Listas ligadas: Resolve o problema dos arrays dinamicos e o overhead de memória não e muito grande
                         nesta estrategia ao ordenar os buckets podia-se logo ordenar para um array usando 
                         insertion sort (é preciso saber o tamanho total do bucket:
                                            -criar uma "superstruct" que guarda sempre o tamanho da ll
                                            -percorrer a ll para saber o tamanho do bucket)

        Ja implementei insertionsort para arrays e para listas ligadas a ordenar logo para um array

    -Percorrer o array para preencher os buckets*

    -Percorrer os buckets ordenando-os*

    -Qual a melhor maneira de juntar os arrays ordenados correspondentes a cada bucket?*
        -Pode se tentar criar logo o array total e depois percorrer os buckets sequencialmente (dificil de paralelizar)
        -Dividir o sorting dos buckets por threads e atribuir uma tag a cada pacote de buckets (que será assigned a uma thread)
        de forma a mais tarde conseguirmos juntar todos


    *: Zonas com asterisco são as que me parecem ser paralelizáveis