#include <iostream>
#include <vector>

template<typename T>
struct Node {
	std::string key;
	T value;
	Node<T>* next;

	Node(std::string k, T val) : key(std::move(k)), value(val), next(nullptr){}
};

template<typename T>
class Hash{
private:
	int size, capacity;
	std::vector<Node<T>*> table;
	
	int hashFunction(const std::string& key) const{
		long long sum = 0, factor = 31;
		for(char ch : key){
			sum = (sum * factor + ch) % capacity;
		}
		return sum;
	}

	void rehash(){
		int oldCapacity = capacity;
		capacity *= 2;
		std::vector<Node<T>*> newTable(capacity, nullptr);

		for (int i = 0; i < oldCapacity; ++i) {
			Node<T>* current = table[i];
			while (current) {
				int newIndex = hashFunction(current->key);
				Node<T>* nextNode = current->next;

				if (newTable[newIndex]) {
					Node<T>* tail = newTable[newIndex];
					while (tail->next) {
						tail = tail->next;
					}
					tail->next = current;
					current->next = nullptr;
				} else {
					newTable[newIndex] = current;
					current->next = nullptr;
				}
				current = nextNode;
			}
		}
		table = std::move(newTable);
	}

	void clearTable(std::vector<Node<T>*>& n){
		for (Node<T>* head : n){
			while (head){
				Node<T>* nextNode = head->next;
				delete head;
				head = nextNode;
			}
		}
	}

public:
	Hash(int capacity = 10) : size(0), capacity(capacity), table(capacity, nullptr){}

	~Hash(){
		clearTable(table);
		size = 0;
	}

	void insert(const std::string& key, T value){
		if ((size + 1) > 0.7 * capacity){
			rehash();
		}

		int index = hashFunction(key);
		for (Node<T>* iter = table[index]; iter != nullptr; iter = iter->next){
			if (iter->key == key){
				iter->value = value;
				return;
			}
		}
		Node<T>* newNode = new Node<T>(key, value);
		newNode->next = table[index];
		table[index] = newNode;
		++size;
	}

	const T* search(const std::string& key)const{
		int index = hashFunction(key);
		for (Node<T>* iter = table[index]; iter != nullptr; iter = iter->next){
			if (iter->key == key){
				return &(iter->value);
			}
		}
		return nullptr;
	}

	bool remove(const std::string& key){
		int index = hashFunction(key);
		Node<T>* iter = table[index];
		Node<T>* prev = nullptr;
		while (iter != nullptr && iter-> key != key){
			prev = iter;
			iter = iter->next;
		}

		if (iter == nullptr){return false;}

		if (prev == nullptr){
			table[index] = iter->next;
		}else{
			prev->next = iter->next;
		}
		delete iter;
		--size;
		return true;
	}

	std::vector<std::string> keys()const{
		std::vector<std::string> keysList;
		for (Node<T>* head : table){
			while(head){
				keysList.push_back(head->key);
				head = head->next;
			}
		}
		return keysList;
	}

	std::vector<T> values()const{
		std::vector<T> valuesList;
		for (Node<T>* head : table){
			while(head){
				valuesList.push_back(head->value);
				head = head->next;
			}
		}
		return valuesList;
	}

	int getSize()const{return size;}

	int getCapacity()const{return capacity;}

	bool haveKey(const std::string& key)const{
		return search(key) != nullptr;
	}

};