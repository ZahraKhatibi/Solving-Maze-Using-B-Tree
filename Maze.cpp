#include <iostream>
using namespace std;

class Btree_node         // dar 'Btree_node', 3 data va 4 pointer tarif shode ast.
{                        // dar asl, yek khane ezafi baraye data va ye pointer azafi dar nazar gerefte shode ast ta 
    public:              // dar hengam dinsert va ghabl az split kardan az on estefade shavad //
        int data1 = -1;
        int data2 = -1;
        int data3 = -1;
        Btree_node *left = 0, *right = 0 , *mid = 0 , *extra = 0 , *father = 0;
};

class matrix_node               // dar 'matrix_node', 2 meghdar shamel vazn har deraye matrix va vazn minimum
{                               // ke az mabda ta on deraye hazine mishavad vojod darad.
    public:                         // default 'min_weight' yek adad bozorg dar nazar gerefte shode ast //                                      
        int weight = -1 ;
        int min_weight = 987654321;
        matrix_node *father = 0;
};

class heap_node                                 // dar 'heap_node' 2 motagheyer 'id' va 'min_wieght_mat' vojod darad.
{                                               // 'id' be ma mokhtasat deraye mored nazar az matrix ra neshan midahad.
    public:                                     // 'min_weight_mat' shamel adadi ast ke neshan midahad az mabda ta on deraye 
        heap_node(int d , int min_weight)       // hazine mishavad vojod darad //
        {                                       
            id = d;
            min_weight_mat = min_weight;
        }
        int id;
        int min_weight_mat;
};

class heap         // in class shamel chandin function ast va 'min_heap' ra baraye ma tashkil midahad //
{
    public:
        heap(int n)
        {
            que = new heap_node *[n] ;
        }
        bool compare(heap_node*,heap_node*);   // dar 'compare' 2 'heap_node' bar asas 'id' va 'min_weight' moghayese mishavand //
        void insert(int,int);                  // meghdar jadidi be 'heap' insert mishavad //
        void check(int);                       // pas az inser, baresi mishavad ke adad jadid dar makan monaseb khodash gharar girad //
        void check2(int);                      // pas az hazf rishe, baresi mishavad ke 'heap' mojadad  be dorosti moratab shavad //
        void update_weight(int, int);          // dar inja agar 'min_weight' kamtari baraye derayeii peyda shod, meghdarash update mishavad //
        int find_id(int);                      // baresi mishavad 'id' mored nazar, az ghabl dar 'heap' vojod darad ya kheyr //
        heap_node* delete_root();              // rishe 'heap' ra delete mikonad //

    private:
        int temp = 0;           // tedad adad haye daron heap ra neshan midahad //
        heap_node **que;        // arayeii ke heap ra tashkil midahad //
};

class b_tree        // in class shamel chandin function ast va 'b_tree' ra baraye ma tashkil midahad // 
{
    public:
        Btree_node *root = 0;  
        void insert (Btree_node*,int);     // meghdar jadidi dar derakht insert mishavad // 
        void split (Btree_node*);                          // dar hengami ke key = 3 shavad, split seda zade mishavad ta derakht moratab shavad //
        void dijkstra (matrix_node**,int,int,int,int,int,b_tree,heap);            // baraye peymayesh az algorithm 'dijkstra' estefade mishavad //
        void print_mat (matrix_node**,int,int,int);     // khoroji ra baraye ma chap mikonad //
        bool find (int,Btree_node*);   // baresi mikonad 'id' mored nazar be onvan r`as bazdid shode dar 'b_tree' gharar darad ya kheyr //     
        Btree_node *find_place (Btree_node* , int);     // zamani ke gharar ast adad jadidi insert shavad, bargi ke bayad dar on insert shavad ra peyda mikonad //
};

main()
{
    int n, m, x , y1 , y2;
    cin >> n >> m;
    matrix_node ** matrix = new matrix_node *[n];

    for(int i=0 ; i<n ; i++)
        matrix[i] = new matrix_node [m];
    
    for(int i=0 ; i<n ; i++)
    {
        for(int j=0 ; j<m ; j++)
        {
            cin >> x;
            if(i==0 && x!=0)
                y1 = j;        // soton mabda` ra save mikonad // 

            if(i==n-1 && x!=0)
                y2 = j;         // soton maghsad ra save mikonad // 

            matrix[i][j].weight = x;
        }
    }

    b_tree tree;
    heap min_heap(n*m);
    tree.dijkstra( matrix,0,y1,y2,n,m,tree,min_heap);
}

