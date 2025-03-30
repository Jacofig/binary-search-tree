#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>
#include <time.h>
#include <ctime>
#include <iomanip>
#include <random>
#include <functional>
#include <vector>
#include <stack>

struct someObject
{
private:
    int field_1;
    char field_2;
    bool owned;
public:
    someObject(int f1, char f2) : field_1(f1), field_2(f2), owned(true) {}
    someObject() : field_1(10), field_2('z'), owned(true) {}

    int getField1() const
    {
        return field_1;
    }
    char getField2() const
    {
        return field_2;
    }
    void setField1(int field)
    {
        field_1 = field;
    }
    void setField2(char field)
    {
        field_2 = field;
    }
    ~someObject() {
        if (owned) {}
    }
    void setOwned(bool own) 
    { 
        owned = own; 
    }
    bool getOwned()
    {
        return owned;
    }
    bool operator<(const someObject& temp) const {
        return field_1 < temp.field_1;
    }
    bool operator>(const someObject& temp) const {
        return field_1 > temp.field_1;
    }
};
template <typename T>
struct node
{
    T data;
    node<T>* parent;
    node<T>* leftChild;
    node<T>* rightChild;
    int index;

    node(T data, int index) : data(data), parent(nullptr), leftChild(nullptr), rightChild(nullptr), index(index) {}
};
template <typename T>
struct tree
{
    node<T>* root;
    int size;
    static int index;
    tree() : root(nullptr), size(0) {}
    ~tree()
    {
        clear(root);
    }
    void add(T newData, std::function<bool(const T&, const T&)> comparator)
    {
        node<T>* toAdd = new node<T>(newData, index++);
        if (root == nullptr) {
            root = toAdd;
            size = 1;
            return;
        }

        node<T>* temp = root;
        node<T>* parent = nullptr;
        while (temp != nullptr)
        {
            parent = temp;
            if (comparator(newData, temp->data)) {
                temp = temp->leftChild;
            }
            else {
                temp = temp->rightChild;
            }
        }

        if (comparator(newData, parent->data)) {
            parent->leftChild = toAdd;
            size++;
        }
        else {
            parent->rightChild = toAdd;
            size++;
        }
        toAdd->parent = parent;
    }
    
    node<T>* search(node<T>* root, const T& newData, std::function<bool(const T&, const T&)> anotherComparator)
    {
        if (root == nullptr) {
            return nullptr;
        }
        if constexpr (std::is_arithmetic<T>::value) {
            if (root->data == newData)
            {
                return root;
            }
        }
        else if constexpr (std::is_pointer<T>::value) {
            if (root->data->getField1() == newData->getField1() && root->data->getField2() == newData->getField2()) {
                return root;
            }
        }
        else {
            if (root->data.getField1() == newData.getField1() && root->data.getField2() == newData.getField2()) {
                return root;
            }
        }
       
        if (anotherComparator(newData, root->data))
        {
            return search(root->leftChild, newData, anotherComparator);
        }
        else {
            return search(root->rightChild, newData, anotherComparator);
        }
        
    }
    
    void delet(node<T>*& root, const T& newData, std::function<bool(const T&, const T&)> anotherComparator)
    {
        if (root == nullptr)
        {
            return;
        }

        if (anotherComparator(newData, root->data)) {
            delet(root->leftChild, newData, anotherComparator);
        }
        else if (anotherComparator(root->data, newData)) {
            delet(root->rightChild, newData, anotherComparator);
        }
        else {
            if constexpr (std::is_arithmetic<T>::value) {
                if (root->data == newData) {
                    deletion(root, anotherComparator);
                }
            }
            else if constexpr (std::is_pointer<T>::value) {
                if (root->data->getField1() == newData->getField1() && root->data->getField2() == newData->getField2()) {
                    deletion(root, anotherComparator);
                }
            }
            else {
                if (root->data.getField1() == newData.getField1() && root->data.getField2() == newData.getField2())
                {
                    deletion(root, anotherComparator);
                }
            }
        }
    }
    node<T>* findMin(node<T>* root)
    {
        while (root->leftChild != nullptr) {
            root = root->leftChild;
        }
        return root;
    }
    void deletion(node<T>*& root, std::function<bool(const T&, const T&)> anotherComparator)
    {
        if (root->leftChild == nullptr && root->rightChild == nullptr) {
            delete root;
            root = nullptr;
        }
        else if (root->leftChild == nullptr) {
            node<T>* temp = root;
            root = root->rightChild;
            root->parent = temp->parent;
            delete temp;
        }
        else if (root->rightChild == nullptr) {
            node<T>* temp = root;
            root = root->leftChild;
            root->parent = temp->parent;
            delete temp;
        }
        else {
            node<T>* next = findMin(root->rightChild);
            root->data = next->data;
            delet(root->rightChild, next->data, anotherComparator);
        }
    }

