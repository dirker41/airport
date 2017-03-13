//WIN VISTA 
//低能C (DEV-C)
//讀CITY做出CITY跟數字轉換的TABLE 
//讀FLIGHT 做出 FLIGHT的TABLE
/*
SI1->DI1->DI2-DI3....   跟課本說的表一樣 不過我終點部分還存了FLIGHT編號跟錢 
 ↓                     SI表出發地 DI表目的地 都已經換成對應編號 
SI2->DI1.....           這張TABLE是個LINKLIST 
 ↓ 
SI2 ->......... 
*/ 


//讀REQUEST開始做事情 一開始先把城市名轉成CITY_TABLE中對應的編號
//如果發現數字為0 表示REQUEST的城市 並不存在CITY_TABLE中 印錯誤訊息 OVER 
//輸入正確的情況 一開始先PUSH並檢查出發地(SI)可以直達的CITY 如果有目的地(DI)
//則PUSH DI 只要是能到達終點的情況都要跟CHEAPEST比較 如果更便宜就換 
// 在來就POP 然後繼續測下一城市
// 如果SI 並沒有直達 DI 一樣先把SI PUSH 然後再一一把SI可以直飛的地方
// 當成新的SI 在呼叫自己一次 
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<fstream>
using namespace :: std ;

typedef char str20[20] ;
typedef char str200[200] ;
typedef char * CharPtr ;

struct city {
  int id ;     
  CharPtr name ;      
  city * next;             
} ;
/*
struct destination {
  int id ;     
  destination * next;             
} ;
*/


struct Stack {
  int id ;     
  Stack * next;             
} ;


struct cost {
  int di_id ;   
  int cost1 ;
  int flight_num ;    
  cost *next;             
} ;


struct flight {
  int si_id ;        
  cost * des ;
  flight * next;             
} ;

/*
struct cost_to_des {
  int di_id ;  
  int cost1 ;
  int flight_num ;   
  destination * next;             
} ;
*/



typedef Stack *stackptr ;
typedef city *cityptr ;
typedef cost *costptr ;
// typedef destination *destinationptr ;
typedef flight *flightptr ;
// typedef cost_to_des cost_to_desptr;

city *city_table = NULL ;
flight *flight_table = NULL ;
Stack *stack = NULL ;
Stack *cheapest_stack = NULL ;
// cost *cost_table = NULL ;



ifstream fp;
ofstream fpout;

bool getfile();
void getcity() ;
void getflight() ;
void dealrequest() ;
void sort_table( cityptr head ,int count) ;
cityptr findbigest( cityptr head  ) ;
// void print( cityptr head ) ;
void print( flightptr head ) ;
void print( flightptr head , costptr chead ) ;
char *num_to_name( int num ) ;
char *num_to_name1( cityptr head , int num ) ;
char *clear_name( str20 name ) ;
int name_to_num1( cityptr head , CharPtr name ) ;
int name_to_num( CharPtr name ) ;
bool newSI( int SI ) ;
bool newSI1( flightptr head , int SI ) ;
bool more_input() ;
flightptr same_si_loc( const int SI ) ;
flightptr same_si_loc1( flightptr head , const int SI ) ;
bool same_city( CharPtr SI,CharPtr DI ) ;
bool not_serve( int SI,int DI ) ;
void push( int id ) ;
int pop() ;
int pop( int & id ) ;
bool SI_in_stack( int SI ) ;
bool SI_in_stack1( stackptr head ,int SI ) ;
bool had_flight( int SI , int DI ) ;
void get_cheapst() ;
void print( stackptr head ) ;
void printcheap() ;
void printcheap1( stackptr head ,int &total ) ;
int get_cost( int SI , int DI ) ;
int get_flight_num( int SI , int DI ) ;
int get_total( stackptr head ) ;
void copy( stackptr & copied , stackptr coping ) ;
void del( stackptr &head ) ;
void copy1( stackptr &copied , stackptr coping ) ;

     
int main() {
    getcity();
    system("pause") ;
    return 0;
}

/*
    len = strlen( str100 ) ;
    str = new char[len+1] ;
    strcpy( str , str100 ) ;
*/

