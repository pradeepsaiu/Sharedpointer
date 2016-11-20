#ifndef DEQHPP
#define DEQHPP
//to  get pair
#include<stdlib.h>
#include <utility> 
#include <stdexcept>
#include <iostream>
namespace cs540{

#define MAX_NUM_LEVELS 100
//this should always be greater than 0
#define INITIAL_HEIGHT 64 

template <typename Key_T, typename Mapped_T>
class skip_list{
        public:
		class node{
			public:
				std::pair< Key_T, Mapped_T> data;
				node *next,*prev,*top,*down;
		};
		//Meta data about the skip list
		int 	total_levels;
		int	current_height;
		int 	height_index;//points to current max head.
		/*To keep track of number of nodes in each level*/
		size_t	count_level[MAX_NUM_LEVELS];
		/*To keep track of first and last node at each level*/
		node *head[MAX_NUM_LEVELS],*tail[MAX_NUM_LEVELS],*temp_head[MAX_NUM_LEVELS];
		/*To initialize the skip list structure 4 nodes pointing to null*/
		skip_list(){
			//to help generate random numbers in different runs.
			//srand(time(NULL));
			total_levels 			= MAX_NUM_LEVELS;
			current_height 			= INITIAL_HEIGHT;
			height_index			= 0;
		//	head[current_height-1] 		= new node();	
			int i = current_height-1;//zero indexing.
			//i = 0 being the main list where all elements are present.
			while(i>=0){
				head[i] 	= new node();
				temp_head[i]	= head[i];
				head[i]->next 	= NULL;	head[i]->prev 	= NULL;
				head[i]->top  	= NULL;	head[i]->down 	= NULL;
				count_level[i]  = 0;
				i--;
			}
			i=0;
		// Initializing the head list of elements
			while(i<current_height){
				if(i == current_height-1){
					head[i]->down = head[i-1];
		}
				if(i!=0){
					head[i]->down = head[i-1];
					head[i]->top  = head[i+1];
				}
				else{
					head[i]->top  = head[i+1];
				}
				i++;
			}
		}
		std::pair<node *,bool> insert( std::pair<const Key_T, Mapped_T> data){
//			std::cout<<"\n###################\n";
		//	std::cout<<data.first<<data.second<<std::endl;	
			srand(time(NULL));
			node *to_insert 	= new node();			
			node *base;
			to_insert->data 	= data;
			int temp_level		= height_index;
			node *start	 	= head[height_index];

			while( temp_level > 0){
//			Traverse to the level 0 where element needs to be inserted
				if(start->next == NULL || data.first < start->next->data.first){
//					std::cout<<"\n^^^1\n";
					temp_head[temp_level] = start;
					temp_level -= 1;
					start = start->down;
					continue;
				}
//					std::cout<<"\n^^^2\n";
					start = start->next;	
			}			
			/*inserting at the end of the linked list or first node of the skip_list*/
			if(start->next == NULL){
	//			std::cout<<head[0]<<"-----------------------";
//				std::cout<<"\n^^^3\n";
				start->next  	= to_insert;
				to_insert->prev	= start;
				tail[0]		= to_insert;
				count_level[0] += 1;	
				return(std::make_pair(start->next,true));
			}
			else{
//				std::cout<<"\n^^^4\n";
			//traverse and find where to insert the element
				while(start->next!=NULL && start->next->data.first<  data.first){
					start = start->next;//	std::cout<<"\n^^^5\n";
				}
				if(start->next == NULL){
				}
				else if(data.first == start->next->data.first){
				//	std::cout<<"\n^^^6\n";
					//returns false for duplicate value.
					return(std::make_pair(start->next,false));
				}else{}
//				std::cout<<"\n^^^7\n";
				//inserting at level zero always
				to_insert->prev = start;
//				if(start->next == NULL){tail[0]=to_insert;}
				to_insert->next = start->next;
				start->next 	= to_insert;
				base		= to_insert;
				count_level[0]++;
				//now randomly check whether to promote up or not.
				//promote randomly only for random numbers
				int coin_flip = rand()%2, count=1;
//				std::cout<<coin_flip;
				//promote if coin_flip = 1;
				while(coin_flip == 1 && count < current_height){
				//	std::cout<<"\n^^^7\n";
					node *to_insert1 	= new node();			
					to_insert1->data 	= data;
					to_insert1-> prev 	= temp_head[count];
					to_insert1-> next	= temp_head[count]->next;
					//by default top = NULL
					to_insert1-> down	= base;
					base->top		= to_insert1;
					base			= to_insert1;

					temp_head[count]->next	= to_insert1;

					count_level[count]++;	
					coin_flip 		= rand()%2;
					if(count > height_index){
						height_index = count;
					}
					//needn't insert higher, this is at highest peak
					if(count_level[count]==1){break;}
					count++;		
				}

				return(std::make_pair(start->next,true));
			}
		}

