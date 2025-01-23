#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next);
        Type value;
        Node* next_node = nullptr;
    };
    
    // Шаблон класса «Базовый Итератор».
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node);

    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept;

        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept;

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept;

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept;

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept;

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept;

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept;

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept;

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept;

    private:
        Node* node_ = nullptr;
    };


public:
    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    SingleLinkedList() {}
    SingleLinkedList(std::initializer_list<Type> values);
    SingleLinkedList(const SingleLinkedList& other);

    ~SingleLinkedList();

    // Возвращает количество элементов в списке
    [[nodiscard]] size_t GetSize() const noexcept;

    // Сообщает, пустой ли список
    [[nodiscard]] bool IsEmpty() const noexcept;

    // Очищает список от элементов и устанавливает его размер на 0
    void Clear() noexcept;

    // Добавляет элемент в начало списка
    void PushFront(const Type& value);
    
    // Удаляет элемент в начале списка
    void PopFront() noexcept;

    // Вставляет элемент value после элемента, на который указывает pos.
    // Возвращает итератор на вставленный элемент
    // Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии      
    Iterator InsertAfter(ConstIterator pos, const Type& value);

     // Удаляет элемент, следующий за pos.
     // Возвращает итератор на элемент, следующий за удалённым
    Iterator EraseAfter(ConstIterator pos) noexcept;
    
    // Обменивает значения с другим списком
    void Swap(SingleLinkedList& other) noexcept;

    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept;

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept;

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept;

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept;

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept;

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept;

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept;

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept;

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept;
    
    SingleLinkedList& operator=(const SingleLinkedList& rhs);

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void Swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.Swap(rhs);
}

template <typename Type>
inline bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
inline bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}

// -----------------------Node-----------------------

template <typename Type>
SingleLinkedList<Type>::Node::Node(const Type& val, Node* next) : value(val), next_node(next) {}

// -------------------BasicIterator------------------

template <typename Type>
template <typename ValueType>
SingleLinkedList<Type>::BasicIterator<ValueType>::BasicIterator(Node* node) : node_(node) {}

template <typename Type>
template <typename ValueType>
SingleLinkedList<Type>::BasicIterator<ValueType>::BasicIterator(const BasicIterator<Type>& other) noexcept {
    node_ = other.node_;
}

template <typename Type>
template <typename ValueType>
bool SingleLinkedList<Type>::BasicIterator<ValueType>::operator==(const BasicIterator<const Type>& rhs) const noexcept {
    return this->node_ == rhs.node_;
}

template <typename Type>
template <typename ValueType>
bool SingleLinkedList<Type>::BasicIterator<ValueType>::operator!=(const BasicIterator<const Type>& rhs) const noexcept {
    return this->node_ != rhs.node_;
}

template <typename Type>
template <typename ValueType>
bool SingleLinkedList<Type>::BasicIterator<ValueType>::operator==(const BasicIterator<Type>& rhs) const noexcept {
    return this->node_ == rhs.node_;
}

template <typename Type>
template <typename ValueType>
bool SingleLinkedList<Type>::BasicIterator<ValueType>::operator!=(const BasicIterator<Type>& rhs) const noexcept {
    return this->node_ != rhs.node_;
}

template <typename Type>
template <typename ValueType>
SingleLinkedList<Type>::BasicIterator<ValueType>& SingleLinkedList<Type>::BasicIterator<ValueType>::operator++() noexcept {
    assert(node_ != nullptr);
    node_ = node_->next_node;
    return *this;
}

template <typename Type>
template <typename ValueType>
SingleLinkedList<Type>::BasicIterator<ValueType> SingleLinkedList<Type>::BasicIterator<ValueType>::operator++(int) noexcept {
    assert(node_ != nullptr);
    auto temp(*this);
    node_ = node_->next_node;
    return temp;
}

template <typename Type>
template <typename ValueType>
typename SingleLinkedList<Type>::BasicIterator<ValueType>::reference SingleLinkedList<Type>::BasicIterator<ValueType>::operator*() const noexcept {
    assert(node_ != nullptr);
    return node_->value;
}

template <typename Type>
template <typename ValueType>
typename SingleLinkedList<Type>::BasicIterator<ValueType>::pointer SingleLinkedList<Type>::BasicIterator<ValueType>::operator->() const noexcept {
    assert(node_ != nullptr);
    return &node_->value;
}

// ------------------SingleLinkedList----------------

