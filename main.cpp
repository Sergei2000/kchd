#include <iostream>
#include <string>
using std::cout ;
using std::endl ;
#define RIGHT 0
#define LEFT 1

struct node
{
    int _value;
    node* _rnode;
    node* _lnode;
    node* _parent;
    bool _isblack;
};
bool is_left(node* element)
{
    return element->_value<element->_parent->_value;
}
void right_move_up(node* a,node* b)
{
    a->_rnode=b->_lnode;
    if (b->_lnode)
    b->_lnode->_parent=a;
    b->_parent=a->_parent;
    b->_lnode=a;
    a->_parent=b;
    if (!b->_parent) return;
    if (b->_parent->_value < b->_value) b->_parent->_rnode=b;
    else b->_parent->_lnode=b;
}
void left_move_up(node* a, node* b)
{
    a->_lnode=b->_rnode;
    if (b->_rnode)
    b->_rnode->_parent=a;

    b->_parent=a->_parent;
    b->_rnode=a;
    a->_parent=b;
    if (!b->_parent) return;
    if (b->_parent->_value < b->_value) b->_parent->_rnode=b;
    else b->_parent->_lnode=b;
}
void red_uncle_red_dad(node* parent, node*grandad, node* uncle,node** head)//красный дядя красный родитель
{
    parent->_isblack=true;
    uncle->_isblack=true;
    if ( head[0] != grandad )
    {
        grandad->_isblack=false;
    }
}

void parent_and_element_red(node* element,node* parent, node*grandad,node** head)//красный родитель и элемент
{
    if (parent->_lnode == element)
    {
        left_move_up(grandad,parent);
        parent->_isblack=true;
        parent->_rnode->_isblack=false;
        if (!parent->_parent) head[0]=parent;
    }
    else if (parent->_rnode == element)
    {
        right_move_up(grandad,parent);
        parent->_isblack=true;
        parent->_lnode->_isblack=false;
        if (!parent->_parent) head[0]=parent;
    }

}
void grandad_parent_different_sides(node* element, node* parent)//дед и родитель в разных сторонах
{
    if (is_left(element))
    {
        left_move_up(parent,element);
    }
    else {
        right_move_up(parent,element);
    }

}

void work_cases(node** head,node* element)
{
    node* parent = element->_parent;
    if (!parent) return;
    node* grandad = parent->_parent;
    node* uncle=nullptr;
    if (!grandad) return ;

    if (is_left(parent)){uncle=grandad->_rnode;}
    else {uncle=grandad->_lnode;}

    if (!uncle)
    {
        //work cases with black uncle
        if ((is_left(element) && (!is_left(parent)))||((!is_left(element)) && is_left(parent)))
        {
            grandad_parent_different_sides(element,parent);
            work_cases(head,parent);
            return;

        }
        else {
            parent_and_element_red(element,parent,grandad,head);
            return;
        }

    }
    else if (!uncle->_isblack)
    {
        red_uncle_red_dad(parent,grandad,uncle,head);
        work_cases(head,grandad);
        return;
    }
    else if (uncle->_isblack)
    {
        if ((is_left(element) && (!is_left(parent)))||((!is_left(element)) && is_left(parent)))
        {
            grandad_parent_different_sides(element,parent);
            work_cases(head,parent);
            return;

        }
        else {
            parent_and_element_red(element,parent,grandad,head);
            return;
        }
    }


}

void balance_tree(node** head,node* element)
{
   work_cases(head,element);
}
void insert (node** head,int value)
{
    node* element= new node;
    element->_value=value;
    element->_lnode=nullptr;
    element->_rnode=nullptr;
    element->_isblack=false;
    element->_parent=nullptr;
    if (!head[0])
    {
        head[0] = element;
        head[0]->_isblack = true;
        return;
    }
    else
    {
        node* tmp=head[0];
        while (tmp)
        {
            if ( value < tmp->_value)
            {
                if (!tmp->_lnode)
                {
                    tmp->_lnode=element;
                    element->_parent=tmp;
                    break;
                }
                else
                {
                    tmp=tmp->_lnode;
                }
            }
            else if (value > tmp->_value)
            {
                if (!tmp->_rnode)
                {
                    tmp->_rnode=element;
                    element->_parent=tmp;
                    break;
                }
                else
                {
                    tmp=tmp->_rnode;
                }
            }
            else if (value == tmp->_value)
            {
                delete element;
                element=nullptr;
                return;
            }
        }
    }

    if (!element->_parent->_isblack)
        balance_tree(head,element);

    element=nullptr;
}