		void print(){
			node * traverse  = head[height_index];
			int level	 = height_index;
			std::cout<<"\nTotal height = "<<height_index<<"\n";
			std::cout<<"\n####"<<level<<"####\n";
			while(traverse != NULL){
				std::cout<<traverse->data.first<<traverse->data.second<<"==>";
				traverse = traverse->next;
				if(traverse == NULL and level != 0){
					std::cout<<"\n####"<<level-1<<"####"<<count_level[level-1]<<std::endl;
					level--;
					traverse = head[level];
				}
			}
				
		}
};


//declaring class with key value pair template
template <typename Key_T, typename Mapped_T>
class Map{
	public:
//don't change the order here
	typedef Key_T			 	 		KeyType;
	typedef Mapped_T			 		MapType;
	typedef skip_list<Key_T,Mapped_T>	 		skiptype;
	typedef std::pair< Key_T, Mapped_T> 			ValueType;

	class Iterator:public Map{
	public:
		typename skiptype::node  *point;
		Iterator(typename skiptype::node *val){
			point = val;
		}		
	
		Iterator &operator++(int){
			Iterator *ret1 = new Iterator(this->point);
			if(this->point!=NULL)
			this->point = this->point->next;
			return *ret1;
		}
		Iterator &operator++(){
			if(this->point!=NULL)
			this->point = this->point->next;
			return *(this);
		}
		Iterator &operator--(int){
			Iterator *ret1 = new Iterator(this->point);
			if(this->point!=NULL)
			this->point = this->point->prev;
			return *ret1;
		}
		Iterator &operator--(){
			if(this->point!=NULL)
			this->point = this->point->prev;
			return *(this);
		}
		ValueType &operator*() const{
			return this->point->data;	
		}
		ValueType *operator->() const{
			return &(this->point->data);
		}
	};
	class ReverseIterator:public Map{
	public:
		typename skiptype::node  *point;
		ReverseIterator(typename skiptype::node *val){
			point = val;
		}		
	
		ReverseIterator &operator++(int){
			ReverseIterator *ret1 = new ReverseIterator(this->point);
			if(this->point!=NULL)
			this->point = this->point->prev;
			return *ret1;
		}
		ReverseIterator &operator++(){
			if(this->point!=NULL)	
			this->point = this->point->prev;
			return *(this);
		}
		ReverseIterator &operator--(int){
			ReverseIterator *ret1 = new ReverseIterator(this->point);
			if(this->point!=NULL)
			this->point = this->point->next;
			return *ret1;
		}
		ReverseIterator &operator--(){
			if(this->point!=NULL)
			this->point = this->point->next;
			return *(this);
		}
		ValueType &operator*() const{
			return this->point->data;	
		}
		ValueType *operator->() const{
			return &(this->point->data);
		}
	};

	class ConstIterator:public Map{
	public:
		typename skiptype::node  *point;
		ConstIterator(typename skiptype::node *val){
			point = val;
		}		
	