    void inOrder(node<T>* root, std::vector<std::string>& output)
    {
        if (root == nullptr) {
            return;
        }
        
        inOrder(root->leftChild, output);
        if constexpr (std::is_arithmetic<T>::value) {
            output.push_back(std::to_string(root->data));
        }
        else if constexpr (std::is_pointer<T>::value)
        {
            output.push_back(std::to_string(root->data->getField1()) + std::string(" ") + std::string(1, root->data->getField2()));
        }
        else {
            output.push_back(std::to_string(root->data.getField1()) + std::string(" ") + std::string(1, root->data.getField2()));
        }
       
        inOrder(root->rightChild, output);
    }
    
    void clear(node<T>*& root)
    {
        if (root == nullptr)
        {
            return;
        }
        clear(root->leftChild);
        clear(root->rightChild);
        if constexpr (std::is_pointer<T>::value) {
            if (root->data && root->data->getOwned())
            {
                delete root->data;
                root->data = nullptr;
            }
        }
        delete root;
        root = nullptr;
        size = 0;
        index = 0;
    }
    
    void preOrder(node<T>* root, std::vector<std::string>& output)
    {
        if (root == nullptr) {
            return;
        }

        if constexpr (std::is_arithmetic<T>::value) {
            output.push_back(std::to_string(root->data));
        }
        else if constexpr (std::is_pointer<T>::value)
        {
            output.push_back(std::to_string(root->data->getField1()) + std::string(" ") + std::string(1, root->data->getField2()));
        }
        else {
            output.push_back(std::to_string(root->data.getField1()) + std::string(" ") + std::string(1, root->data.getField2()));
        }
        preOrder(root->leftChild, output);
        preOrder(root->rightChild, output);
    }
    
    int determineHeight(node<T>* root)
    {
        if (root == nullptr) {
            return 0;
        }
        int l = determineHeight(root->leftChild);
        int r = determineHeight(root->rightChild);
        return std::max(l, r) + 1;
    }
    
    std::string toString(node<T>* root)
    {

        if (root == nullptr)
        {
            return "";
        }

        std::ostringstream output;
        output << "binary search tree:\nsize: " << size << "\n";

        std::stack<std::pair<node<T>*, int>> stack;
        stack.push({ root, 0 });
        int maxH = determineHeight(root);

        while (!stack.empty()) {
            auto [current, currentH] = stack.top();
            stack.pop();

            bool includeLayer = (currentH < 3) || (currentH >= maxH - 2);
            if (includeLayer) {
                if constexpr (std::is_arithmetic<T>::value) {
                    output << current->index << " value: " << current->data << " [p: " << (current->parent ? current->parent->index : -1) << " l: " << (current->leftChild ? current->leftChild->index : -1) << " r: " << (current->rightChild ? current->rightChild->index : -1) << "]\n";
                }
                else if constexpr (std::is_pointer<T>::value) {
                    output << current->index << " value: " << current->data->getField1() << " " << current->data->getField2() << " [p: " << (current->parent ? current->parent->index : -1) << " l: " << (current->leftChild ? current->leftChild->index : -1) << " r: " << (current->rightChild ? current->rightChild->index : -1) << "]\n";
                }
                else {
                    output << current->index << " value: " << current->data.getField1() << " " << current->data.getField2() << " [p: " << (current->parent ? current->parent->index : -1) << " l: " << (current->leftChild ? current->leftChild->index : -1) << " r: " << (current->rightChild ? current->rightChild->index : -1) << "]\n";
                }
            }

            if (current->rightChild) stack.push({ current->rightChild, currentH + 1 });
            if (current->leftChild) stack.push({ current->leftChild, currentH + 1 });
        }

        return output.str();
    }
};
template <typename T>
int tree<T>::index = 0;

