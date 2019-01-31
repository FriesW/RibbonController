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
            if(index != 0)
                return array[index-1];
            else
                return array[size-1];
        }
        
        T oldest() const {
            return array[front];
        }

        T index(unsigned int index) const {
            index += front;
            index = index % size;
            return array[index];
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