void getcity(){
     if ( !getfile() ) return ;
     str20 name ;
     int count = 0 ;
     cityptr temp = NULL ;
     fp >> name ; // eat city
     fp >> name ;
     while ( strcmp( name , "flight:" ) != 0 ) {
           if ( city_table == NULL ) { // empty table
             city_table = new city ;
             city_table->name = new char[ strlen(name) + 1 ] ;
             strcpy( city_table->name , name ) ;
             city_table->next = NULL ;
             temp = city_table ; 
             // no number
           } //end if
           else { // not null
             temp->next = new city ;
             temp = temp->next;
             temp->name = new char[ strlen(name) + 1 ] ;
             strcpy( temp->name , name ) ;
             temp->next = NULL;

           } // else
           
           count++;
           fp >> name ;
     }
     sort_table( city_table ,count) ;
     /*
     for( int i = 1 ; i <= count ; i ++ )
     cout << num_to_name( i ) << "\t\t" << name_to_num( num_to_name( i ) ) << endl;
     */
     getflight() ;
}

bool getfile(){
     char fpname[200];
     printf("Enter the file name: \n");
     scanf( "%s", &fpname );
     //------------file name -----------------
    // strcpy( fpname,"C:\\Users\\a20010329\\Desktop\\DS_MID\\DS_input.txt") ;
     fp.open(fpname); ////////////////open file
     /////////////////////////ERROR CHECK ///////////////////
     if ( fp == NULL ) {
          printf("File \"%s\" isn't exist !\n",fpname ) ;
          return false;
     }
     
     char outfpname[200] ;
     outfpname[0] = '\0' ;
     strcpy( outfpname,fpname) ;
     strcat( outfpname,".out");
     fpout.open( outfpname ) ;
     
     return true ;
}



void print( cityptr head ) {
     if ( head == NULL )
     return ;
     printf( "%s\t\t%d\n",head->name,head->id );
     return print( head->next ) ;
}

void print( flightptr head ) {
     if ( head == NULL ) return ;
     print( head , head->des ) ;
     return print( head->next ) ;
}

void print( flightptr head , costptr chead ) {
     if ( chead ==NULL ) return ;
     printf( "%d\t\t",head->si_id );
     printf( "%d\t\t%d\t\t%d\n",chead->di_id,chead->flight_num,chead->cost1 );
     fpout << head->si_id << "\t\t" << chead->di_id << "\t\t" ;
     fpout << chead->flight_num << "\t\t" << chead->cost1 << endl ;
     return print( head , chead->next ) ;
}



int name_to_num( CharPtr name ) {
    int len;
    len = strlen( name ) ;
    if ( name[len - 1 ] == ',' ) 
      name[ len - 1 ] = '\0';
    return name_to_num1( city_table , name ) ;
}

int name_to_num1( cityptr head , CharPtr name ) {
    if ( head == NULL ) return 0 ;
    if ( strcmp( name , head->name ) == 0 )
      return head->id ;
    return name_to_num1( head->next , name ) ;
}

char *num_to_name( int num ) {
     return num_to_name1( city_table , num ) ;
}

char *num_to_name1( cityptr head ,int num ) {
     if ( head == NULL ) return NULL ;
     if ( head->id == num )
       return head->name ;
     return num_to_name1( head->next , num ) ;
         
}

void sort_table( cityptr head ,int count ) {
        if ( head == NULL ) return ;
        
        char *name1;
        city *walk ;
        city *goal ;
        goal = NULL ;
        for( int i = 1 ; i < count ; i ++ ) {
          for( walk = head ; walk->next != goal ; walk = walk->next ) {
            if ( strcmp( walk->name, walk->next->name ) > 0 ) {
             name1 = walk->name; // using charPtr not string
             walk->name = walk->next->name;
             walk->next->name = name1 ;
            } 
          } 
        
          goal = walk ;
        } // for
        
        walk = head ;
        for( int i = 1  ; i <= count ; i ++ ) {
             walk->id = i;
             walk = walk->next ;
        }
        
}

cityptr findbigest( cityptr head ) {
        if ( head == NULL ) return NULL;
        if ( head->next == NULL ) return head;
        if ( strcmp( head->name,findbigest( head->next )->name  ) > 0 ) 
        return head ;
        else return findbigest( head->next ) ;
}

