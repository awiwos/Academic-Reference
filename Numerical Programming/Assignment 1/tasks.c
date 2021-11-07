
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>
#include "tasks.h"

#define INITIAL_SIZE 100 //Initial number of elements to use for malloc
#define FILENAME_TASK1 "task1.csv"
#define FILENAME_TASK2 "task2.csv"
#define FILENAME_TASK3 "task3.csv"
#define FILENAME_TASK4 "task4.csv"
#define X_BOUND_TASK1 20.0
#define Y_VALUE_TASK3 0.0
#define THRESH_NUM 5 //Number of thresholds in Task 4.
#define T4_THRESH_GAP 5
#define T4_THRESH_START 0

/*Enum points for Task 1*/
enum{MAX_U,MIN_U,MAX_V,MIN_V,TARGET_NUMBER};
/*Enumeration of variables stored in struct, data*/
enum{RHO=0,X,Y,U,V,S,VAR_NUM};

typedef struct{
    double vars[VAR_NUM];
} data_t;

typedef struct l_node l_node_t;
typedef struct t_node t_node_t;

/*struct for a node of a linked list*/
struct l_node{
  data_t data;
  l_node_t *next;
};

/*Struct for a node of a BST (tree)*/
struct t_node{
  data_t data;
  t_node_t *left, *right;
};

/*List struct*/
typedef struct {
	l_node_t *head;
	l_node_t *foot;
} list_t;

/*BST struct*/
typedef struct{
  t_node_t *root;
} tree_t;
/*=====================General prototypes===================================*/
FILE* safe_fopen(const char* path, const char* mode);
void* safe_malloc(size_t bytes);
void* safe_realloc(void* ptr,size_t bytes);
data_t* read_data(const char* flow_file, int* data_points);
void handle_headings(FILE* fp);
void merge_sort(data_t* array, int left, int right
               ,int cmp(const void*, const void*));
void merge(data_t* array, int l, int m, int r
          ,int cmp(const void*, const void*));
int is_clsr_to_than(double is, double to, double than);
/*=====================Task 1 prototypes====================================*/
void find_extrema(data_t* data_array,int data_points, int* target_points);
void cmp_max(double base_val, double new_val, int* base_ind, int new_ind);
void cmp_min(double base_val, double new_val, int* base_ind, int new_ind);
void task1_output(data_t* data_array, int* target_points);
void data_point_output(FILE* fp, data_t point);
/*=====================Task 2 prototypes====================================*/
data_t* init_coarse_array(int res);
int* init_count_array(int res);
void fill_coarse(data_t* data_array, int data_points, data_t* coarse_array
                ,int*count, int res);
void find_delta_and_min(data_t* data_array,int data_points,double* min_val
                       ,double* delta,int var,int res);
int find_max(data_t* data_array,int data_points,int var);
int find_min(data_t* data_array,int data_points,int var);
void calc_coarse_vals(data_t* coarse_array,int* count,int res);
double calc_s(data_t point);
int task2_cmp(const void *first,const void *second);
void task2_output(data_t* coarse_array, int res);
/*=====================Task 3 prototypes====================================*/
data_t* task3_read(int* data_points,const char* flow_file);
int task3_cmp(const void *first,const void *second);
//Array search functions
void array_searches(data_t* array, int data_points, double target);
void lin_search(data_t* data_array, int data_points,double target);
void bin_search(data_t* data_array,int data_points,double target);
//list search functions
void list_control(data_t *array, int data_pts, double target);
list_t *make_empty_list(void);
list_t *insert_at_foot(list_t *list,data_t data);
void free_list(list_t *list);
void list_search(list_t* data_list,double target);
//BST search functions
void bst_control(data_t *array,int data_pts,double target);
tree_t *make_empty_tree(void);
void tree_insert(tree_t* tree,data_t *data);
void bst_search(tree_t* data_tree,double target);
void balanced_tree_insert(data_t* sorted_array,tree_t* tree,int low,int high);
t_node_t *find_parent(t_node_t* node, data_t* data);
void free_bst(t_node_t *node);
/*=====================Task 4 prototypes====================================*/
data_t** task4_read(int *rows,int *columns,const char* flow_file);
data_t first_row(data_t **data_array,int *rows,int *columns,FILE *fp);
data_t **sub_rows(data_t **array,int *rows,int *columns,FILE *fp
                 ,data_t *first);
void calc_vort(double **vort,data_t **data_array,int rows,int columns);
void task4_output(double **vort,int rows, int columns);

