#ifndef _HASH_H
#define _HASH_H

#include <vector>
#include <string>

#include <stdio.h>

class hashTable{
    hashTable(int size = 0);
    int insert(const std::string &key, void *pv = nullptr);
    bool contains(const std::string &key);
    void *getPointer(const std::string &key, bool *b = nullptr);
    int setPointer(const std::string &key, void *pv);
    bool remove(const std::string &key);

    private:
    class hashItem{
        std::string key{""};
        bool isOccupied {false};
        bool isDeleted {false};
        void *pv {nullptr};
        
        hashItem() = default;
        public:
        hashItem(std::string key);
        hashItem(std::string key, void* pv);
        std::string getkey(){return key;};
        bool getOcc(){return isOccupied;};
        bool getDel(){return isDeleted;};
    };
    
    int capacity;
    int filled;
    
    std::vector<hashItem> data;

    int hash(const std::string &key);

    int findPos(const std::string &key);
    bool rehash();
    static unsigned int getPrime(int size);
};

hashTable::hashItem::hashItem(std::string key, void *pv = nullptr){ 
    key = key;
    pv = pv;
}

hashTable :: hashTable(int size){
    //get the prime first
    int prime = hashTable::getPrime(size);
    filled = 0;
    capacity = prime;
    data.resize(prime);
}
int hashTable::insert(const std::string &key, void *pv = nullptr){
    if (findPos(key) < 0){
        return 1;
    }
    if((double)(filled+1)/capacity > 0.5){ //load factor  
        if(rehash() < 0);
        return 2;        
    }
    int index = hash(key);
    while(data.at(index).getOcc() != false){
        index++;
    }
    data.insert(data.begin() + index, hashItem(key, pv));
}
bool hashTable::contains(const std::string &key){ //findPos function
    int initial_index, current_index = hash(key);
    do {
        if(data.at(current_index).getkey() == key){ //comp string
            return true;
        }
        else if(current_index == capacity){
            current_index = 0;
        }
        else{
            current_index++;
        }
    } while(initial_index != current_index);
    return false;
}

void* hashTable::getPointer(const std::string &key, bool *b = nullptr){
       
}

#endif //_HASH_H