//Matthew Atmadja
//matmadja
//PA5

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"Dictionary.h"

// Exported type --------------------------------------------------------------

typedef struct NodeObj{
   KEY_TYPE key;
   VAL_TYPE value;
   struct NodeObj* parent;
   struct NodeObj* left;
   struct NodeObj* right;
} NodeObj;

typedef struct NodeObj* Node;

typedef struct DictionaryObj{
    Node nil;
    Node root;
    Node cursor;
    int uniqueCheck;
    int size;
}DictionaryObj;

typedef struct DictionaryObj* Dictionary;

// Constructors-Destructors ---------------------------------------------------

Node newNode(KEY_TYPE k, VAL_TYPE v) {
   Node N = malloc(sizeof(NodeObj));
   N->key = k;
   N->value = v;
   N->parent = NULL;
   N->left = NULL;
   N->right = NULL;
   return N;
}

// newDictionary()
// Creates a new empty Dictionary. If unique==false (0), then the Dictionary 
// will accept duplicate keys, i.e. distinct pairs with identical keys. If 
// unique==true (1 or any non-zero value), then duplicate keys will not be 
// accepted. In this case, the operation insert(D, k) will enforce the 
// precondition: lookup(D, k)==VAL_UNDEF
Dictionary newDictionary(int unique){
    Dictionary D = malloc(sizeof(DictionaryObj));
    D->nil = newNode(KEY_UNDEF, VAL_UNDEF);

    D->nil->left = D->nil;
    D->nil->right = D->nil;

    D->root = D->nil;
    D->cursor = D->nil;

    if(unique > 0){
        D->uniqueCheck = 1;
    }else{
        D->uniqueCheck = 0;
    }
    
    D->size = 0;
    return D;
}

void freeNode(Node* pN){
   if( pN!=NULL && *pN!=NULL ){
      free(*pN);
      *pN = NULL;
   }
}

//freeDictionary()
// Frees heap memory associated with *pD, sets *pD to NULL.
void freeDictionary(Dictionary* pD){
    if(pD!=NULL && *pD!=NULL){
        makeEmpty(*pD);
        
        freeNode(&((*pD)->nil));
        (*pD)->nil = NULL;

        free(*pD);
        *pD = NULL;
    }
}

// Access functions -----------------------------------------------------------

Node treeMin(Dictionary D, Node n){
    Node temp = n;

    while(temp->left != D->nil){
        temp = temp->left;
    }
    return temp;
}

Node treeMax(Dictionary D, Node n){
     Node temp = n;

    while(temp->right != D->nil){
        temp = temp->right;
    }
    return temp;
}

// size()
// Returns the number of (key, value) pairs in Dictionary D.
int size(Dictionary D){
    if(D == NULL){
        fprintf(stderr, "Dictionary Error: Calling size() on NULL Dictionary reference\n");
        exit(1);
    }

    return D->size;
}

// getUnique()
// Returns true (1) if D requires that all pairs have unique keys. Returns
// false (0) if D accepts distinct pairs with identical keys.
int getUnique(Dictionary D){
    if(D == NULL){
        fprintf(stderr, "Dictionary Error: Calling getUnique() on NULL Dictionary reference\n");
        exit(1);
    }

    return D->uniqueCheck;
}

VAL_TYPE treeSearch(Dictionary D, Node x, KEY_TYPE k){
    if(x == D->nil || (KEY_CMP(k, x->key) == 0)){
        return x->value;
    } else if(KEY_CMP(k, x->key) < 0){
        treeSearch(D, x->left, k);
    } else{
        treeSearch(D, x->right, k);
    }
}

// lookup()
// If Dictionary D contains a (key, value) pair whose key matches k (i.e. if
// KEY_CMP(key, k)==0), then returns value. If D contains no such pair, then
// returns VAL_UNDEF.
VAL_TYPE lookup(Dictionary D, KEY_TYPE k){
    if(D == NULL){
        fprintf(stderr, "Dictionary Error: Calling lookup() on NULL Dictionary reference\n");
        exit(1);
    }

    return treeSearch(D, D->root, k);
}


// Manipulation procedures ----------------------------------------------------

// insert()
// Insert the pair (k,v) into Dictionary D. 
// If getUnique(D) is false (0), then there are no preconditions.
// If getUnique(D) is true (1), then the precondition lookup(D, k)==VAL_UNDEF
// is enforced. 
void insert(Dictionary D, KEY_TYPE k, VAL_TYPE v){
    if(D == NULL){
        fprintf(stderr, "Dictionary Error: Calling insert() on NULL Dictionary reference\n");
        exit(1);
    }

    if(getUnique(D)){
        if(lookup(D,k) != VAL_UNDEF){
            fprintf(stderr, "Dictionary Error: Calling insert() on duplicate key\n");
            exit(1);
        }
    }

    Node temp = newNode(k ,v);

    temp->left = D->nil;
    temp->right = D->nil;

    Node x = D->root;
    Node y = D->nil;

    while(x != D->nil){
        y = x;
        if(KEY_CMP(temp->key, x->key) < 0){
            x = x->left;
        }else{
            x = x->right;
        }
    }

    temp->parent = y;
    if(y == D->nil){
        D->root = temp;
    }else if(KEY_CMP(temp->key, y->key) < 0){
        y->left = temp;
    }else{
        y->right = temp;
    }

    D->size++;

}

void transplant(Dictionary D, Node u, Node v){
    if(u->parent == D->nil){
        D->root = v;
    }else if(u == u->parent->left){
        u->parent->left = v;
    }else{
        u->parent->right = v;
    }

    if(v != D->nil){
        v->parent = u->parent;
    }
}