int main()
{
    std::function<bool(someObject*, someObject*)> anotherComparator = [](someObject* obj1, someObject* target)
        {
            return obj1->getField1() < target->getField1();
        };

    std::function<bool(const someObject&, const someObject&)> oanotherComparator = [](const someObject& obj1, const someObject& target)
        {
            return obj1.getField1() < target.getField1();
        };
    srand(time(0));
    const int MAX_ORDER = 7;
    tree<someObject*>* bst = new tree<someObject*>();
    char letters[] = "abcdefghijklmnopqrstuvwxyz";
    for (int o = 1; o <= MAX_ORDER; o++)
    {
        const int n = pow(10, o);
        clock_t t1 = clock();

        for (int i = 0; i < n; i++)
        {
            int x = ((rand()<<15) + rand()) % 1000000000;
            //int xi = (x << 15) * (rand() % 1000000000000);
            char y = letters[rand() % 26];
            someObject* temp = new someObject(x, y);
            bst->add(temp, anotherComparator);
        }
        clock_t t2 = clock();
        double t3 = (t2 - t1) / (double)CLOCKS_PER_SEC;
        std::ostringstream out;
        std::cout << bst->toString(bst->root) << std::endl;
        std::cout << "Current height: " << bst->determineHeight(bst->root) << std::endl;
        std::cout << "Total add time: " << bst->size << " elements: " << t3 << "s" << std::endl;
        //std::cout << std::fixed << std::setprecision(10);
        std::cout << "Average add time " << t3 / n * 1000000 << "us" << std::endl;
        std::cout << "Height to data size ratio" << bst->determineHeight(bst->root) / bst->size << std::endl;
        std::cout << "Log2 of data size: " << log2(bst->size) << std::endl;
        std::cout << "Tree height to data log ratio " << bst->determineHeight(bst->root) / log2(bst->size) << std::endl;
        
        const int m = pow(10, 4);
        int hits = 0;
        t1 = clock();
        for (int i = 0; i < m; i++)
        {
            int x = rand() % (10000) + 1;
            char y = letters[rand() % 26];
            someObject* temp = new someObject(x, y);
            node<someObject*>* result = bst->search(bst->root, temp, anotherComparator);
            if (result != NULL)
            {
                hits++;
            }
            delete temp;
        }
        t2 = clock();
        t3 = (t2 - t1) / (double)CLOCKS_PER_SEC;
        std::cout << "Search time " << m << " elements: " << t3 << "s" << std::endl;
        std::cout << "Hits: " << hits << std::endl;
                std::cout << "////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;

        bst->clear(bst->root);
    }
    delete bst;
    return 0;
}