void print(node* root,int counter)
{
    node* tmp=root;
    node* v4;
    int v3,v5;
    if (root)
    {
        v3=counter;
        while(true)
        {
            v4=tmp->_rnode;
            if(v4) print(v4,counter+4);
            if (v3>0)
            {
                v5=v3;
                do
                {
                    cout<<" ";
                    --v5;
                }while(v5);
            }
            tmp->_isblack?cout<<tmp->_value<<endl:cout<<"<"<<tmp->_value<<">"<<endl;
            tmp=tmp->_lnode;
            if (!tmp) break;
            v3+=4;
        }

    }
}



node* get_only_child(node* element)
{
    if (element->_lnode) return element->_lnode;
    if (element->_rnode) return element->_rnode;
    return nullptr;
}

node* get_brother(node* element)
{
    if(is_left(element))return element->_parent->_rnode;
    else return element->_parent->_lnode;

}

void big_left_turn(node* a,node* b)
{
    node* c=b->_rnode;
    b->_rnode=c->_lnode;
    if (b->_rnode)
    b->_rnode->_parent=b;

    a->_lnode= c->_rnode;
    if (a->_lnode)
    a->_lnode->_parent=a;

    c->_parent=a->_parent;
    c->_lnode=b;
    c->_lnode->_parent=c;
    c->_rnode=a;
    c->_rnode->_parent=c;
    if (!c->_parent) return;
    if (is_left(c))c->_parent->_lnode=c;
    else c->_parent->_rnode=c;
}
void big_right_turn(node* a, node* b)
{
    node* c = b->_lnode;
    b->_lnode=c->_rnode;
    if (b->_lnode)
    b->_lnode->_parent=b;

    a->_rnode= c->_lnode;
    if (a->_rnode)
    a->_rnode->_parent=a;

    c->_parent=a->_parent;
    c->_rnode=b;
    c->_rnode->_parent=c;
    c->_lnode=a;
    c->_lnode->_parent=c;
    if (!c->_parent) return;
    if (is_left(c))c->_parent->_lnode=c;
    else c->_parent->_rnode=c;
}

void balance_element_red(node* element,node* child,int half)
{
    if (half == RIGHT)
    {
        if (!child->_lnode && !child->_rnode)
        {
            child->_isblack=false;
            element->_isblack=true;
            return;
        }
        if (child->_lnode)
        {
            if (!child->_lnode->_isblack)
            {
                left_move_up(element,child);
                child->_lnode->_isblack=true;
                element->_isblack=false;
                return;
            }
            if (child->_rnode->_isblack)
            {
                child->_isblack=false;
                element->_isblack=true;
                return;
            }
        }
    }
    else
    {
        if (!child->_lnode && !child->_rnode)
        {
            child->_isblack=false;
            element->_isblack=true;
            return;
        }
        if (child->_rnode)
        {
            if (!child->_rnode->_isblack)
            {
                right_move_up(element,child);
                child->_rnode->_isblack=true;
                element->_isblack=false;
                return;
            }
            if (child->_lnode->_isblack)
            {
                child->_isblack=false;
                element->_isblack=true;
                return;
            }
        }

    }
}
void balance_after_delete(node* element,int half)//балансировка после удаления
{
    node* child=nullptr;

    if (half == RIGHT)
    {
        child=element->_lnode;
    }
    else {
        child=element->_rnode;
    }
    if (!element->_isblack)
    {
        balance_element_red(element,child,half);
        return;
    }
    else
    {
        if (!child->_isblack)
        {
            if (half==RIGHT)
            {
                if (!child->_rnode->_lnode && !child->_rnode->_rnode)
                {
                    left_move_up(element,child);
                    child->_isblack=true;
                    element->_lnode->_isblack=false;
                    return;
                }
                if (child->_rnode->_lnode)
                {
                    if (!child->_rnode->_lnode->_isblack)
                    {
                        big_left_turn(element,child);
                        child->_rnode->_isblack=true;
                        return;
                    }
                    else if (child->_rnode->_rnode->_isblack)
                    {
                        left_move_up(element,child);
                        child->_isblack=true;
                        element->_lnode->_isblack=false;
                        return;
                    }
                }
            }
            else
            {
                if (!child->_lnode->_lnode && !child->_lnode->_rnode)
                {
                    right_move_up(element,child);
                    child->_isblack=true;
                    element->_rnode->_isblack=false;
                    return;
                }
                if (child->_lnode->_rnode)
                {
                    if (!child->_lnode->_rnode->_isblack)
                    {
                        big_right_turn(element,child);
                        child->_rnode->_isblack=true;
                        return;
                    }
                    else if (child->_lnode->_lnode->_isblack)
                    {
                        right_move_up(element,child);
                        child->_isblack=true;
                        element->_rnode->_isblack=false;
                        return;
                    }
                }
            }
        }
        else
        {
            if (half==RIGHT)
            {
                if (!child->_rnode && !child->_lnode)
                {
                    child->_isblack=false;
                    if (element->_parent)
                    {
                        if (element->_parent->_value<element->_value) half=RIGHT;
                        else half=LEFT;
                        balance_after_delete(element->_parent,half);
                    }
                    return;
                }
                if (child->_rnode)
                {
                    if (!child->_rnode->_isblack)
                    {
                        big_left_turn(element,child);
                        child->_parent->_isblack=true;
                        return;
                    }
                    if (child->_rnode->_isblack && child->_lnode->_isblack)
                    {
                        child->_isblack=false;
                        if (element->_parent)
                        {
                            if (element->_parent->_value<element->_value) half=RIGHT;
                            else half=LEFT;
                            balance_after_delete(element->_parent,half);
                        }
                        return;
                    }
                }
            }
            else
            {
                if (!child->_rnode && !child->_lnode)
                {
                    child->_isblack=false;
                    if (element->_parent)
                    {
                        if (element->_parent->_value<element->_value) half=RIGHT;
                        else half=LEFT;
                        balance_after_delete(element->_parent,half);
                    }
                    return;
                }
                if (child->_lnode)
                {
                    if (!child->_lnode->_isblack)
                    {
                        big_right_turn(element,child);child->_parent->_isblack=true;
                        return;
                    }
                    if (child->_rnode->_isblack && child->_lnode->_isblack)
                    {
                        child->_isblack=false;
                        if (element->_parent)
                        {
                            if (element->_parent->_value<element->_value) half=RIGHT;
                            else half=LEFT;
                            balance_after_delete(element->_parent,half);
                        }
                        return;
                    }
                }
            }

        }
    }

}