/*============================================================================
-----------------------------General Functions--------------------------------
============================================================================*/
/*Safe fopen to ensure file opening successful*/
FILE* safe_fopen(const char* path, const char* mode){
   FILE* fp = fopen(path, mode);
   //if file pointer null, fopen unsuccessful, then failure
   if (fp == NULL) {
      printf("file open error.");
      exit(EXIT_FAILURE);
   }
   return fp;
}

/*Safe malloc to ensure malloc successful*/
void* safe_malloc(size_t bytes){
   void* ptr = malloc(bytes);
   //If ptr null, malloc unsuccessful, then failure
   if (ptr == NULL) {
      printf("error mallocing %lu bytes\n", bytes);
      exit(EXIT_FAILURE);
   }

   return ptr;
}

/*Safe realloc to ensure realloc successful*/
void* safe_realloc(void* ptr,size_t bytes){
   void* new_ptr = realloc(ptr,bytes);
   //if new_ptr null, realloc unsuccessful, then failure
   if (new_ptr == NULL) {
      printf("error reallocing %lu bytes\n", bytes);
      exit(EXIT_FAILURE);
   }
   return new_ptr;
}

/*Read flow data file*/
data_t *read_data(const char* flow_file,int* data_points){
   FILE* fp;
   fp = safe_fopen(flow_file,"r");;
   data_t* data_array;
   int current_size = INITIAL_SIZE;
   data_t temp;
   data_array = safe_malloc(INITIAL_SIZE*sizeof(data_t));
/*Dispose of headings in the data file.*/
   handle_headings(fp);
/*While there are data points in the file record them one by one into the
data array, and allocate more space as necessary.*/
   while(fscanf(fp,"%lf,%lf,%lf,%lf,%lf",&temp.vars[RHO],&temp.vars[U],
                 &temp.vars[V],&temp.vars[X],&temp.vars[Y])==5){
/*If there is no more space in the data array, double the allocated space*/
      if(*data_points==current_size){
         current_size*=2;
         data_array = safe_realloc(data_array,current_size*sizeof(data_t));
      }
/*Store the next set of data in the data array*/
      data_array[*data_points] = temp;
      *data_points += 1;
   }
   fclose(fp);
   return data_array;
}

/*Handles (disposes of) headings of flow data file*/
void handle_headings(FILE* fp){
   fscanf(fp,"rho,u,v,x,y\n");
}

/*Garden variety merge sort over array of type data_t with respect to
comparison function, cmp*/
void merge_sort(data_t* array, int l, int r,int cmp(const void*, const void*)){
   if(r>l){
      int m = (int)(l+r)/2;
      merge_sort(array,l,m,cmp);
      merge_sort(array,m+1,r,cmp);
      merge(array,l,m,r,cmp);
    }
}

/*merge function for merge sort again over array of type data_t and comparison
function, cmp*/
void merge(data_t* array, int l, int m, int r
           ,int cmp(const void*, const void*)){
    /*Step 1: find number of elements are in each sub array and allocate space
    for temporary arrays*/
   int i=l,j=m+1,k=0;
   data_t* tmp = (data_t*)safe_malloc((r-l+1)*sizeof(data_t));
   while(i<=m&&j<=r) {
      tmp[k++]=cmp(&array[i],&array[j])<1?array[i++]:array[j++];
   }
   //tidy up remaining elements
   while(i<=m) {
      tmp[k++] = array[i++];
   }
   while(j<=r) {
      tmp[k++] = array[j++];
   }
   //copy sorted temp array back to original array
   for(i=l;i<=r;i++) array[i] = tmp[i-l];
   free(tmp);
}

//returns 1 if is is closer to to than than. Otherwise 0.
int is_clsr_to_than(double is, double to, double than){
   return (fabs(is-to)<fabs(to-than))?1:0;
}
/*============================================================================
----------------------------------Task 1--------------------------------------
============================================================================*/
void maxfluxdiff(const char* flow_file)
{
/*Initialise array to hold all data points, buddy variable with number of
data points,and array to store indexes of the relevant points.*/
   data_t* data_array;
   int data_points=0;
   int extrema_pts[TARGET_NUMBER];
/*Read in the data points from data file. data_array is dynamically
allocated.*/
   data_array = read_data(flow_file, &data_points);
/*Find the data points with the maximum velocity difference and store their
indexes from the data array*/
   find_extrema(data_array,data_points,extrema_pts);
/*Output the data points with the maximum velocity difference.*/
   task1_output(data_array,extrema_pts);
/*Free the dynamically allocated array to store the data points.*/
   free(data_array);
}