void getflight(){
     str20 SI ;
     str20 DI ;
     int count = 0 ;
     int flight_num;
     int cost1 ;
     flight *tail_flight = NULL;
     cost *temp_cost = NULL;
     flight *temp_flight = NULL;
     // flight: had been eat 
     fp >> SI ;
     while ( strcmp( SI , "request:" ) != 0 ) {
           fp >> DI ;
           fp >> flight_num;
           fp >> cost1 ;
           if ( flight_table == NULL ) { // empty table && cost table must empty
             
             flight_table = new flight ;
             flight_table->si_id = name_to_num( SI ) ;
             
             flight_table->des = new cost ;
             flight_table->des->di_id = name_to_num( DI ) ;
             flight_table->des->cost1 = cost1 ;
             flight_table->des->flight_num = flight_num ;
             flight_table->des->next = NULL ;
             flight_table->next = NULL ;
             tail_flight = flight_table ; 
             

             
           } //end if
           else if ( newSI( name_to_num(SI) ) ) { //new element
             tail_flight->next = new flight ;
             tail_flight = tail_flight->next ;
             
             tail_flight->si_id = name_to_num( SI ) ;
             
             tail_flight->des = new cost ;
             tail_flight->des->di_id = name_to_num( DI ) ;
             tail_flight->des->cost1 = cost1 ;
             tail_flight->des->flight_num = flight_num ;
             tail_flight->des->next = NULL ;
             tail_flight->next = NULL ;
             
           } // new SI
           else { // SI had been saved
             temp_cost = new cost ; 
             temp_cost->di_id = name_to_num( DI ) ;
             temp_cost->cost1 = cost1 ;
             temp_cost->flight_num = flight_num ;
             temp_cost->next = NULL ;
             
             temp_flight = same_si_loc( name_to_num( SI ) );
             temp_cost->next = temp_flight->des ;
             temp_flight->des = temp_cost ;
           } // else
           
           count++;
           fp >> SI ;
     }
     
     // print( flight_table ) ;
     
     dealrequest() ;
}

bool newSI( int SI ) {
     
     return newSI1( flight_table , SI ) ;
}

bool newSI1( flightptr head , int SI ) {
     if ( head == NULL ) return 1 ; 
     if ( head->si_id == SI ) return 0 ;
     return newSI1( head->next , SI ) ;
}

flightptr same_si_loc( const int SI ) {
          return same_si_loc1( flight_table , SI ) ;
}

flightptr same_si_loc1( flightptr head , const int SI ) {
          if ( head == NULL ) return NULL;
          if ( head->si_id == SI ) return head ;
          return same_si_loc1( head->next , SI ) ;
}


void dealrequest(){
     str20 SIstr ;
     str20 DIstr ;
     int SI ;
     int DI ;
     while ( more_input() ) { 
           fp >> SIstr ;
           fp >> DIstr ;
           SI = name_to_num( SIstr ) ;
           DI = name_to_num( DIstr ) ;
           cout << "Request is to fly from " ;
           cout << clear_name( SIstr ) ; 
           cout << " to " << DIstr << "." << endl ;
           fpout << "Request is to fly from " ;
           fpout << clear_name( SIstr ) ; 
           fpout << " to " << DIstr << "." << endl ;
           
           if ( same_city( clear_name(SIstr),clear_name(DIstr) ) ) ;
           else if ( not_serve( SI , DI ) ) {
             if ( SI ==0 && DI == 0 ){
                  cout << "Sorry. HPAir does not server " << clear_name( SIstr ) ;
                  cout << " and " << clear_name( DIstr ) << ".\n" ;
                  fpout << "Sorry. HPAir does not server " << clear_name( SIstr ) ;
                  fpout << " and " << clear_name( DIstr ) << ".\n" ;
             }
             else if ( SI == 0 ) {
               cout << "Sorry. HPAir does not server " << clear_name( SIstr ) << ".\n" ;
               fpout << "Sorry. HPAir does not server " << clear_name( SIstr ) << ".\n" ;
             }
             else if ( DI == 0 ) {
               cout << "Sorry. HPAir does not server " << clear_name( DIstr ) << ".\n" ;
               fpout << "Sorry. HPAir does not server " << clear_name( DIstr ) << ".\n" ;
             }
             cout << endl ;
             fpout << endl ;
           }
           else if ( same_si_loc( SI ) == NULL ) {                
             cout << "Sorry. HPAir does not fly from " << clear_name( SIstr ) ;
             cout << " to " << clear_name( DIstr ) << endl << endl;
             fpout << "Sorry. HPAir does not fly from " << clear_name( SIstr ) ;
             fpout << " to " << clear_name( DIstr ) << endl << endl;
           }
           else {
                had_flight( SI ,DI ) ;
                if ( cheapest_stack != NULL )
                  printcheap() ;
                else if ( cheapest_stack == NULL ) {
                  cout << "Sorry. HPAir does not fly from " << clear_name( SIstr ) ;
                  cout << " to " << clear_name( DIstr ) << endl << endl;
                  fpout << "Sorry. HPAir does not fly from " << clear_name( SIstr ) ;
                  fpout << " to " << clear_name( DIstr ) << endl << endl;
                }
           }
           
           cheapest_stack = NULL ;
           stack = NULL ;
           //cout << name_to_num( SI ) << "   " << name_to_num( DI ) << endl;
     }
     
     fp.close();
     fpout.close();
}

