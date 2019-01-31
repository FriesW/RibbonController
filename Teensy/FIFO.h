#ifndef FIFO_H
#define FIFO_H

#include<Arduino.h>

template <class T, unsigned int array_size>
class FIFO {

    private:
        T array[array_size];
        
        unsigned int front = 0;
        bool full = false;
        const unsigned int size = array_size;
        
    public:
        T newest() const {
            if(front != 0)
                return array[front-1];
            else
                return array[size-1];
        }
        
        T oldest() const {
            return array[front];
        }

        T index(unsigned int i) const {
            i += front;
            i = i % size;
            return array[i];
        }
        
        T max() const {
            T c_max = array[0];
            for(unsigned int i = 1; i < size; i++)
                if(c_max < array[i]) c_max = array[i];
            return c_max;
        }
        
        T min() const {
            T c_min = array[0];
            for(unsigned int i = 1; i < size; i++)
                if(c_min > array[i]) c_min = array[i];
            return c_min;
        }
        
        T average() const {
            T c_sum = array[0];
            for(unsigned int i = 1; i < size; i++)
                c_sum += array[i];
            return c_sum / size;
        }
        
        bool flushed() const {
            return full;
        }
        
        T push(T new_element) {
            T out = array[front];
            array[front] = new_element;
            
            front++;
            if(front == size){
                front = 0;
                full = true;
            }
            
            return out;
        }

};

#endif //FIFO_H