		ConstIterator &operator++(int){
			ConstIterator *ret1 = new Iterator(this->point);
			if(this->point!=NULL)
			this->point = this->point->next;
			return *ret1;
		}
		ConstIterator &operator++(){
			if(this->point!=NULL)
			this->point = this->point->next;
			return *(this);
		}
		ConstIterator &operator--(int){
			ConstIterator *ret1 = new Iterator(this->point);
			if(this->point!=NULL)
			this->point = this->point->prev;
			return *ret1;
		}
		ConstIterator &operator--(){
			if(this->point!=NULL)
			this->point = this->point->prev;
			return *(this);
		}
		ValueType &operator*() const{
			return this->point->data;	
		}
		ValueType *operator->() const{
			return &(this->point->data);
		}
	};

	skip_list<Key_T,Mapped_T> sk_list;
	Iterator begin(){
		return Iterator(sk_list.head[0]->next);
	}
	ConstIterator begin() const{
		return ConstIterator(Iterator(sk_list.head[0]->next));
	}
	ConstIterator end() const{
		return ConstIterator(Iterator(NULL));
	}	
	Iterator end(){
		return Iterator(NULL);
	}
	ReverseIterator rbegin(){
		typename skiptype::node *it1 = sk_list.head[0];
		while(it1->next !=NULL) it1 = it1->next;
		return ReverseIterator(it1);
	}
	ReverseIterator rend(){
		return ReverseIterator(sk_list.head[0]);
	}
	void clear(){
		int i = sk_list.current_height-1;//zero indexing.
		//first remove all elements
		typename skiptype::node *temp,*temp1;	
		while(i>=0){
			temp = sk_list.head[i]->next; 	
			sk_list.head[i]->next = NULL;
			while(temp!=NULL){
				temp1 = temp;
				temp = temp->next;
				delete temp1;
			}
			i--;
		}
		for(int i=sk_list.height_index;i>=0;i--)		
			sk_list.count_level[i] = 0;
	
		sk_list.height_index = 0;	
	}

	size_t size() const{
		return sk_list.count_level[0];
	}
	bool empty() const{
		return (sk_list.count_level[0] == 0);
	}
	Mapped_T &at(const Key_T & data_key){
		//what if it's a empty map.
			int temp_level		= sk_list.height_index;
			typename skiptype::node * start	= sk_list.head[sk_list.height_index];
			if(sk_list.count_level[0]==0){
				throw std::out_of_range("Indexing empty map :/");
			}
			while( temp_level > 0){
//			Traverse to the level 0 where element needs to be inserted
				if(start->next == NULL || data_key < start->next->data.first){
					temp_level -= 1;
					start = start->down;
					continue;
				}
				start = start->next;
			}			
			/*inserting at the end of the linked list or first node of the skip_list*/
			if(start->data.first == data_key){
				if(start == sk_list.head[0]) throw std::out_of_range("Indexing empty map :...");
				return (start->data.second);
			}
			else{
			//traverse and find in case it's only in root
				while(start->next !=NULL && start->next->data.first <  data_key){
					start = start->next;
				}
				if(start->next != NULL && start->next->data.first == data_key){
					return (start->next->data.second);
				}
				else{
					throw std::out_of_range ("Index not in Map");
				}
			}
	}
	const Mapped_T &at(const Key_T & data_key) const{
/*			Iterator *ret1 = new Iterator(this->point);
			this->point = this->point->next;
			return *ret1;*/
		//what if it's a empty map.
			int temp_level		= sk_list.height_index;
			typename skiptype::node * start	= sk_list.head[sk_list.height_index];
//			const Mapped_T *ob1;
	//		const Mapped_T * ob_pointer = new Mapped_T();
			if(sk_list.count_level[0]==0){
				throw std::out_of_range("Indexing empty map :/");
			}
			while( temp_level > 0){
//			Traverse to the level 0 where element needs to be inserted
				if(start->next == NULL || data_key < start->next->data.first){
					temp_level -= 1;
					start = start->down;
					continue;
				}
				start = start->next;
			}			
			/*inserting at the end of the linked list or first node of the skip_list*/
			if(start->data.first == data_key){
				if(start == sk_list.head[0]) throw std::out_of_range("Indexing empty map :...");
				const Mapped_T *ob1 =  new Mapped_T(start->data.second) ;
				return *ob1;
			}
		else{
			//traverse and find in case it's only in root
				while(start->next !=NULL && start->next->data.first <  data_key){
					start = start->next;
				}
				if(start->next != NULL && start->next->data.first == data_key){
					const Mapped_T  *ob1 = new Mapped_T( start->next->data.second);
					return *ob1;
				}
				else{
					throw std::out_of_range ("Index not in Map");
				}
			}
	}
	Iterator find(const Key_T & data_key){
		//what if it's a empty map.
			int temp_level		= sk_list.height_index;
			typename skiptype::node * start	= sk_list.head[sk_list.height_index];
			if(sk_list.count_level[0]==0){
				return Iterator(NULL);
//				throw std::out_of_range("Indexing empty map :/");
			}
			while( temp_level > 0){
//			Traverse to the level 0 where element needs to be inserted
				if(start->next == NULL || data_key < start->next->data.first){
					temp_level -= 1;
					start = start->down;
					continue;
				}
				start = start->next;
			}			
			if(start->data.first == data_key){
				if(start == sk_list.head[0]) throw std::out_of_range("Indexing empty map :/");
				return Iterator(start);
			}
			else{
			//traverse and find in case it's only in root
				while(start->next !=NULL && start->next->data.first <  data_key){
					start = start->next;
				}
				if(start->next != NULL && start->next->data.first == data_key){
					return Iterator(start->next);
				}
				else{
					return Iterator(NULL);
//					throw std::out_of_range ("Index not in Map");
				}
			}	
	}
	Mapped_T &operator[](const Key_T & key_val){
		try{
			return(at(key_val));
		}
		catch (const std::out_of_range& e){
			insert(std::make_pair(key_val,Mapped_T()));
			return(at(key_val));
		}
	}
	Map &operator=(const Map & ob1){
		typename skiptype::node *it1 = ob1.sk_list.head[0]->next;
		clear();
		while(it1 !=NULL){
			insert(it1->data);
			it1 = it1->next;
		}
		return *this;
	}