/*Find the maximun flux difference by finding extrema for values in
u and v and storing them in the array pts.*/
void find_extrema(data_t* data_array,int data_points, int* pts){
   int first_val = 1;
   /*Iterate over all points in the data array*/
   for(int i=0;i<data_points;i++){
      if(data_array[i].vars[X]>X_BOUND_TASK1){
         /*If this is the first data point above the X bound let all indexes
         be that of the data point.*/
         if(first_val){
            first_val = 0;
            pts[MAX_U] = pts[MIN_U] = pts[MAX_V] = pts[MIN_V] = i;
         }
         /*Check to see if the current data point has a greater than maximum
         or lesser than minimum data value than the current maxs and minimums
         and update as necessary*/
         cmp_max(data_array[pts[MAX_U]].vars[U]
                *data_array[pts[MAX_U]].vars[RHO]
                ,data_array[i].vars[U]*data_array[i].vars[RHO]
                ,&pts[MAX_U],i);
         cmp_min(data_array[pts[MIN_U]].vars[U]
                *data_array[pts[MIN_U]].vars[RHO]
                ,data_array[i].vars[U]*data_array[i].vars[RHO]
                ,&pts[MIN_U],i);
         cmp_max(data_array[pts[MAX_V]].vars[V]
                *data_array[pts[MAX_V]].vars[RHO]
                ,data_array[i].vars[V]*data_array[i].vars[RHO]
                ,&pts[MAX_V],i);
         cmp_min(data_array[pts[MIN_V]].vars[V]
                *data_array[pts[MIN_V]].vars[RHO]
                ,data_array[i].vars[V]*data_array[i].vars[RHO]
                ,&pts[MIN_V],i);
        }
    }
}

/*If new value greater than base value update the base index to new index*/
void cmp_max(double base_val, double new_val, int* base_ind, int new_ind){
   if(new_val>base_val){
      *base_ind = new_ind;
   }
}

/*If new value less than base value update the base index to new index*/
void cmp_min(double base_val, double new_val, int* base_ind, int new_ind){
   if(new_val<base_val){
      *base_ind = new_ind;
   }
}

/*Output data points with the greatest velocity differences*/
void task1_output(data_t* data_array, int* target_points){
   FILE* fp;
   fp = safe_fopen(FILENAME_TASK1,"w");
   fprintf(fp,"rho,u,v,x,y\n");
   data_point_output(fp,data_array[target_points[MAX_U]]);
   data_point_output(fp,data_array[target_points[MIN_U]]);
   data_point_output(fp,data_array[target_points[MAX_V]]);
   data_point_output(fp,data_array[target_points[MIN_V]]);
   fclose(fp);
}

/*Output results to file*/
void data_point_output(FILE* fp,data_t point){
   fprintf(fp,"%lf.6,%lf.6,%lf.6,%lf.6,%lf.6\n"
   ,point.vars[RHO],point.vars[U],point.vars[V],point.vars[X],point.vars[Y]);
}

/*============================================================================
----------------------------------Task 2--------------------------------------
============================================================================*/

void
coarsegrid(const char* flow_file, int res)
{
   data_t *data_array,*coarse_array;
   int data_points=0, *count;
   /*Read in data*/
   data_array = read_data(flow_file, &data_points);
   /*Initialise coarse array, and count array, used to store number of
   data points in each cell of the coarse array.*/
   coarse_array=init_coarse_array(res);
   count = init_count_array(res);
   /*fill the coarse array*/
   fill_coarse(data_array,data_points,coarse_array,count,res);
   /*Sort the coarse array over the whole array (i.e. index 0 to size-1)
   by S with merge sort*/
   merge_sort(coarse_array,0,res*res-1,task2_cmp);
   /*Output results*/
   task2_output(coarse_array, res);
   /*Free dynamically allocated memory.*/
   free(count);
   free(data_array);
   free(coarse_array);
}

/*Allocate memory for the coarse array, and initialise all values to zero.*/
data_t*
init_coarse_array(int res){
   data_t blank_point;
   blank_point.vars[RHO]=0.0;
   blank_point.vars[U]=0.0;
   blank_point.vars[V]=0.0;
   blank_point.vars[X]=0.0;
   blank_point.vars[Y]=0.0;
   blank_point.vars[S]=0.0;
   data_t* coarse_array;
   coarse_array = (data_t*)safe_malloc(sizeof(data_t)*res*res);
   for(int i=0;i<res*res;i++){
      coarse_array[i] = blank_point;
   }
   return coarse_array;
}

