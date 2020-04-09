# Trabalho 3 - Programação Paralela

Mariano Dorneles de Freitas

## Compilando o programa

```bash
g++ -std=c++11 -o NomeDoPrograma OpenMPDemoABC.cpp -fopenmp
```

## Explicação geral

O programa foi desenvolvido com base no programa disponibilizado no repósitório, o teste foi realizado com as mesmas condições: 3 threads, 60 espaços no vetor de trabalho e chunks sempre setados com 4 espaços.

O escopo do método principal está definido abaixo, foi alterado para receber o tipo de schedule (para ser inicializada na própria função), o número de chunks (setado em 4 em todas as chamadas do método na main) e uma booleana para setar se é ou não a execução que utilizará o runtime.

Os resultados ordenados deve-se à clausula ORDERED utilizada para acessos à região crítica do programa.

```cpp
ArrayFiller(bool useMutex, omp_sched_t scheduleType, int chunkNum, bool runtimeSet)
```

## Saída
```bash
01 - Runtime with mutex
BCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCA
A=20 B=20 C=20 
02 - Static with chunk and with mutex
AAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCC
A=20 B=20 C=20 
03 - Static with chunk and without mutex
BABCABCCAB--CCBABACBCA--CCBACBACBA--CCBAABCABC-BCCBACBACBA--
A=15 B=17 C=19 
04 - Static without chunk and with mutex
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC
A=20 B=20 C=20 
05 - Dynamic with chunk and with mutex
BBBBAAAACCCCBBBBAAAACCCCBBBBAAAACCCCBBBBAAAACCCCBBBBAAAACCCC
A=20 B=20 C=20 
06 - Dynamic with chunk and without mutex
CCBACBACBA-BCCBACBACBA-BCBCACBACBA-BCBCABACBACBACBACBACBCA--
A=16 B=19 C=20 
07 - Dynamic without chunk and with mutex
BCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCA
A=20 B=20 C=20 
08 - Guided with chunk and with mutex
CCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBAAAAAAAAACCCCCCBBBBAAAACCC
A=13 B=18 C=29 
09 - Guided without chunk and with mutex
CCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBAAAAAAAAACCCCCCBBBBAAACCBA
A=13 B=19 C=28 
10 - Auto with mutex
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC
A=20 B=20 C=20 
```

## Comentários de casos específicos

### Casos 1 e 7
No primeiro caso, runtime com mutex, o programa utilizou schedule dinâmico com 1 chunk, escalonando inicialmente a thread B e repetindo, assim, sempre no padrão "BCA", como no caso 7.

### Caso 2
O segundo caso está rodando corretamente, de forma circular e alternando a cada 4 iterações (como esperado de um schedule estático).

### Casos 3 e 6
Estes casos foram definidos sem o Mutex e, como esperado, retornaram erros em sua execução, demonstrando assim a importância de se definir corretamente a seção crítica.

### Casos 4 e 10
No caso definido pelo AUTO (10), o escolhido foi um scheduling estático, sem chunks, retornando uma saída igual ao caso 4 (executando todas as iterações de cada thread por vez).

### Casos 8 e 9
No scheduling guiado, a thread C foi escalonada primeira, conseguindo, assim, mais iterações, uma vez que o guided scheduling é dinâmico e adapta também o tamanho do chunk, o que faz com que a definição do tamanho do chunk seja indiferente para o resultado (uam vez que os resultados foram muito, muito próximos).



## Referências

 1. Slides da disciplina + vídeo aula;
 2. [https://www.ime.usp.br/~oberlan/DCE11720/Aulas/Aula19.pdf](https://www.ime.usp.br/~oberlan/DCE11720/Aulas/Aula19.pdf)
