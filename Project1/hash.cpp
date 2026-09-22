#include "hash.h"
#include <vector>
#include <string>

#include <stdio.h>

int hashTable::hash(const std::string &key){
    //FNV hash function
    int FNV_PRIME = 0x01000193;
    int FNV_OFFSET_BASIS = 0x811c9dc5;

    int hash = FNV_OFFSET_BASIS;

    for(char c : key) {
        hash ^= static_cast<int>(c);
        hash *= FNV_PRIME;
    }

    return static_cast<int>(hash % capacity); //cast ensures returned hash is positive int
}
    

int hashTable::findPos(const std::string &key){
    int initial_index = hash(key);
    int current_index = initial_index;
    do {
        if (!data[current_index].isOccupied && !data[current_index].isDeleted) {
            return -1;
        }

        if (data[current_index].isOccupied && !data[current_index].isDeleted) {
            if (data[current_index].key == key) {
                return current_index;
            }
        }
        current_index = (current_index + 1) % capacity;
    } while (current_index != initial_index);
    return -1;
}

bool hashTable::rehash(){
    std::vector<hashItem> temp;
    int oldCapacity = capacity;
    capacity = getPrime(capacity+1);
    filled = 0;
    try{
        temp.resize(capacity); //does this reserve memory too?
        for(int i = 0; i < oldCapacity; i++){
            hashItem cur = data[i];
            if (cur.isDeleted || !cur.isOccupied){
                continue;
            }
            else{
                int newIndex = hash(cur.key);
                temp[newIndex] = cur;
                filled++;
            }
        }
        std::swap(temp,data);
        return true;
    } 
    catch(...){
        return false;
    }
}
unsigned int hashTable::getPrime(int size){
    int primes[] = {
        53,
        97,
        193,
        389,
        769,
        1543,
        3079,
        6151,
        12289,
        24593,
        49157,
        98317,
        196613,
        393241,
        786433,
        1572869,
        3145739,
        6291469  
    };

    for (int i : primes){
        if (i>size){
            return i;
        }
    }
    return primes[16];
}

hashTable :: hashTable(int size){
    //get the prime first
    int prime = hashTable::getPrime(size);
    filled = 0;
    capacity = getPrime(size);
    data.resize(prime);
}

int hashTable::insert(const std::string &key, void *pv){
    if (findPos(key) < 0){
        return 1;
    }
    if((double)(filled+1)/capacity > 0.5){ //load factor  
        if(rehash() < 0){
            return 2;
        }        
    }
    int index = hash(key);
    while(data[index].isOccupied && !data[index].isDeleted){
        index = (index + 1) % capacity;
    }
    data[index] = hashItem();
    data[index].key = key;
    data[index].isOccupied = true;
    data[index].pv = pv;
    filled++;
    return 0;
}
bool hashTable::contains(const std::string &key){ //findPos function
    return !(findPos(key)<0);
}

void* hashTable::getPointer(const std::string &key, bool *b){
    int i = findPos(key);
    if(b != nullptr){
        if(i<0){
            *b = false;
        }
        else{
            *b = true;
        }
    }
    if (i<0){
        return nullptr;
    }
    else{
        return data[i].pv;
    }
}
int hashTable::setPointer(const std::string &key, void *pv){
    int i = findPos(key);
    if(i<0){
        return 1;
    }
    else{
        data[i].pv = pv;
        return 0;
    }
}

bool hashTable::remove(const std::string &key){//lazy deletion, do not deincrement filled
    int i = findPos(key);
    if(i<0){
        return false;
    }
    else{;
        data[i].isDeleted = true;
        return true;
    }
}