	Map(){	
	}
	Map(std::initializer_list<std::pair<const Key_T, Mapped_T> >  list_args){	
		auto start_ptr = list_args.begin();

		while(start_ptr != list_args.end())
		{
			sk_list.insert(std::make_pair(std::get<0>(*start_ptr),std::get<1>(*start_ptr)));			
			start_ptr++;
		}
	}
	Map(const Map & copy_obj){
		typename skiptype::node *traverse = copy_obj.sk_list.head[0]->next; 
		while(traverse!=NULL){
			insert(traverse->data);
			traverse = traverse->next;
		}
	}
	~Map(){
			int i = sk_list.current_height-1;//zero indexing.
			//i = 0 being the main list where all elements are present.
			typename skiptype::node *temp,*temp1;	
			while(i>=0){
				temp = sk_list.head[i]; 	
				while(temp!=NULL){
					temp1 = temp;
					temp = temp->next;
					delete temp1;
				}
				i--;
			}
	}
	void erase(Iterator pos){
		typename skiptype::node * temp=pos.point,*temp1;
		int level = 0;
		while(temp!=NULL){
			temp->prev->next = temp->next;
			temp1		= temp;
			temp		= temp->top;
			delete temp1;
			sk_list.count_level[level++]-=1;
		}
	}
	void erase(const Key_T & key_val){
		erase(find(key_val));
	}
	std::pair<Iterator, bool> insert(const ValueType & insert_value){

		typename skiptype::node * ret_iterator;	bool flag;

		auto ret = std::make_pair(ret_iterator,flag);
		ret	 = sk_list.insert(insert_value);

		Iterator it(ret.first);
		flag = ret.second;

		return (std::make_pair(it,flag));
	}
	/*Element access funcitons*/
	//Mapped_T &at(const Key_T &){
	//}
};
	template<typename Key_T,typename Mapped_T> 
	bool operator==(const Map<Key_T,Mapped_T> & ob1, const Map<Key_T,Mapped_T> & ob2){
		if(ob1.size() != ob2.size()) return false;
		else{
			auto traverse_ob1 = ob1.sk_list.head[0]->next;
			auto traverse_ob2 = ob2.sk_list.head[0]->next;
			
			while(traverse_ob1!=NULL and traverse_ob2!=NULL){
				if(traverse_ob1->data!=traverse_ob2->data)
					return false;
				traverse_ob1 = traverse_ob1->next;
				traverse_ob2 = traverse_ob2->next;
			}
			return true;
		}
	}
	
