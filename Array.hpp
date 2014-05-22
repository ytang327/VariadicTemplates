#include <cstddef>
#include <cassert>
#include <stdexcept> 

namespace cs540{

    class OutOfRange : public std::out_of_range{
        public:
            OutOfRange()
                : std::out_of_range{"Out of Range"}
            { }

    };

    template <typename T, std::size_t D, std::size_t... Ds>
    class Array{
        public:
            using ValueType = T;
            using SubArrayType = Array<T, Ds...>;
            Array() = default;
            ~Array() = default;
            Array(const Array&);
            template <typename U> Array(const Array<U, D, Ds...> &);
            Array& operator=(const Array&); 
            template <typename U>Array &operator=(const Array<U, D, Ds...>&);
            SubArrayType& operator[](std::size_t ind);
            const SubArrayType& operator[](std::size_t ind) const;
            class FirstDimensionMajorIterator{
                private:
                    enum Pos {BEGIN, END};
                    FirstDimensionMajorIterator(Array<T,Ds...> *arr, Pos p)
                        :arr{arr},
                        i{p == BEGIN ? 0 : D}
                        {
                            if (p == BEGIN) {
                                assert(this->i == 0);
                                this->sub_iter = arr[this->i].fmbegin();
                            }
                        }
                public:
                    static FirstDimensionMajorIterator
                        MakeBeginIterator(Array<T,Ds...> *arr)
                    { 
                        return {arr, BEGIN};
                    }

                    static FirstDimensionMajorIterator
                        MakeEndIterator(Array<T,Ds...> *arr)
                    {
                        return {arr, END};
                    }

                    FirstDimensionMajorIterator() = default;
                    FirstDimensionMajorIterator(const FirstDimensionMajorIterator &)
                     = default;
                    FirstDimensionMajorIterator &operator=(
                        const FirstDimensionMajorIterator &) = default;

                    FirstDimensionMajorIterator& operator++() {
                        ++this->sub_iter;
                        if (this->sub_iter == this->arr[this->i].fmend()) {
                            ++this->i;
                            if (this->i < D) {
                                this->sub_iter = this->arr[this->i].fmbegin();
                            }
                        }
                        return *this;
                    }

                    FirstDimensionMajorIterator operator++(int) {
                        FirstDimensionMajorIterator it = *this;
                        ++this->sub_iter;
                        if (this->sub_iter == this->arr[this->i].fmend()) {
                            ++this->i;
                            if (this->i < D) {
                                this->sub_iter = this->arr[this->i].fmbegin();
                            }
                        }
                        return it;
                    }

                    T &operator*() const{
                        return *sub_iter;
                    }

                    Array<T, Ds...> *arr;
                    size_t i;
                    using SubIter = decltype(arr[0].fmbegin());
                    SubIter sub_iter;

            };
            FirstDimensionMajorIterator fmbegin();
            FirstDimensionMajorIterator fmend();

            friend bool operator==(const FirstDimensionMajorIterator &iter1,
                    const FirstDimensionMajorIterator &iter2){
                return (iter1.i == iter2.i) && 
                    ((iter1.i == D && iter2.i == D) ||
                     (iter1.sub_iter == iter2.sub_iter));
            }

            friend bool operator!=(const FirstDimensionMajorIterator &iter1,
                    const FirstDimensionMajorIterator &iter2){
                return (iter1.i != iter2.i) ||
                    ((iter1.i != D && iter2.i != D) &&
                    iter1.sub_iter != iter2.sub_iter);
            }
             
            Array<T , Ds...> arr[D];
            static constexpr std::size_t Dim = D;
    };
    
    template <typename T, std::size_t D, std::size_t... Ds>
    Array<T,D,Ds...>::Array(const Array& array){
        for (std::size_t i = 0; i < Dim; i++){
            this->arr[i] = array.arr[i];
        }

    }

    template <typename T, std::size_t D, std::size_t... Ds>
    template <typename U>
    Array<T,D,Ds...>::Array(const Array<U, D, Ds...>& array){
        for (std::size_t i = 0; i < Dim; i++){
            this->arr[i] = array.arr[i];
        }
    }
    
    template <typename T, std::size_t D, std::size_t... Ds>
    Array<T,D,Ds...>& Array<T,D,Ds...>::operator=(const Array& array){
       if (this != &array){
            for (size_t i = 0; i < Dim; i++){
                this->arr[i] = array.arr[i];
            } 
       }
       return *this;
    } 
            
    template <typename T, std::size_t D, std::size_t... Ds>
    template <typename U>
    Array<T,D,Ds...>& Array<T,D,Ds...>::operator=(const Array<U,D, Ds...>& array){
        for (size_t i = 0; i < Dim; i++){
            this->arr[i] = array.arr[i];
        } 
       return *this;
    }

