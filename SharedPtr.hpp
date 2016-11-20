#ifndef SHAREDPTR
#define SHAREDPTR
#include<stdlib.h>
#include<iostream>
namespace cs540{
template <typename T>
class smart_pointer{	
		public:
		T 		*ptr;
		int 		*ref_count;
	};	
template <typename T> 
class SharedPtr{
//	private:
	public:
		smart_pointer<T> * data;
	public:
		SharedPtr();
		SharedPtr(const SharedPtr &p);
		template<typename U> explicit SharedPtr(U *ptr_to_obj){	
			data 			= new smart_pointer<U>();
			data->ptr		= ptr_to_obj;
			data->ref_count 	= new int();	
			*(data->ref_count)	= 1;
		}
		SharedPtr(SharedPtr &&p);
		template <typename U> SharedPtr(const SharedPtr<U> &p);
		template <typename U> SharedPtr(SharedPtr<U> &&p);
		SharedPtr &operator=(const SharedPtr &p);
		template <typename U> SharedPtr& operator=(SharedPtr<U> &&p);
		SharedPtr &operator=(const SharedPtr &&p);
		template <typename U> SharedPtr<T> & operator = (const SharedPtr<U> &p);
		~SharedPtr();
};

/*
template<typename T> template<typename U> SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<U> &&p){

	data = p.data;
	p.data = nullptr;//delete p data?
	return *this;
}
template<typename T> SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T> &p){

	if(data != nullptr){
		*(data->ref_count) -=1;
		//should i delete memory if ref_count < 0 ??
	}
	if(p.data!=nullptr){
		*(p.data->ref_count) +=1;
	}
	data = p.data;
	return *this;
}
template<typename T> template<typename U> SharedPtr<T>::SharedPtr(const SharedPtr<U> &p){

//	SharedPtr<T>
//	std::cout<<"oooooooooooooooooooooooops";
//	SharedPtr<U> * temp = const_cast<SharedPtr<U> *>(&p);
//	data = dynamic_cast<smart_pointer<T> *>(temp->data);
//	this = temp;
//	data = p.data;
//	(data->ref_count)+=1;
	data  =  new smart_pointer<T>;
	data->ptr = p.data->ptr;
	data->ref_count = p.data->ref_count;	
	 

}
template<typename T>template <typename U> SharedPtr<T>:: SharedPtr(SharedPtr<U> &&p){
	data	= p.data;
	p.data	= nullptr;//should we call destructor
}
template<typename T>SharedPtr<T>::SharedPtr(SharedPtr &&p){
	data 	= p.data;
	p.data	= nullptr;//should we call destructor
}
//empty constructor
template<typename T>SharedPtr<T>::SharedPtr(){
	data 			= new smart_pointer<T>();
	data->ptr		= nullptr;
	data->ref_count 	= new int();	
	*(data->ref_count)	= 1;
}
template<typename T>SharedPtr<T>::SharedPtr(const SharedPtr &p){
	//constructing one shared pointer from other
	if(p.data!=NULL){
		data = p.data;
		*(data->ref_count)+=1;
	}	
	else{
		//create memory and allocate
	}
}
template<typename T>SharedPtr<T>::~SharedPtr(){
	if(data!=NULL){
		if(data->ref_count!=NULL)
			*(data->ref_count)-=1;
		if(( *(data->ref_count)) <1){
//			printf("hmm delete the pointer");
			delete(data->ref_count);
			if(data->ptr!=NULL)
				delete(data->ptr);
			delete(data);
			}
		}
	}
}*/
#endif