/*Allocate memory for the count array, and initialise all values to zero*/
int*
init_count_array(int res){
   int* count;
   count = (int*)safe_malloc(sizeof(int)*res*res);
   for(int i=0;i<res*res;i++){
      count[i]=0;
   }
   return count;
}

/*Fill the coarse array using the data array*/
void
fill_coarse(data_t* data_array, int data_points, data_t* coarse_array
            ,int*count, int res){
   double delta_x=0.0, delta_y=0.0, min_x_val,min_y_val;
   int row, column,coarse_ind;
   /*Find and store the minimum x and y values as well as the increase in x
    and y values between the boundaries of cells in the coarse array*/
   find_delta_and_min(data_array,data_points,&min_x_val,&delta_x,X,res);
   find_delta_and_min(data_array,data_points,&min_y_val,&delta_y,Y,res);
   /*For each point in the data array calculate the appropriate cell
   in the coarse grid and add the values of the point to the relevant cell's
   values and increment the count for that cell.*/
   for(int i=0;i<data_points;i++){
      column = (int)((data_array[i].vars[X] - min_x_val)/delta_x);
      row    = (int)((data_array[i].vars[Y] - min_y_val)/delta_y);
      /*If element not exactly on boudary, elements in the coarse array are
       stored by row then column.*/
      if((((column*delta_x)+min_x_val)!=data_array[i].vars[X])&&
            (((row*delta_y)+min_y_val)!=data_array[i].vars[Y])){
         coarse_ind=(res*column)+row;
         for(int j=0;j<VAR_NUM-1;j++){
            coarse_array[coarse_ind].vars[j] += data_array[i].vars[j];
         }
         count[coarse_ind]+=1;
       }
    }
    /*For each cell in the coarse array, average the values by the number of
    data points captured by the cell (stored in the count array) and calculate
    'S' value*/
    calc_coarse_vals(coarse_array,count,res);
}

/*Find and store the minimum x and y values as well as the increase in x
and y values between the boundaries of cells in the coarse array*/
void
find_delta_and_min(data_t* data_array,int data_points,double* min_val
                  ,double* delta,int var,int res){
   /*Find and store the index of the max and min value of the specified
   variable*/
   int max = find_max(data_array,data_points,var);
   int min = find_min(data_array,data_points,var);
   double max_val = data_array[max].vars[var];
   *min_val = data_array[min].vars[var];
   *delta = (max_val - *min_val)/res;
}

/*Searches for and returns the index of the array element with the max value
for the specified variable*/
int find_max(data_t* data_array,int data_points,int var){
   int ind = 0;
   for(int i=0;i<data_points;i++){
      cmp_max(data_array[ind].vars[var],data_array[i].vars[var],&ind,i);
   }
   return ind;
}

/*Searches for and returns the index of the array element with the min value
for the specified variable*/
int find_min(data_t* data_array,int data_points,int var){
   int ind = 0;
   for(int i=0;i<data_points;i++){
      cmp_min(data_array[ind].vars[var],data_array[i].vars[var],&ind,i);
   }
   return ind;
}

/*Average values in coarse array and calculate S*/
void calc_coarse_vals(data_t* coarse_array,int* count,int res){
   for(int i=0;i<res*res;i++){
      for(int j=0;j<VAR_NUM-1;j++){
         if(count!=0){
            coarse_array[i].vars[j] /= count[i];
         }
      }
      coarse_array[i].vars[S] = calc_s(coarse_array[i]);
   }
}

/*Calculate S value for data point given other data parameters. Calculation
per assignment page 3.*/
double calc_s(data_t point){
   double x=point.vars[X],y=point.vars[Y],u=point.vars[U],v=point.vars[V];
   return 100.0*sqrt(((u*u)+(v*v))/((x*x)+(y*y)));
}

/*Task 2 comparison for merge sort - returns 1 if the S value of the
first element is larger, -1 if smaller and 0 if equal.*/
int task2_cmp(const void *first,const void *second){
   data_t a = *(data_t*)first;
   data_t b = *(data_t*)second;
   if(a.vars[S]>b.vars[S]) return 1;
   else if (a.vars[S]<b.vars[S]) return -1;
   return 0;
}