void b_tree :: insert (Btree_node *root1, int x)
{
    if (root1==0)       
    {
        root = new Btree_node;
        root->data1 = x;
    }

    else            
    {
        root1 = find_place(root1 , x);   // barg monaseb ra baraye insert peyda mikonad // 

        if(root1->data2 == -1)          // agar dar on barg faghat 1 data vojod dashte bashad... //
        {
            if(x>root1->data1)         // be tatib so`odi data1,data2 va data3 ra por mikonim // 
                root1->data2 = x;

            else
            {
                int p = root1->data1;
                root1->data1 = x;
                root1->data2 = p;
            }
        }
        else                        // agar dar on barg 2 data vojod dashte bashad...//
        {
            if(x < root1->data1 )
            {
                root1->data3 = root1->data2;
                root1->data2 = root1->data1;
                root1->data1 = x;
            }

            else if(x>root1->data1 && x<root1->data2)
            {
                root1->data3 = root1->data2;
                root1->data2 = x;
            }

            else if(x>root1->data2)
                root1->data3 = x;

            split(root1);   // hal ke dar 'Btree_node', 3 data vojod darad,on node ra 'split' mikonim //
        }
    }
}

void b_tree :: split(Btree_node* root1)
{
    Btree_node *new_node_left = new Btree_node;     // ebteda 'Btree_node' haye jadidi misazim. 
    new_node_left->data1 = root1->data1;         // data1 'Btree_node' jadid ra, barabar data1 'Btree_node' shamel 3 data gharar midahim //
    new_node_left->left = root1->left;       // farzand haye 1 'Btree_node' shamel 3 data ra be 'Btree_node' node haye jadid midahim //
    new_node_left->mid = root1->mid;
    Btree_node *new_node_right = new Btree_node;
    new_node_right->data1 = root1->data3;
    new_node_right->left = root1->right;
    new_node_right->mid = root1->extra;
    
    if(root1->left !=0)    // agar 'Btree_node' shamel 3data, farzand nashte bashad //
    {
        root1->left->father = new_node_left;
        root1->mid->father = new_node_left;
        root1->right->father = new_node_right;
        root1->extra->father = new_node_right;
    }
    
    if(root1->father == 0)      // agar 'Btree_node' shamel 3data, rishe bashad //
    {
        Btree_node *new_node = new Btree_node;
        new_node->data1 = root1->data2;
        new_node ->left = new_node_left ;
        new_node ->mid  = new_node_right ;

        new_node_left->father = new_node;
        new_node_right->father = new_node;

        root = new_node;
    }

    else                        // tamimi taghirat dar pointer ha va data ha dade mishavad//
    {
        new_node_left->father = root1->father;
        new_node_right->father = root1->father;

        if(root1->father-> data2 == -1)
        {
            if(root1->father->data1 < root1 ->data2)
            {
                root1->father->data2 = root1 ->data2;
                root1->father->mid = new_node_left;
                root1->father->right = new_node_right;
            }
            else
            {
                int temp = root1->father->data1;
                root1->father->data1 = root1 ->data2;
                root1->father->data2 = temp;

                root1->father->right = root1->father->mid;

                root1->father->mid = new_node_right;
                root1->father->left = new_node_left;
            }
        }

        else
        {
            if(root1 -> data2 < root1->father-> data1)
            {
                root1->father-> data3 = root1->father-> data2;
                root1->father-> data2 = root1->father-> data1;
                root1->father-> data1 = root1-> data2;
                root1->father->extra = root1->father->right ;
                root1->father->right = root1->father->mid ;

                root1->father->mid = new_node_right ;
                root1->father->left = new_node_left ;
            }

            else if(root1 -> data2 > root1->father-> data1 && root1-> data2 < root1->father-> data2) 
            {
                root1->father-> data3 = root1->father-> data2;
                root1->father-> data2 = root1->data2 ;

                root1->father->extra = root1->father->right ;
                root1->father->right = new_node_right ;
                root1->father->mid = new_node_left ;
            }

            else if(root1-> data2 > root1->father-> data2)
            {
                root1->father-> data3 = root1->data2;
                root1->father->extra = new_node_right;
                root1->father->right = new_node_left;
            }

            split(root1->father);    //mojadad pedar 'Btree_node' avalie, seda zade mishavad chon momken ast 3 taii shode bashad //
        }
    }
}

Btree_node* b_tree :: find_place(Btree_node* root1 , int x)    // bargi ke bayad dar on adadi insert shavad ra peyda mikonad //
{
    if(root1->left ==0)
        return root1;

    else
    {
        if(x<root1->data1 )
            find_place(root1->left ,x);

        else if(x>root1->data1 && root1->data2== -1)
            find_place(root1->mid ,x);

        else if(x>root1->data2)
            find_place( root1->right ,x);

        else
            find_place(root1->mid , x);
    }
}

