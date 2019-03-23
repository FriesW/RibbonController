/*void ManagerClass::update(){
    Serial.println("B");
    Serial.println(length);
    for(unsigned int i = 0; i < length; i++){
        unsigned int t = (i + last) % length;
        Serial.println(t);
        if(metros[t]->_if_check_then_run()){
            last = t+1;
            Serial.println("EE");
            return;
        }
    }
    Serial.println("E");
}*/

void ManagerClass::attach(Metro* m){
    metros[length] = m;
    length++;
}



void ManagerClass::update(){
    Serial.println("B");
    Serial.println(length);
    for(unsigned int i = 0; i < length; i++){
        Serial.println(i);
        if(metros[i]->_if_check_then_run()){
            Serial.println("EE");
        }
    }
    Serial.println("E");
}