/*Output coarse array data points from smallest to largest S value*/
void task2_output(data_t* coarse_array, int res){
   FILE* fp;
   fp = safe_fopen(FILENAME_TASK2,"w");
   fprintf(fp,"rho,u,v,x,y,S\n");
   for(int i=res*res-1;i>=0;i--){
      fprintf(fp,"%.6lf,%.6lf,%.6lf,%.6lf,%.6lf,%.6lf\n"
      ,coarse_array[i].vars[RHO],coarse_array[i].vars[U]
      ,coarse_array[i].vars[V],coarse_array[i].vars[X]
      ,coarse_array[i].vars[Y],coarse_array[i].vars[S]);
   }
   fclose(fp);
}
/*============================================================================
----------------------------------Task 3--------------------------------------
============================================================================*/
/*Task 3 control function*/
void searching(const char* flow_file)
{
   data_t* array;
   int data_pts=0;
   //create array and read into data to it.
   array = task3_read(&data_pts,flow_file);
   //sort array by rho*u
   merge_sort(array,0,data_pts-1,task3_cmp);
   //Identify target, and call call searching control functions
   double target = array[data_pts-1].vars[RHO]*array[data_pts-1].vars[U]*0.4;
   array_searches(array,data_pts,target);
   list_control(array,data_pts,target);
   bst_control(array,data_pts,target);
   free(array);
}

/*Read in data points along Y axis and return array*/
data_t* task3_read(int* data_points,const char* flow_file){
   FILE* fp;
   fp = safe_fopen(flow_file,"r");
   data_t* data_array;
   int current_size = INITIAL_SIZE;
   data_t temp;
   data_array = safe_malloc(INITIAL_SIZE*sizeof(data_t));
/*Dispose of headings in the data file.*/
   handle_headings(fp);
/*While there are data points in the file record them one by one into the
data array, and allocate more space as necessary.*/
   while(fscanf(fp,"%lf,%lf,%lf,%lf,%lf",&temp.vars[RHO],&temp.vars[U],
                &temp.vars[V],&temp.vars[X],&temp.vars[Y])==5){
/*If there is no more space in the data array, double the allocated space*/
      if(temp.vars[Y]==Y_VALUE_TASK3){
         if(*data_points==current_size){
            current_size*=2;
            data_array = safe_realloc(data_array, current_size*sizeof(data_t));
         }
         data_array[*data_points] = temp; //combine these two lines
         (*data_points)++;
      }
   }
   fclose(fp);
   return data_array;
}
/*If first has greater rho*u than second return 1 if less return -1
otherwise 0*/
int task3_cmp(const void *first,const void *second){
   data_t a = *(data_t*)first;
   data_t b = *(data_t*)second;
   if((a.vars[RHO]*a.vars[U])>(b.vars[RHO]*b.vars[U])) return 1;
   else if((a.vars[RHO]*a.vars[U])<(b.vars[RHO]*b.vars[U])) return -1;
   return 0;
}

void array_searches(data_t* array, int data_points, double target){
   struct timeval start, stop;
   //Do linear search on arrat and report time
   gettimeofday(&start, NULL);
   lin_search(array,data_points,target);
   gettimeofday(&stop, NULL);
   double elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
   printf("Task 3 Array Linear Search:  %.2f milliseconds\n", elapsed_ms);
   //Do binary search on array and report time
   gettimeofday(&start, NULL);
   bin_search(array,data_points,target);
   gettimeofday(&stop, NULL);
   elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
   printf("Task 3 Array Binary Search:  %.2f milliseconds\n", elapsed_ms);
}
/*Perform linear search on array for target*/
void lin_search(data_t* array, int data_points,double target){
   FILE* fp;
   int i=1;
   fp = safe_fopen(FILENAME_TASK3,"w");
   //print first value
   fprintf(fp,"%lf",array[0].vars[RHO]*array[0].vars[U]);
   //while next is closer to target print next
   while((i<data_points)&&(is_clsr_to_than(array[i].vars[RHO]
         *array[i].vars[U],target,array[i-1].vars[RHO]*array[i-1].vars[U]))){
      fprintf(fp,",%lf",(array[i].vars[RHO]*array[i].vars[U]));
      i++;
   }
   fprintf(fp,"\n");
   fclose(fp);
}

