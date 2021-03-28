typedef enum{RED=0,BLACK=1}Color;
template<typename type>
struct RBTNode{
        Color color;
        type key;
        RBTNode* left;
        RBTNode* right;
        RBTNode* parent;
};

