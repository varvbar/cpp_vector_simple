#pragma once

#include <new>
#include <utility>

template <typename Type>
class ArrayPtr {
public:
    // Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле raw_ptr_ должно быть равно nullptr
    explicit ArrayPtr(std::size_t size) {
       
        if(size) {
            raw_ptr_ = new Type [size];
        }
        else {
            raw_ptr_ = nullptr;
        }
        
    }

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept {
        raw_ptr_ = raw_ptr;
    }

    // Запрещаем копирование
    ArrayPtr(const ArrayPtr&) = delete;

    // Move constructor
    ArrayPtr(ArrayPtr&& arr_temp) {
        
        raw_ptr_ = std::exchange(arr_temp.raw_ptr_, nullptr);
        
    }

    ~ArrayPtr() {
        delete[] raw_ptr_;
    }

    // Запрещаем присваивание 
    ArrayPtr& operator=(const ArrayPtr&) = delete;


    // Присваивание перемещением
    ArrayPtr& operator=(ArrayPtr&& arr_temp) {

        if (this != &arr_temp) {

            raw_ptr_ = std::exchange(arr_temp.raw_ptr_, nullptr);

        }
        
        return *this;
    }

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept {
        Type* temp = raw_ptr_;
        raw_ptr_ = nullptr;

        return temp;
    }

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](std::size_t index) noexcept {

        return raw_ptr_[index];
    }

    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](std::size_t index) const noexcept {
        
        const Type* temp = &raw_ptr_[index];
        
        return temp[0];
    }

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const {
        
        return raw_ptr_ == nullptr ? true : false;
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept {
      
        return raw_ptr_;
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept {
       
        std::swap(raw_ptr_, other.raw_ptr_);
    }

private:
    Type* raw_ptr_ = nullptr;
};