/*Perform binary search on array*/
void bin_search(data_t* data_array, int data_points,double target){
   FILE* fp;
   fp = safe_fopen(FILENAME_TASK3,"a");
   //output first value
   fprintf(fp,"%lf",data_array[0].vars[U]);
   //while there's a gap between values under inspection
   int l=0,m,r=data_points;
   while(r>l){
      //identify middle value
      m = (l+r)/2;
      //ouput middle value
      fprintf(fp,",%lf",(data_array[m].vars[RHO]*data_array[m].vars[U]));
      //update bounds depending on relation between target and middle value
      if(target<data_array[m].vars[RHO]*data_array[m].vars[U]) r=m-1;
      else if(target>data_array[m].vars[RHO]*data_array[m].vars[U]) l=m+1;
      else break;
    }
    fprintf(fp,"\n");
    fclose(fp);
}

/*Control function to operate list search*/
void list_control(data_t *array, int data_pts, double target){
   struct timeval start, stop;
   list_t *list;
   //make list and insert values from sorted array
   list = make_empty_list();
   for(int i=0;i<data_pts;i++){
      insert_at_foot(list,array[i]);
   }
   //perform list search and report time taken
   gettimeofday(&start, NULL);
   list_search(list,target);
   gettimeofday(&stop, NULL);
   double elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
   elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
   printf("Task 3 List Linear Search:  %.2f milliseconds\n", elapsed_ms);
   free_list(list);
}

/*make empty list, set head and tail to null and return*/
list_t *make_empty_list(void){
	list_t *list;
	list = (list_t*)safe_malloc(sizeof(*list));
	list -> head = list -> foot = NULL;
	return list;
}

/*Insert new data to foot of list*/
list_t *insert_at_foot(list_t *list, data_t data){
	l_node_t  *new;
   //create new node
	new = (l_node_t*)safe_malloc(sizeof(*new));
	new -> data = data;
	new->next = NULL;
	/*if list empty do first insertion to list otherwise insert after last
   node*/
	if(list->foot==NULL){
		list -> head = list->foot = new;
	} else {
		list->foot->next = new;
		list -> foot = new;
	}
	return list;
}

/*perform list search for target.*/
void list_search(list_t* data_list,double target) {
    FILE* fp;
    fp = safe_fopen(FILENAME_TASK3,"a");
    l_node_t *node = data_list->head;
    fprintf(fp,"%lf",node->data.vars[RHO]*node->data.vars[U]);
    //while next node is closer to the than current node, print next
    while(is_clsr_to_than(node->next->data.vars[RHO]*node->next->data.vars[U]
          ,target,node->data.vars[RHO]*node->data.vars[U])&&node->next!=NULL){
       fprintf(fp,",%lf",node->next->data.vars[RHO]*node->next->data.vars[U]);
       node = node->next;
    }
    fprintf(fp,"\n");
    fclose(fp);
}

/*Free list, by freeing all the nodes, then freeing list*/
void free_list(list_t *list){
	l_node_t *curr, *prev;
	curr = list -> head;
	while(curr) {
		prev = curr;
		curr = curr -> next;
		free(prev);
	}
	free(list);
}