void b_tree :: dijkstra(matrix_node** matrix,int start_x,int start_y,int end_y,int n,int m,b_tree tree ,heap min_heap)
{
    matrix[0][start_y].min_weight = matrix[0][start_y].weight;  // 'min_weight' mabda` ra barabar 'weight' mabda` gharar midahim //

    while (!find((n-1)*1000 + end_y , tree.root )) // ta zamani ke maghsad dar tree nabashad, edame midahim //
    {
        if(start_x+1 < n)  // harket be paeen //
        {
            if(matrix[start_x+1][start_y].weight != 0 && !find(((start_x+1)*1000 )+ start_y,tree.root ))  //baresi mikonim divar nabshad va ghablan dide nashode bashad //
            {
                if(matrix[start_x][start_y].min_weight + matrix[start_x+1][start_y].weight <= matrix[start_x+1][start_y].min_weight ) // aghar 'min_weight' jadid kamtar boode, update mikonim //
                {
                    matrix[start_x+1][start_y].min_weight = matrix[start_x][start_y].min_weight + matrix[start_x+1][start_y].weight;
                    matrix[start_x+1][start_y].father = &matrix[start_x][start_y];  // masir ra save mikonim // 
                    if(min_heap.find_id(((start_x+1)*1000) + start_y) != -1) // agar az ghabl vojod dasht,'min_weight' ra update mikonim //
                        min_heap.update_weight(min_heap.find_id((start_x+1)*1000 + start_y),matrix[start_x+1][start_y].min_weight);

                    else // agar az ghabl vojod nadhst, insert mikonim //
                        min_heap.insert(((start_x+1)*1000) + start_y , matrix[start_x+1][start_y].min_weight);
                }
            }
        }

        if(start_y+1 < m)  // harkat be rast // 
        {
            if(matrix[start_x][start_y+1].weight != 0 && !find((start_x)*1000 + (start_y+1),tree.root ))
            {
                if(matrix[start_x][start_y].min_weight + matrix[start_x][start_y+1].weight <= matrix[start_x][start_y+1].min_weight)
                {
                    matrix[start_x][start_y+1].min_weight = matrix[start_x][start_y].min_weight + matrix[start_x][start_y+1].weight;
                    matrix[start_x][start_y+1].father = &matrix[start_x][start_y];

                    if(min_heap.find_id((start_x)*1000 + start_y+1)!=-1)
                    {
                        min_heap.update_weight(min_heap.find_id((start_x)*1000 + start_y+1),matrix[start_x][start_y+1].min_weight);
                    }
                    else
                        min_heap.insert((start_x)*1000 + start_y+1 , matrix[start_x][start_y+1].min_weight);
                }
            }
        }

        if(start_x-1 >= 0)  //harkat be bala //
        {
            if(matrix[start_x-1][start_y].weight != 0 && !find((start_x-1)*1000 + start_y,tree.root ) )
            {
                if(matrix[start_x][start_y].min_weight + matrix[start_x-1][start_y].weight <= matrix[start_x-1][start_y].min_weight )
                {
                    matrix[start_x-1][start_y].min_weight = matrix[start_x][start_y].min_weight + matrix[start_x-1][start_y].weight;
                    matrix[start_x-1][start_y].father = &matrix[start_x][start_y];

                    if(min_heap.find_id((start_x-1)*1000 + start_y)!=-1)
                    {
                        min_heap.update_weight(min_heap.find_id((start_x-1)*1000 + start_y),matrix[start_x-1][start_y].min_weight);
                    }
                    else
                        min_heap.insert((start_x-1)*1000 + start_y , matrix[start_x-1][start_y].min_weight);
                }
            }
        }

        if(start_y-1 >=0)      // harkat be chap //
        {
            if(matrix[start_x][start_y-1].weight != 0 && !find((start_x)*1000 + (start_y-1),tree.root ))
            {
                if(matrix[start_x][start_y].min_weight + matrix[start_x][start_y-1].weight <= matrix[start_x][start_y-1].min_weight)
                {
                    matrix[start_x][start_y-1].min_weight = matrix[start_x][start_y].min_weight + matrix[start_x][start_y-1].weight;
                    matrix[start_x][start_y-1].father = &matrix[start_x][start_y];

                    if(min_heap.find_id((start_x)*1000 + start_y-1)!=-1)
                        min_heap.update_weight(min_heap.find_id((start_x)*1000 + start_y-1),matrix[start_x][start_y-1].min_weight);

                    else
                        min_heap.insert((start_x)*1000 + start_y-1 , matrix[start_x][start_y-1].min_weight);
                }
            }
        }

        tree.insert(tree.root,(start_x*1000) + start_y);  // be onvan deraye dide shde dar b_tree insert mishavad //
        int new_coordinate =  min_heap.delete_root()->id;   // 'id' rishe heap ke shamel derayeii ba kamtrin 'min_weight' save karde va rishe ra hazf mikonim //
        int new_start_x = new_coordinate/1000;      // satr jadid baraye start ra peyda mikonim //
        int new_start_y =new_coordinate % 1000;     // soton jadid baraye start ra peyda mikonim //
        start_x = new_start_x;
        start_y = new_start_y;
    }
    print_mat(matrix,end_y,n,m);        // dar nahayat matrix ra chap mikonim // 
}