    template <typename T, std::size_t D, std::size_t... Ds>
    auto Array<T,D,Ds...>::operator[](std::size_t ind) -> SubArrayType&{
        if (ind < Dim){
            return arr[ind];
        }else{
            throw OutOfRange{};
        } 
    }

    template <typename T, std::size_t D, std::size_t... Ds>
    auto Array<T,D,Ds...>::operator[](std::size_t ind) const -> const SubArrayType&{
        if (ind < Dim){
            return arr[ind];
        }else{
            throw OutOfRange{};
        } 
    }
   
    template <typename T, std::size_t D, std::size_t... Ds>
    typename Array<T,D,Ds...>::FirstDimensionMajorIterator Array<T,D,Ds...>::fmbegin(){
        return FirstDimensionMajorIterator::MakeBeginIterator(arr);
    }

    template <typename T, std::size_t D, std::size_t... Ds>
    typename Array<T,D,Ds...>::FirstDimensionMajorIterator Array<T,D,Ds...>::fmend(){
        return FirstDimensionMajorIterator::MakeEndIterator(arr);
    }

    template <typename T, std::size_t Base_Dim>
    class Array<T, Base_Dim>{
        public:
            using ValueType = T;
            Array() = default;
            ~Array() = default;
            Array(const Array&);
            template <typename U> Array(const U&);
            Array &operator=(const Array&); 
            template <typename U>Array &operator=(const U&);
            T &operator[](std::size_t ind);
            const T &operator[](std::size_t ind) const;
            class FirstDimensionMajorIterator{
                public:
                    FirstDimensionMajorIterator() = default;
                    FirstDimensionMajorIterator(T * ptr):ptr(ptr){}
                    FirstDimensionMajorIterator(const FirstDimensionMajorIterator &)
                        = default;
                    FirstDimensionMajorIterator &operator=(
                            const FirstDimensionMajorIterator &) = default; 
                    FirstDimensionMajorIterator &operator++(){
                        ++ptr;
                        return *this;
                    }
                    FirstDimensionMajorIterator operator++(int){
                        FirstDimensionMajorIterator it = *this;
                        ++ptr;
                        return it;
                    }
                    T &operator*() const{
                        return *ptr;
                    }
                    T * ptr;

            };
            FirstDimensionMajorIterator fmbegin();
            FirstDimensionMajorIterator fmend();
            friend bool operator==(const FirstDimensionMajorIterator &iter1,
                const FirstDimensionMajorIterator &iter2){
                    return (iter1.ptr == iter2.ptr);
            }

            friend bool operator!=(const FirstDimensionMajorIterator &iter1,
                const FirstDimensionMajorIterator &iter2){
                        return (iter1.ptr != iter2.ptr);
            }
            T arr[Base_Dim];
            static constexpr std::size_t Dim = Base_Dim;
    };

    template <typename T, std::size_t Base_Dim>
    Array<T, Base_Dim>::Array(const Array &array){
        for (std::size_t i = 0; i < Dim; i++){
            this->arr[i] = array.arr[i]; 
        }
    }

    template <typename T, std::size_t Base_Dim>
    template <typename U>
    Array<T, Base_Dim>::Array(const U& array){
        for (std::size_t i = 0; i < Dim; i++){
            this->arr[i] = array.arr[i]; 
        }
    }

    template <typename T, std::size_t Base_Dim>
    Array<T, Base_Dim>& Array<T,Base_Dim>::operator=(const Array& array){
       if (this != &array){
            for (size_t i = 0; i < Dim; i++){
                this->arr[i] = array.arr[i];
            } 
       }
       return *this;
    }

    template <typename T, std::size_t Base_Dim>
    template <typename U>
    Array<T, Base_Dim> &Array<T,Base_Dim>::operator=(const U& array){
        for (size_t i = 0; i < Dim; i++){
            this->arr[i] = array.arr[i];
        } 
       return *this;
    }

    template <typename T, std::size_t Base_Dim>
    T &Array<T,Base_Dim>::operator[](std::size_t ind){
        if (ind < Dim){
            return arr[ind];
        }else{
            throw OutOfRange{};
        } 
    }
    
    template <typename T, std::size_t Base_Dim>
    const T &Array<T, Base_Dim>::operator[](std::size_t ind) const{
        if (ind < Dim){
            return arr[ind];
        }else{
            throw OutOfRange{};
        } 
    }

    template <typename T, std::size_t Base_Dim>
    typename Array<T,Base_Dim>::FirstDimensionMajorIterator Array<T,Base_Dim>::
    fmbegin(){
        return {arr};
    }

    template <typename T, std::size_t Base_Dim>
    typename Array<T,Base_Dim>::FirstDimensionMajorIterator Array<T,Base_Dim>::
    fmend(){
        return {arr + Base_Dim};
    }



}