void delete_element(node* head,int value)
{
    node* tmp=head;
    while(tmp->_value != value)
    {
        if (!tmp){cout<<"no such element\n";return;}
        if (tmp->_value < value) tmp=tmp->_rnode;
        else if (tmp->_value > value)tmp=tmp->_lnode;
    }

    if (tmp->_lnode && tmp->_rnode) {
        node* next_key=tmp->_rnode;
        while(next_key->_lnode)
            next_key=next_key->_lnode;
        std::swap(next_key->_value,tmp->_value);
        tmp=next_key;

    }


    if ((tmp->_lnode && !tmp->_rnode)||(!tmp->_lnode && tmp->_rnode))
    {
        node* child=get_only_child(tmp);
        child->_parent=tmp->_parent;
        if (child->_parent->_value>child->_value) child->_parent->_lnode=child;
        else child->_parent->_rnode=child;
        child->_isblack=true;
    }
    else if (!tmp->_lnode &&!tmp->_rnode)
    {
        if (!tmp->_isblack)
        {
            // simply delete red element
            if (is_left(tmp)) tmp->_parent->_lnode=nullptr;
            else tmp->_parent->_rnode=nullptr;
        }
        else
        {
            //work delete cases and balancing
            if (is_left(tmp))tmp->_parent->_lnode=nullptr;
            else tmp->_parent->_rnode=nullptr;
            int half;//какую сторону нужно балансировать
            if (tmp->_parent->_value<tmp->_value) half= RIGHT;
            else half =LEFT;

            balance_after_delete(tmp->_parent,half);

        }
    }
    tmp->_lnode=nullptr;
    tmp->_parent=nullptr;
    tmp->_rnode=nullptr;
    delete tmp;
    tmp=nullptr;

}


int main()
{
    node** root=new node*;

    insert(root,20);
    insert(root,10);
    insert(root,25);
    insert(root,4);
    insert(root,30);
    insert(root,16);
    insert(root,2);
    insert(root,5);
    insert(root,3);
    insert(root,14);
    insert(root,17);
    insert(root,12);
    insert(root,15);
    insert(root,19);

    insert(root,10);
    insert(root,23);
    insert(root,25);

   // print(root[0],0);
    delete_element(root[0],5);
    print(root[0],0);

    return 0;
}