int main2()
{
    std::function<bool(someObject*, someObject*)> anotherComparator = [](someObject* obj1, someObject* target)
        {
            return obj1->getField1() < target->getField1();
        };

    std::function<bool(const someObject&, const someObject&)> oanotherComparator = [](const someObject& obj1, const someObject& target)
        {
            return obj1.getField1() < target.getField1();
        };

    std::cout << "/////////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
    std::cout << "Int: " << std::endl;
    tree<int> intTree;
    intTree.add(10, [](const int& a, const int& b) { return a < b; });
    intTree.add(5, [](const int& a, const int& b) { return a < b; });
    intTree.add(15, [](const int& a, const int& b) { return a < b; });
    intTree.add(3, [](const int& a, const int& b) { return a < b; });
    intTree.add(7, [](const int& a, const int& b) { return a < b; });

    std::vector<std::string> intInOrder;
    intTree.inOrder(intTree.root, intInOrder);
    std::cout << "In-order: ";
    for (const auto& str : intInOrder) {
        std::cout << str << " ";
    }
    std::cout << std::endl;
    std::vector<std::string> intPreOrder;
    intTree.preOrder(intTree.root, intPreOrder);
    std::cout << "Pre-order: ";
    for (const auto& str : intPreOrder) {
        std::cout << str << " ";
    }
    std::cout << std::endl;

    std::ostringstream intOutput;
    std::cout << "To string: ";
    std::cout << intTree.toString(intTree.root) << std::endl;

    
    node<int>* searchResult = intTree.search(intTree.root, 7, [](const int& a, const int& b) { return a < b; });
    std::cout << "Looking for 7: " << (searchResult ? "Is" : "Isnt") << std::endl;

    intTree.delet(intTree.root, 7, [](const int& a, const int& b) { return a < b; });
    std::cout << "After delete:" << std::endl;
    std::cout << intTree.toString(intTree.root) << std::endl;
    intInOrder.clear();
    intTree.inOrder(intTree.root, intInOrder);
    std::cout << "After clear: " << std::endl;
    intTree.clear(intTree.root);
    std::cout << intTree.toString(intTree.root) << std::endl;
    std::cout << "/////////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
    std::cout << "Objects" << std::endl;
    tree<someObject> objTree;
    objTree.add(someObject(10, 'a'), [](const someObject& a, const someObject& b) { return a.getField1() < b.getField1(); });
    objTree.add(someObject(5, 'b'), [](const someObject& a, const someObject& b) { return a.getField1() < b.getField1(); });
    objTree.add(someObject(15, 'c'), [](const someObject& a, const someObject& b) { return a.getField1() < b.getField1(); });
    objTree.add(someObject(3, 'd'), [](const someObject& a, const someObject& b) { return a.getField1() < b.getField1(); });
    objTree.add(someObject(7, 'e'), [](const someObject& a, const someObject& b) { return a.getField1() < b.getField1(); });

    std::vector<std::string> objInOrder;
    objTree.inOrder(objTree.root, objInOrder);
    std::cout << "In-order: ";
    for (const auto& str : objInOrder) {
        std::cout << str << " ";
    }
    std::cout << std::endl;

    std::vector<std::string> objPreOrder;
    objTree.preOrder(objTree.root, objPreOrder);
    std::cout << "Pre-order: ";
    for (const auto& str : objPreOrder) {
        std::cout << str << " ";
    }
    std::cout << std::endl;

    std::ostringstream objOutput;
    std::cout << "To string: ";
    std::cout << objTree.toString(objTree.root) << std::endl;

    
    someObject searchObj(7, 'e');
    node<someObject>* searchResultObj = objTree.search(objTree.root, searchObj, [](const someObject& a, const someObject& b) { return a.getField1() < b.getField1(); });
    std::cout << "Looking for 7: " << (searchResultObj ? "Is" : "Isnt") << std::endl;

    
    objTree.delet(objTree.root, searchObj, [](const someObject& a, const someObject& b) { return a.getField1() < b.getField1(); });
    std::cout << "After delete:" << std::endl;
    std::cout << objTree.toString(objTree.root) << std::endl;
    objInOrder.clear();
    std::cout << "After clear: " << std::endl;
    objTree.clear(objTree.root);
    std::cout << objTree.toString(objTree.root) << std::endl;
    std::cout << std::endl;
    std::cout << "/////////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
    std::cout << "Pointers" << std::endl;
    std::function<bool(someObject*, someObject*)> comparator = [](someObject* a, someObject* b) {
        return a->getField1() < b->getField1();
        };
    tree<someObject*>* bst = new tree<someObject*>();
    someObject* obj1 = new someObject(10, 'a');
    someObject* obj2 = new someObject(5, 'b');
    someObject* obj3 = new someObject(15, 'c');
    someObject* obj4 = new someObject(3, 'd');
    someObject* obj5 = new someObject(7, 'e');
    bst->add(obj1, anotherComparator);
    bst->add(obj2, anotherComparator);
    bst->add(obj3, anotherComparator);
    bst->add(obj4, anotherComparator);
    bst->add(obj5, anotherComparator);

    std::cout << bst->toString(bst->root) << std::endl;
    std::cout << std::endl;

    std::vector<std::string> ptrInOrder;
    bst->inOrder(bst->root, ptrInOrder);
    std::cout << "In-order: ";
    for (const auto& str : ptrInOrder) {
        std::cout << str << " ";
    }
    std::cout << std::endl;

    std::vector<std::string> ptrPreOrder;
    bst->preOrder(bst->root, ptrPreOrder);
    std::cout << "Pre-order: ";
    for (const auto& str : ptrPreOrder) {
        std::cout << str << " ";
    }
    std::cout << std::endl;

    someObject* temp = new someObject(7, 'e');
    node<someObject*>* result = bst->search(bst->root, temp, anotherComparator);
    std::cout << "Looking for 7" << (result ? "Is" : "Isnt") << std::endl;


    bst->delet(bst->root, temp, anotherComparator);
    std::cout << "After delete: " << std::endl;
    std::cout << bst->toString(bst->root) << std::endl;
    std::cout << std::endl;


    std::cout << "After clear: " << std::endl;
    bst->clear(bst->root);
    std::cout << bst->toString(bst->root) << std::endl;
    std::cout << std::endl;
    return 0;
}

