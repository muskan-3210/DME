template<typename T>
class DynamicArray{
private:
    T* data;
    size_t size_;
    size_t capcity_;
public:
    DynamicArray();

    explicit DynamicArray(size_t, initial_Capacity)
    ~DynamicArray();

    void push_back(const T& value);
    void pop_back();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    size_t size() const;
    size_t capacity() const;
    bool empty() const;

    void clear();
    void reserve(size_t new_Capacity);
  
};

template<typename T>
DynamicArray<T>::DynamicArray(): data_(nullptr), size_(0), capacity_(0){}

template<typename T>
void DynamicArray<T>::push_back(const T& value){
    if(size_ == capacity_){
        reverse(capacity_ == 0 ? 1: capacity_ * 2)
    }
    data_[size++] = value;
}

template<typename T>
void DynamicArray<T>::pop_back(){
    if(size == 0)return;
    --size_;
}

// template<typename T>
// void DynamicArray<T>::operator[](size_t index){
//     return data_[index];
// }