bool more_input() {
     while ( ( fp.peek() == '\n' || 
               fp.peek() == '\t' || 
               fp.peek() == ' ' ||
               fp.peek() == '\f' ||
               fp.peek() == '\r' ||
               fp.peek() == '\v' ) 
             ) fp.get() ;
     if ( fp.peek() == EOF ) return 0 ;
     else return 1 ;
}


bool same_city( CharPtr SI,CharPtr DI ) {
     if ( strcmp( SI,DI ) == 0 ) {
     cout << "Sorry. HPAir does not fly from " ;
     cout << SI << " to " << DI << endl << endl;
     fpout << "Sorry. HPAir does not fly from " ;
     fpout << SI << " to " << DI << endl << endl;
     return 1 ;
     } 
     return 0 ;
}


bool not_serve( int SI,int DI ) {
     if ( SI == 0 || DI == 0 ) return 1 ;
     return 0 ;
}



char *clear_name( str20 name ) {
     char *name1 = new char[ strlen(name) + 1 ] ;
     strcpy( name1,name ) ;
     if ( name1[ strlen(name) - 1 ] == ',' ) 
       name1[ strlen(name) -1 ] = '\0';
       
     return name1;
}

void push( int id ) {
     if ( stack == NULL ) {
          stack = new Stack ;
          stack->id = id ;
          stack->next = NULL ;
          return ;
     }
     
     Stack *temp = new Stack ;
     temp->id = id ;
     temp->next = stack ;
     stack = temp ;
}

int pop() {
    if ( stack == NULL ) return 0 ;
    Stack *temp = stack;
    stack = stack->next ;
    int id = temp->id ;
    delete temp ;
    return id ;
}

int pop( int & id ) {
    if ( stack == NULL ) return 0 ;
    Stack *temp = stack;
    stack = stack->next ;
    id = temp->id ;
    delete temp ;
    return id ;
}

bool SI_in_stack( int SI ) {    
     return SI_in_stack1( stack,SI ) ;
}

bool SI_in_stack1( stackptr head ,int SI ) {
     if ( head == NULL ) return 0;
     if ( head->id == SI ) return 1 ;
     return SI_in_stack1( head->next,SI ) ;
}
/*
bool had_flight( int SI , int DI ) {
     if ( SI_in_stack( SI ) ) return 0 ;
     if ( SI == DI ) {
          push( SI ) ; 
          get_cheapst() ;
          pop() ;
          return 1 ;          
     }
     if ( same_si_loc( SI ) == NULL ) {
       return 0 ;
     }
     
     bool had_flight1 = 0 ;
     push( SI ) ;
     
     
     costptr des = same_si_loc( SI )->des ; 
     
     
     for ( costptr walk = des ; walk != NULL ; walk = walk->next ) {
         if ( same_si_loc( walk->di_id ) != NULL ) {
           if ( had_flight( walk->di_id ,DI ) )
             had_flight1 = 1 ;
         }
         
     
     }
     
     
     if ( had_flight1 = 1  ) { 
          pop();
          return 1 ;
     }
     
     pop() ;
     return 0 ;
     
     
}
*/