/*Control function for BST search*/
void bst_control(data_t *array,int data_pts,double target){
   struct timeval start, stop;
   //Create balanced tree from sorted array.
   tree_t *tree;
   tree = make_empty_tree();
   tree->root = NULL;
   balanced_tree_insert(array,tree,0,data_pts-1);
   //perform timed binary search
   gettimeofday(&start, NULL);
   bst_search(tree,target);
   gettimeofday(&stop, NULL);
   //report BST search time
   double elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
   elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
   printf("Task 3 BST Search:  %.2f milliseconds\n", elapsed_ms);
   //Free tree
   if(tree->root!=NULL) {
      free_bst(tree->root);
   }
   free(tree);
}
/*Dynamically allocates memory for tree, initialises tree to empty and returns
tree.*/
tree_t *make_empty_tree(void){
    tree_t *tree;
    tree = (tree_t*)safe_malloc(sizeof(tree_t));
    tree -> root = NULL;
    return tree;
}
/*Takes sorted array, between indexes l and r, and inserts in a binary
search fashion to ensure a balanced tree.*/
void balanced_tree_insert(data_t* array,tree_t* tree,int l,int r){
   /*If there's only one element left insert it, otherwise if more elements
   insert the middle element, subsequently insert nodes to left, then right.*/
   if(l==r){
      tree_insert(tree,&array[l]);
   }else if (r>l){
      int m = (r+l)/2;
      tree_insert(tree,&array[m]);
      balanced_tree_insert(array,tree,l,m-1);
      balanced_tree_insert(array,tree,m+1,r);
   }
}
/*Takes value of data and inserts into tree*/
void tree_insert(tree_t *tree, data_t *data){
   //set up node to be inserted as temp
   t_node_t *tmp = (t_node_t*)safe_malloc(sizeof(t_node_t));
   tmp -> data = *data;
   tmp -> right = NULL;
   tmp -> left = NULL;
   //If tree empty insert as tree's root
   if(tree->root == NULL) {
      tree->root = tmp;
      return;
   }
   //Find parent of insertion value, and insert to left/right as appropriate
   t_node_t *parent = find_parent(tree->root,data);
   if(task3_cmp(data,&(parent->data))==1){
      parent->right = tmp;
   } else if(task3_cmp(data,&(parent->data))==-1){
      parent->left = tmp;
   } else {
      //repeated points not accepted. Exit failure
      exit(EXIT_FAILURE);
   }
}
/*Returns parent from which data aught to be inserted at*/
t_node_t *find_parent(t_node_t* node, data_t* data){
   t_node_t* next=NULL;
   if(task3_cmp(data,&(node->data))==1){
      next=node->right;
   } else if(task3_cmp(data,&(node->data))==-1){
      next=node->left;
   } else {
      exit(EXIT_FAILURE);
   }
   return (next==NULL)?node:find_parent(next,data);
}
/*Searches BST for target outputting nodes inspected along the way*/
void bst_search(tree_t *tree, double target){
   FILE* fp;
   fp = safe_fopen(FILENAME_TASK3,"a");
   t_node_t *node = tree->root;
   double closest,current;
   closest = current = node->data.vars[RHO]*node->data.vars[U];
   fprintf(fp,"%lf",current);
   node=target>current?node->right:node->left;
   while(node!=NULL){
      current = node->data.vars[RHO]*node->data.vars[U];
      fprintf(fp,",%lf",current);
      if(is_clsr_to_than(current,target,closest)) closest = current;
      node=target>current?node->right:node->left;
   }
   fprintf(fp,",%lf",closest);
   fprintf(fp,"\n");
   fclose(fp);
}
/*Free nodes to left, then nodes to right, then free node.*/
void
free_bst(t_node_t* node) {
    if(node->left!=NULL) free_bst(node->left);
    if(node->right!=NULL) free_bst(node->right);
    free(node);
}

/*============================================================================
----------------------------------Task 4--------------------------------------
============================================================================*/
void vortcalc(const char* flow_file)
{
   int rows = 0, columns = 0;
   data_t **data_array;
   //Read in data from flow file to data array
   data_array = task4_read(&rows,&columns,flow_file);
   /*Malloc row space and column space in each row for vortex data.
   Noted vort array has one less column and row than data array*/
   double **vort = (double**)safe_malloc((rows-1)*sizeof(double*));
   for(int i=0;i<rows-1;i++) {
      vort[i] = (double*)safe_malloc((columns-1)*sizeof(double));
   }
   //calculate and output vortex data
   calc_vort(vort,data_array,rows,columns);
   task4_output(vort, rows, columns);
   //Free data array and vort array.
   for(int i=0;i<rows;i++) free(data_array[i]);
   free(data_array);
   for(int i=0;i<rows-1;i++) free(vort[i]);
   free(vort);
}

/*Read in flow data file to a 2D array*/
data_t** task4_read(int *rows,int *columns,const char* flow_file){
   data_t **data_array, temp;
   //Initial allocation of row space in array
   data_array = (data_t**)safe_malloc(INITIAL_SIZE*sizeof(data_t*));
   FILE *fp;
   fp = safe_fopen(flow_file,"r");
   handle_headings(fp);
   /*First row filled seperately to remaining rows to avoid using realloc
     to fill each row. First value after first row stored in temp*/
   temp = first_row(data_array,rows,columns,fp);
   //Fill subsequent rows now that required column space is known
   data_array = sub_rows(data_array,rows,columns,fp,&temp);
   fclose(fp);
   return data_array;
}

