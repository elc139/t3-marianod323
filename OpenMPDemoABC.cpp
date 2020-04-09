#include <algorithm>
#include <iostream>
#include <string>
#include <omp.h>

class SharedArray{
    private:
        char *array;
        int index;
        int size;
        bool useMutex;

    public:
        SharedArray(int n, bool useMutex): size(n), index(0), useMutex(useMutex){
            array = new char[size];
            std::fill(array, array + size, '-');
        }
        ~SharedArray(){
            delete[] array;
        }
        void addChar(char c){
            if (useMutex){
                #pragma omp ordered
                {
                    array[index] = c;
                    spendSomeTime();
                    index++;
                }
            }
            else{
                array[index] = c;
                spendSomeTime();
                index++;
            }
        }
        int countOccurrences(char c){
            return std::count(array, array + size, c);
        }
        std::string toString(){
            return std::string(array, size);
        }

    private:
        void spendSomeTime(){
            for (int i = 0; i < 10000; i++){
                for (int j = 0; j < 100; j++){
                    // These loops shouldn't be removed by the compiler
                }
            }
        }
};

class ArrayFiller{
    private:
        static const int nThreads = 3;
        static const int nTimes = 20;
        omp_sched_t scheduleType;
        int chunkNum;
        SharedArray *array;
        bool runtimeSet;

    public:
        ArrayFiller(bool useMutex, omp_sched_t scheduleType, int chunkNum, bool runtimeSet): scheduleType(scheduleType), chunkNum(chunkNum), runtimeSet(runtimeSet){
            array = new SharedArray(nThreads * nTimes, useMutex);
        }
        void fillArrayConcurrently(){
            omp_set_num_threads(nThreads);

            if (!runtimeSet)
                omp_set_schedule(scheduleType, chunkNum);

            #pragma omp parallel for schedule(runtime) shared(array) ordered
                for (int i = 0; i < nThreads * nTimes; ++i)
                    array->addChar('A' + omp_get_thread_num());
        }
        void printStats(){
            std::cout << array->toString() << std::endl;
            for (int i = 0; i < nThreads; ++i)
                std::cout << (char)('A' + i) << "="
                        << array->countOccurrences('A' + i) << " ";
            std::cout << std::endl;
        }
        ~ArrayFiller(){
            delete array;
        }
};

int main(){
    std::cout << "Each thread should add its char to the array n times (n=20)" << std::endl;

    std::cout << "01 - Runtime with mutex" << std::endl;
    ArrayFiller c1(true, omp_sched_auto, 0, true);
    c1.fillArrayConcurrently();
    c1.printStats();

    std::cout << "02 - Static with chunk and with mutex" << std::endl;
    ArrayFiller c2(true, omp_sched_static, 4, false);
    c2.fillArrayConcurrently();
    c2.printStats();
    
    std::cout << "03 - Static with chunk and without mutex" << std::endl;
    ArrayFiller c3(false, omp_sched_static, 4, false);
    c3.fillArrayConcurrently();
    c3.printStats();
    
    std::cout << "04 - Static without chunk and with mutex" << std::endl;
    ArrayFiller c4(true, omp_sched_static, 0, false);
    c4.fillArrayConcurrently();
    c4.printStats();
    
    std::cout << "05 - Dynamic with chunk and with mutex" << std::endl;
    ArrayFiller c5(true, omp_sched_dynamic, 4, false);
    c5.fillArrayConcurrently();
    c5.printStats();
    
    std::cout << "06 - Dynamic with chunk and without mutex" << std::endl;
    ArrayFiller c6(false, omp_sched_dynamic, 4, false);
    c6.fillArrayConcurrently();
    c6.printStats();
    
    std::cout << "07 - Dynamic without chunk and with mutex" << std::endl;
    ArrayFiller c7(true, omp_sched_dynamic, 0, false);
    c7.fillArrayConcurrently();
    c7.printStats();
    
    std::cout << "08 - Guided with chunk and with mutex" << std::endl;
    ArrayFiller c8(true, omp_sched_guided, 4, false);
    c8.fillArrayConcurrently();
    c8.printStats();
    
    std::cout << "09 - Guided without chunk and with mutex" << std::endl;
    ArrayFiller c9(true, omp_sched_guided, 0, false);
    c9.fillArrayConcurrently();
    c9.printStats();

    std::cout << "10 - Auto with mutex" << std::endl;
    ArrayFiller c10(true, omp_sched_static, 0, false);
    c10.fillArrayConcurrently();
    c10.printStats();

}