	template<typename Key_T,typename Mapped_T> 
	bool operator!=(const Map<Key_T,Mapped_T> & ob1, const Map<Key_T,Mapped_T> & ob2){
		return !(ob1 == ob2);
	}

	template<typename Key_T,typename Mapped_T> 
	bool operator<(const Map<Key_T,Mapped_T> & ob1, const Map<Key_T,Mapped_T> & ob2){
		auto traverse_ob1 = ob1.sk_list.head[0]->next;
		auto traverse_ob2 = ob2.sk_list.head[0]->next;	
		while(traverse_ob1!=NULL and traverse_ob2!=NULL){
			if(traverse_ob1->data > traverse_ob2->data)
				return false;
			traverse_ob1 = traverse_ob1->next;
			traverse_ob2 = traverse_ob2->next;
		}
		if(ob1.size() < ob2.size())
			return true;
		return false;
	}
	template<typename Key_T,typename Mapped_T> 
	bool operator==(const typename Map<Key_T,Mapped_T>::Iterator & ob1,const typename Map<Key_T,Mapped_T>::Iterator & ob2){
		return (ob1.point->data == ob2.point->data);
	}
	template<typename Key_T,typename Mapped_T> 
	bool operator==(const typename Map<Key_T,Mapped_T>::ConstIterator & ob1,const typename Map<Key_T,Mapped_T>::ConstIterator & ob2){
		return (ob1.point->data == ob2.point->data);
	}
	template<typename Key_T,typename Mapped_T> 
	bool operator==(const typename Map<Key_T,Mapped_T>::Iterator & ob1,const typename Map<Key_T,Mapped_T>::ConstIterator & ob2){
		return (ob1.point->data == ob2.point->data);
	}
	template<typename Key_T,typename Mapped_T> 
	bool operator==(const typename Map<Key_T,Mapped_T>::ConstIterator & ob1,const typename Map<Key_T,Mapped_T>::Iterator & ob2){
		return (ob1.point->data == ob2.point->data);
	}
	template<typename Key_T,typename Mapped_T> 
	bool operator==(const typename Map<Key_T,Mapped_T>::ReverseIterator & ob1,const typename Map<Key_T,Mapped_T>::ReverseIterator & ob2){
		return (ob1.point->data == ob2.point->data);
	}
	template<typename Key_T,typename Mapped_T> 
	bool operator!=(const typename Map<Key_T,Mapped_T>::Iterator & ob1,const typename Map<Key_T,Mapped_T>::Iterator & ob2){
		return !(ob1==ob2);
	}
	template<typename Key_T,typename Mapped_T> 
	bool operator!=(const typename Map<Key_T,Mapped_T>::ConstIterator & ob1,const typename Map<Key_T,Mapped_T>::ConstIterator & ob2){
		return !(ob1==ob2);
	}
	template<typename Key_T,typename Mapped_T> 
	bool operator!=(const typename Map<Key_T,Mapped_T>::Iterator & ob1,const typename Map<Key_T,Mapped_T>::ConstIterator & ob2){
		return !(ob1==ob2);
	}
	template<typename Key_T,typename Mapped_T> 
	bool operator!=(const typename Map<Key_T,Mapped_T>::ConstIterator & ob1,const typename Map<Key_T,Mapped_T>::Iterator & ob2){
		return !(ob1==ob2);
	}
	template<typename Key_T,typename Mapped_T> 
	bool operator!=(const typename Map<Key_T,Mapped_T>::ReverseIterator & ob1,const typename Map<Key_T,Mapped_T>::ReverseIterator & ob2){
		return !(ob1==ob2);
	}
}
#endif
