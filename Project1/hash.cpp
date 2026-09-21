#include "hash.h"
#include <vector>
#include <string>

#include <stdio.h>

/*class hashTable{
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
        std::string getkey(){return key;}
        void setpv(void *p){pv = p;}
        void *getpv(){return pv;}
        bool getOcc(){return isOccupied;}
        bool getDel(){return isDeleted;}
        void flagAsDelete(){isDeleted = true;}
    };
    
    int capacity;
    int filled;
    
    std::vector<hashItem> data;

    int hash(const std::string &key){
        //FNV hash function
        int FNV_PRIME = 0x01000193;
        int FNV_OFFSET_BASIS = 0x811c9dc5;

        int hash = FNV_OFFSET_BASIS;

        for(char c : key) {
            hash ^= static_cast<int>(c);
            hash *= FNV_PRIME;
        }

        return hash % capacity;
    }
    

    int findPos(const std::string &key){
        int initial_index, current_index = hash(key);
        do {
            if(data[current_index].getDel()){
                current_index++;
                continue;
            }
            if(data.at(current_index).getkey() == key){ //comp string
                return current_index;
            }
            else if(current_index == capacity){
                current_index = 0;
            }
            else{
                current_index++;
            }
        } while(initial_index != current_index);
        return -1;
    }
 
    bool rehash(){
        bool a = data[0].isOccupied;
    }
    static unsigned int getPrime(int size);
};*/

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
    while(data.at(index).isOccupied != false){
        index++;
    }
    data[index] = hashItem();
    data[index].key = key;
    data[index].isOccupied = true;
    data[index].pv = pv;
}
bool hashTable::contains(const std::string &key){ //findPos function
    return !(findPos(key)<0);
}

void* hashTable::getPointer(const std::string &key, bool *b = nullptr){
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

bool hashTable::remove(const std::string &key){
    int i = findPos(key);
    if(i<0){
        return false;
    }
    else{;
        data[i].isDeleted = true;
        return true;
    }
}