/*Reads in row of values (lowest y) and returns first data after first row*/
data_t first_row(data_t **data_array,int *rows,int *columns,FILE *fp){
   int col_size = INITIAL_SIZE;
   data_t temp;
   double y_val;
   //Allocate column space for the first row
   data_array[*rows] = (data_t*)safe_malloc(col_size*sizeof(data_t));
   //Read in first element and store y value to track row change
   fscanf(fp,"%lf,%lf,%lf,%lf,%lf",&temp.vars[RHO],&temp.vars[U],
              &temp.vars[V],&temp.vars[X],&temp.vars[Y]);
   y_val = temp.vars[Y];
   data_array[*rows][(*columns)++] = temp;
   //While data to read place into array
   while(fscanf(fp,"%lf,%lf,%lf,%lf,%lf",&temp.vars[RHO],&temp.vars[U],
              &temp.vars[V],&temp.vars[X],&temp.vars[Y])==5){
      /*If y value changes end of row, return first value of second row, so
      not lost*/
      if(temp.vars[Y]!=y_val) {
         (*rows)++;
         return temp;
      }
      //If column space in first row filled double space with realloc
      if(*columns==col_size){
         col_size*=2;
         data_array[*rows]=safe_realloc(data_array[*rows]
                                        ,col_size*sizeof(data_t));
      }
      data_array[*rows][(*columns)++] = temp;
   }
   /*If get here there was no second row of data. Vort calc can't operate
     without at least two rows of data. Therefore failure.*/
   exit(EXIT_FAILURE);
}

data_t **sub_rows(data_t **array,int *rows,int *columns,FILE *fp
                  ,data_t *first){
   int row_size = INITIAL_SIZE,curr_col=0;
   data_t temp;
   double y_val = first->vars[Y];
   array[*rows] = (data_t*)safe_malloc(*columns*sizeof(data_t));
   array[*rows][curr_col++]=(*first);
   while(fscanf(fp,"%lf,%lf,%lf,%lf,%lf",&(temp.vars[RHO]),&(temp.vars[U]),
   &(temp.vars[V]),&(temp.vars[X]),&(temp.vars[Y]))==5){
      //If new y val then there's a new row of data
      if(temp.vars[Y]!=y_val){
         (*rows)++;
         curr_col = 0;
         //If out of row space, double row space with realloc
         if(*rows==row_size){
            row_size*=2;
            array=(data_t**)safe_realloc(array, row_size*sizeof(data_t*));
         }
         //allocate column space for the new row and update y value
         array[*rows]=(data_t*)safe_malloc(*columns*sizeof(data_t));
         y_val = temp.vars[Y];
      }
      array[*rows][curr_col++] = temp;
   }
   (*rows)++;
   return array;
}

/*Calculates each of the vorticy value for each element in the matrix*/
void calc_vort(double **vort,data_t **data_array,int rows,int columns){
   double v,v_plus,x,x_plus,u,u_plus,y,y_plus;
   /*Iterate over all elements in vort array, noted vort array has one
   less row and one less column than the data array. For each element.
   calc vort value from data array.*/
   for(int i=0;i<rows-1;i++){
      for(int j=0;j<columns-1;j++){
         v = data_array[i][j].vars[V];
         v_plus = data_array[i][j+1].vars[V];
         x = data_array[i][j].vars[X];
         x_plus = data_array[i][j+1].vars[X];
         u = data_array[i][j].vars[U];
         u_plus = data_array[i+1][j].vars[U];
         y = data_array[i][j].vars[Y];
         y_plus = data_array[i+1][j].vars[Y];
         vort[i][j]= ((v_plus - v)/(x_plus - x)) - ((u_plus-u)/(y_plus - y));
      }
   }
}

/*Calculate values between thresholds and output them to file.*/
void task4_output(double **vort,int rows, int columns){
   FILE *fp;
   //Set up threshold values
   int thresh[THRESH_NUM+1], thresh_count[THRESH_NUM]={0};
   for(int i=0;i<THRESH_NUM+1;i++) {
       thresh[i] = T4_THRESH_START + (i*T4_THRESH_GAP);
   }
   //for each element in vort array, if inbetween thresholds then count.
   for(int i=0;i<rows-1;i++){
      for(int j=0;j<columns-1;j++){
         for(int k=0;k<THRESH_NUM;k++){
            if(vort[i][j]>(double)thresh[k]&&vort[i][j]<(double)thresh[k+1]){
               thresh_count[k]++;
               break;
            }
         }
      }
   }
   //Output results to file
   fp = safe_fopen(FILENAME_TASK4,"w");
   fprintf(fp,"threshold,points\n");
   for(int k=0;k<THRESH_NUM;k++){
      fprintf(fp,"%d,%d\n",thresh[k+1],thresh_count[k]);
   }
   fclose(fp);
}