bool b_tree :: find (int x,Btree_node*root)  // baresi mikonad 'id' mored nazar be onvan r`as bazdid shode dar 'b_tree' gharar darad ya kheyr //
{
    if(root==0)
        return false;

    if(x == root->data1)
        return true;

    else if(x> root->data1 && root->data2 == -1 && root->mid != 0)
        return find(x,root->mid);

    else if(x==root->data2)
        return true;

    else if(x < root->data1 && root->left!=0 )
        return find(x,root->left);

    else if(x < root->data2 && x > root->data1 && root->mid!= 0)
        return find(x,root->mid);

    else if(x > root->data2 && root->right!= 0)
        return find(x , root->right);

    else
        return false;
}

void b_tree :: print_mat(matrix_node**matrix,int end,int n,int m) //matrix nahaii shamel kotah tarim masir ta maghsad ra chap mikonam //
{
    while(matrix[n-1][end].father != 0) // ebeta hc pedar maghsad shoro` mikonim va 'weight' on ha ra -1 mikonim //
    {
        matrix[n-1][end].father->weight = -1;
        matrix[n-1][end].father = matrix[n-1][end].father->father;
    }
    matrix[n-1][end].weight = -1;  // 'weight' maghsad ra -1 mikonim //
    for(int i=0 ; i<n ; i++)
    {
        for(int j=0 ; j<m ; j++)
        {
            cout<< matrix[i][j].weight << " " ;  // matrix ra chap mikonim //
        }
        cout<<endl;
    }
}

void heap :: insert(int d,int w)  // dar 'heap' insert mikonim //
{
    if(temp == 0)
        que[++temp] = new heap_node(d,w); 

    else
    {
        que[++temp] = new heap_node (d,w);
        check(temp);  //makan jadid onsor insert shode ra peyda mikonim // 
    }
}

bool heap :: compare(heap_node *child,heap_node *father)  // dar 'compare' 2 'heap_node' bar asas 'id' va 'min_weight' moghayese mishavand //
{
    if(child->min_weight_mat < father->min_weight_mat)  // olaviyat ba 'min_weight' ast //
        return true;

    if(child->min_weight_mat > father->min_weight_mat)
        return false;

    if(child->min_weight_mat == father->min_weight_mat)  // agar 'min_weight' ha barabar bashad, olaviyat ba 'id' ast //
    {
        if(child->id < father->id)
            return true;
        else
            return false;
    }

    return false;
}

void heap :: check(int a)      // pas az inser, baresi mishavad ke adad jadid dar makan monaseb khodash gharar girad //
{
    if(a <= 1)
        return;

    if(compare(que[a],que[a/2]))
    {
        swap(que[a],que[a/2]);
        check (a/2);
    }
}

heap_node* heap :: delete_root()     // rishe 'heap' ra delete mikonad//
{
    swap(que[1],que[temp]);
    temp--;
    check2(1);
    return que[temp+1];
}

void heap :: check2(int a)      // pas az hazf rishe, baresi mishavad ke 'heap' mojadad  be dorosti moratab shavad //
{
    if((2*a)+1 <= temp)
    {
        if(compare(que[a],que[a*2]) && compare(que[a],que[(a*2)+1]) )
            return;

        if(compare(que[2*a],que[(2*a)+1]))
        {
            swap(que[a],que[2*a]);
            check2(2*a);
        }
        else
        {
            swap(que[a],que[(2*a)+1]);
            check2((2*a)+1);
        }   
    }

    else if( 2*a <= temp)
    {
        if(compare(que[2*a],que[a]))
        {
            swap(que[a],que[2*a]);
            check2(2*a); 
        }     
    }
}

int heap :: find_id(int d)      // baresi mishavad 'id' mored nazar, az ghabl dar 'heap' vojod darad ya kheyr //
{
    for(int i=0 ; i<temp ; i++) 
    {
        if(que[i]->id == d)
            return i;
    }
    return -1;
}

void heap :: update_weight(int index_i, int new_weight) // dar inja agar 'min_weight' kamtari baraye derayeii peyda shod, meghdarash update mishavad //
{
    que[index_i]->min_weight_mat = new_weight ;
    check(temp);
}
