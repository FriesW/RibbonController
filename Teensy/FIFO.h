#ifndef FIFO_H
#define FIFO_H

#include<Arduino.h>

template <class T, unsigned int array_size>
class FIFO {

    private:
        T array[array_size];
        
        unsigned int front = 0;
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
            T out = array[0];
            for(unsigned int i=1; i < size; i++)
                if(array[i] > out)
                    out = array[i];
            return out;
        }
        
        T push(T new_element) {
            T out = array[front];
            array[front] = new_element;
            front++;
            if(front == size)
                front = 0;
            return out;
        }

};

#endif //FIFO_H