template <typename Type>
SingleLinkedList<Type>::SingleLinkedList(std::initializer_list<Type> values) {
    for (auto it = rbegin(values); it < rend(values); ++it) {
        PushFront(*it);
    }
}

template <typename Type>
SingleLinkedList<Type>::SingleLinkedList(const SingleLinkedList& other) {
    assert(size_ == 0 && head_.next_node == nullptr);
    SingleLinkedList temp;
    auto temp_it = temp.cbefore_begin();

    for (const auto& value : other) {
        temp_it = InsertAfter(temp_it, value);
    }

    temp.size_ = other.size_;
    Swap(temp);
}

template <typename Type>
SingleLinkedList<Type>::~SingleLinkedList() {
    Clear();
}

template <typename Type>
size_t SingleLinkedList<Type>::GetSize() const noexcept {
    return size_;
}

template <typename Type>
bool SingleLinkedList<Type>::IsEmpty() const noexcept {
    return (size_ == 0 ? true : false);
}

template <typename Type>
void SingleLinkedList<Type>::Clear() noexcept {
    while (head_.next_node != nullptr) {
        auto* temp = (head_.next_node)->next_node;
        delete head_.next_node;
        head_.next_node = temp;
    }
    size_ = 0;
}

template <typename Type>
void SingleLinkedList<Type>::PushFront(const Type& value) {
    head_.next_node = new Node(value, head_.next_node);
    ++size_;
}

template <typename Type>
void SingleLinkedList<Type>::PopFront() noexcept {
    assert(!IsEmpty());
    auto* temp = (head_.next_node)->next_node;
    delete head_.next_node;
    head_.next_node = temp;
    --size_;
}

template <typename Type>
typename SingleLinkedList<Type>::Iterator SingleLinkedList<Type>::InsertAfter(ConstIterator pos, const Type& value) {
    assert(pos.node_ != nullptr);
    pos.node_->next_node = new Node(value, pos.node_->next_node);
    ++size_;

    return Iterator{ pos.node_->next_node };
}

template <typename Type>
typename SingleLinkedList<Type>::Iterator SingleLinkedList<Type>::EraseAfter(ConstIterator pos) noexcept {
    assert(pos.node_ != nullptr);
    auto* temp = pos.node_->next_node->next_node;
    delete pos.node_->next_node;
    pos.node_->next_node = temp;
    --size_;

    return Iterator{ pos.node_->next_node };
}

template <typename Type>
void SingleLinkedList<Type>::Swap(SingleLinkedList& other) noexcept {
    std::swap(other.head_.next_node, head_.next_node);
    std::swap(other.size_, size_);
}

template <typename Type>
typename SingleLinkedList<Type>::Iterator SingleLinkedList<Type>::before_begin() noexcept {
    return SingleLinkedList<Type>::Iterator{ &head_ };
}

template <typename Type>
typename SingleLinkedList<Type>::ConstIterator SingleLinkedList<Type>::before_begin() const noexcept {
    return cbefore_begin();
}

template <typename Type>
typename SingleLinkedList<Type>::ConstIterator SingleLinkedList<Type>::cbefore_begin() const noexcept {
    return SingleLinkedList<Type>::ConstIterator{ const_cast<Node*>(&head_) };
}

template <typename Type>
typename SingleLinkedList<Type>::Iterator SingleLinkedList<Type>::begin() noexcept {
    return SingleLinkedList<Type>::Iterator{ head_.next_node };
}

template <typename Type>
typename SingleLinkedList<Type>::Iterator SingleLinkedList<Type>::end() noexcept {
    return SingleLinkedList<Type>::Iterator{ nullptr };
}

template <typename Type>
typename SingleLinkedList<Type>::ConstIterator SingleLinkedList<Type>::begin() const noexcept {
    return cbegin();
}

template <typename Type>
typename SingleLinkedList<Type>::ConstIterator SingleLinkedList<Type>::end() const noexcept {
    return cend();
}

template <typename Type>
typename SingleLinkedList<Type>::ConstIterator SingleLinkedList<Type>::cbegin() const noexcept {
    return SingleLinkedList<Type>::ConstIterator{ head_.next_node };
}

template <typename Type>
typename SingleLinkedList<Type>::ConstIterator SingleLinkedList<Type>::cend() const noexcept {
    return SingleLinkedList<Type>::ConstIterator{ nullptr };
}

template<typename Type>
SingleLinkedList<Type>& SingleLinkedList<Type>::operator=(const SingleLinkedList& rhs) {
    if (this != &rhs) {
        SingleLinkedList temp(rhs);
        Swap(temp);
    }
    return *this;
}