bool had_flight( int SI , int DI ) {
     if ( stack == NULL ) {
          push( SI ) ;
          costptr des = same_si_loc( SI )->des ; 
     
     for ( costptr walk = des ; walk != NULL ; walk = walk->next ) {
              if ( walk->di_id == DI ) {
              push( DI );
              get_cheapst() ;
              pop() ; 
              }
     }
          
          
     for ( costptr walk = des ; walk != NULL ; walk = walk->next ) {
       // cout << walk->di_id << "!" << endl ;
       if ( same_si_loc( walk->di_id ) != NULL ) 
         had_flight( walk->di_id ,DI ) ;
     }
          
     
         if ( cheapest_stack == NULL )
           return 0 ;
         return 1 ;
     }
     
     if ( SI_in_stack( SI ) ) return 0 ;
     if ( SI == DI ) {
          push( SI ) ; 
          get_cheapst() ;
          pop() ;
          return 1 ;          
     }
     if ( same_si_loc( SI ) == NULL ) {
       return 0 ;
     }
     
     bool had_flight1 = 0 ;
     push( SI ) ;
     
     
     costptr des = same_si_loc( SI )->des ; 
     
     for ( costptr walk = des ; walk != NULL ; walk = walk->next ) {
              if ( walk->di_id == DI ) {
              push( DI );
              get_cheapst() ;
              pop() ;
              }
     }
     
     for ( costptr walk = des ; walk != NULL ; walk = walk->next ) {
         if ( same_si_loc( walk->di_id ) != NULL ) {
           if ( had_flight( walk->di_id ,DI ) )
             had_flight1 = 1 ;
         }
         
     
     }
     
     
     if ( had_flight1 == 1  ) { 
          pop();
          return 1 ;
     }
     
     pop() ;
     return 0 ;
     
     
}


void get_cheapst() {
     if ( cheapest_stack == NULL ) {
          copy( cheapest_stack , stack ) ;
          return ;
     }
     
     if ( get_total( cheapest_stack ) > get_total( stack ) ) {
        copy( cheapest_stack , stack ) ;
     }
     
} 


void print( stackptr head ) {
     if ( head == NULL ) return ;
     cout << head->id << "->" ;
     return print( head->next ) ;
     
}

void printcheap() {
     int total = 0;
     printcheap1( cheapest_stack ,total ) ;
     cout << "Total Cost .........................................  $";
     cout << total << endl << endl ;
     fpout << "Total Cost .........................................  $";
     fpout << total << endl << endl ;
}

void printcheap1( stackptr head ,int &total ) {
     if ( head == NULL ) return ;
     if ( head->next == NULL ) return ;
     printcheap1( head->next ,total) ;
     cout << "Flight #" << get_flight_num( head->next->id ,head->id) ;
     cout << " from " << num_to_name( head->next->id ) << " to " ;
     cout << num_to_name( head->id ) << "\t\tCost: $" ;
     cout << get_cost( head->next->id ,head->id) << endl ;
     
     fpout << "Flight #" << get_flight_num( head->next->id ,head->id) ;
     fpout << " from " << num_to_name( head->next->id ) << " to " ;
     fpout << num_to_name( head->id ) << "\t\tCost: $" ;
     fpout << get_cost( head->next->id ,head->id) << endl ;
     
     total += get_cost( head->next->id ,head->id) ;
     
}

int get_flight_num( int SI , int DI ) {
    for ( costptr walk = same_si_loc( SI )->des ; walk != NULL ; walk = walk->next )
      if ( DI == walk->di_id ) return walk->flight_num ;
      
    return 0 ;
}

int get_cost( int SI , int DI ) {
    for ( costptr walk = same_si_loc( SI )->des ; walk != NULL ; walk = walk->next )
      if ( DI == walk->di_id ) return walk->cost1 ;
      
    return 0 ;
}

int get_total( stackptr head ) {
    if ( head == NULL ) return 0 ;
    if ( head->next == NULL ) return 0 ;
    return get_cost( head->next->id ,head->id) + get_total( head->next ) ;
}


void copy( stackptr &copied , stackptr coping ) {
     del( copied );
     copy1( copied , coping ) ;
}

void copy1( stackptr &copied , stackptr coping ) {
     if ( coping == NULL ) return ;
     copied = new Stack ;
     copied->id = coping->id ;
     copied->next = NULL ;
     copy1(  copied->next , coping->next ) ;
}

void del( stackptr &head ) {
     if ( head == NULL ) return ;
     Stack *temp = head;
     head = head->next ;
     delete( temp ) ;
     del( head );
}