//delete()
// Remove the pair whose key is k from Dictionary
// Pre: lookup(D,k)!=VAL_UNDEF (i.e. D contains a pair whose key is k.)
void delete(Dictionary D, KEY_TYPE k){
    if(D == NULL){
        fprintf(stderr, "Dictionary Error: Calling delete() on NULL Dictionary reference\n");
        exit(1);
    }

    if(lookup(D, k) == VAL_UNDEF){
        fprintf(stderr, "Dictionary Error: Trying to delete non-existent key\n");
        exit(1);
    }

    Node tempC = NULL;
    int c = 0;

    if(currentKey(D) != k){
        tempC = D->cursor;
        c = 1;
    }

    int temp = beginForward(D);
    
    while(currentKey(D) != k){
        temp = next(D);
    }
    
    if(D->cursor->left == D->nil){
        transplant(D, D->cursor, D->cursor->right);
    }else if(D->cursor->right == D->nil){
        transplant(D, D->cursor, D->cursor->left);
    }else{
        Node y = treeMin(D, D->cursor->right);
        if(y->parent != D->cursor){
            transplant(D, y, y->right);
            y->right = D->cursor->right;
            y->right->parent = y;
        }

        transplant(D, D->cursor, y);
        y->left = D->cursor->left;
        y->left->parent = y;
    }
    
    freeNode(&(D->cursor));
    if(c){
        D->cursor = tempC;
    }else{
        D->cursor = D->nil;
    }
    D->size--;

}

// makeEmpty()
// Reset Dictionary D to the empty state, containing no pairs.
void makeEmpty(Dictionary D){
    if(D == NULL){
        fprintf(stderr, "Dictionary Error: Calling makeEmpty() on NULL Dictionary reference");        
        exit(1);
    }

    VAL_TYPE holder = beginForward(D);
    KEY_TYPE k = currentKey(D);

    while(size(D) > 0){
        delete(D, k);
        holder = beginForward(D);
        k = currentKey(D);
    }

    D->size = 0;
    D->root = D->nil;
    D->cursor = D->nil;
}

// beginForward() (similar to moveFront)
// If D is non-empty, starts a forward iteration over D at the first key 
// (as defined by the order operator KEY_CMP()), then returns the first
// value. If D is empty, returns VAL_UNDEF. 
VAL_TYPE beginForward(Dictionary D){
    D->cursor = treeMin(D, D->root);

    return currentVal(D);
}

// beginReverse() (similar to moveBack)
// If D is non-empty, starts a reverse iteration over D at the last key 
// (as defined by the order operator KEY_CMP()), then returns the last
// value. If D is empty, returns VAL_UNDEF.
VAL_TYPE beginReverse(Dictionary D){
    D->cursor = treeMax(D, D->root);

    return currentVal(D);
}

// currentKey() (similar to get)
// If an iteration (forward or reverse) over D has started, returns the 
// the current key. If no iteration is underway, returns KEY_UNDEF.
KEY_TYPE currentKey(Dictionary D){
    if(D == NULL){
        fprintf(stderr, "Dictionary Error: Calling currentKey() on NULL Dictionary reference\n");
        exit(1);
    }

    return D->cursor->key;
}

// currentVal() (similar to get)-
// If an iteration (forward or reverse) over D has started, returns the 
// value corresponding to the current key. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE currentVal(Dictionary D){
    if(D == NULL){
        fprintf(stderr, "Dictionary Error: Calling currentVal() on NULL Dictionary reference\n");
        exit(1);
    }

    return D->cursor->value;
}

Node treeSucc(Dictionary D, Node n){
    if(n->right != D->nil){
        return treeMin(D, n->right);
    }

    Node y = n->parent;
    Node temp = n;
    while(y != D->nil && temp == y->right){
        temp = y;
        y = y->parent;
    }

    return y;
}

// next() (similar to moveNext)
// If an iteration (forward or reverse) over D has started, and has not
// reached the last pair, moves to the next key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the last pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE next(Dictionary D){
    if(D == NULL){
        fprintf(stderr, "Dictionary Error: Calling next() on NULL Dictionary reference\n");
        exit(1);
    }

    D->cursor = treeSucc(D, D->cursor);
    return currentVal(D);
}

Node treePred(Dictionary D, Node n){
    if(n->left != D->nil){
        return treeMax(D, n->left);
    }

    Node y = n->parent;
    Node temp = n;

    while(y != D->nil && temp == y->left){
        temp = y;
        y = y->parent;
    }
    return y;
}

// prev() (similar to movePrev)
// If an iteration (forward or reverse) over D has started, and has not
// reached the first pair, moves to the previous key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the first pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF. 
VAL_TYPE prev(Dictionary D){
    if(D == NULL){
        fprintf(stderr, "Dictionary Error: Calling prev() on NULL Dictionary reference\n");
        exit(1);
    }

    D->cursor = treePred(D, D->cursor);
    return currentVal(D);
}


// Other operations -----------------------------------------------------------

// printDictionary()
// Prints a text representation of D to the file pointed to by out. Each key-
// value pair is printed on a single line, with the two items separated by a
// single space.  The pairs are printed in the order defined by the operator
// KEY_CMP().
void printDictionary(FILE* out, Dictionary D){
    if(D == NULL){
        fprintf(stderr, "Dictionary Error: Calling printDictionary() on NULL reference\n");
        exit(1);
    }

    VAL_TYPE val = beginForward(D);
    KEY_TYPE key = currentKey(D);
    fprintf(out, KEY_FORMAT "\n", key);

    val = next(D);
    key = currentKey(D);
    while(D->cursor != D->nil){
        fprintf(out, KEY_FORMAT"\n", key);

        val = next(D);
        key = currentKey(D);
    }
}
