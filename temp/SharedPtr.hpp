#ifndef SHAREDPTR
#define SHAREDPTR
#include<stdlib.h>
#include<iostream>
#include<assert.h>
namespace cs540{
template <typename T>
class smart_pointer{	
		public:
		T 		*ptr;
		int 		*ref_count;
	/*	smart_pointer(T *ptr1,int *ref1){
			ptr = ptr1;
			ref_count = ref1;
		}
		smart_pointer(){
			ptr=nullptr;
			ref_count=nullptr;
		}*/
	};	
template <typename T> 
class SharedPtr{
//	private:
	public:
		smart_pointer<T> * data;
	public:
		SharedPtr(){
			std::cout<<"1. Empty Constructor-------------------------------- \n";
			data = nullptr;
		}
	/*	SharedPtr(smart_pointer<T> *d){
			data=d;
		}*/
		SharedPtr(const SharedPtr &p){
//			std::cout<<"1.b Copy Constructor---------------------------------\n"<<std::endl;
			data = new smart_pointer<T>();
			data->ptr = p.data->ptr;
			(*p.data->ref_count)+=1;
			data->ref_count = p.data->ref_count;
	//		std::cout<<*data->ref_count<<"_____";
		}
		template<typename U> explicit SharedPtr(U *ptr_to_obj){	
			std::cout<<"2. Explicit constructor------------------------------\n";
			data 			= new smart_pointer<T>();
			data->ptr		= ptr_to_obj;
			data->ref_count 	= new int(1);	
	//		std::cout<<*(data->ref_count)<<"++++++";
		}
	/*	SharedPtr(SharedPtr &&p){
			std::cout<<"3 .Move the managed object---------------------------\n";

		}*/
		template <typename U> SharedPtr(const SharedPtr<U> &p){
//			std::cout<<"4.Copy Assignment------------------------------------\n";
			if(p.data!=nullptr){
				data = new smart_pointer<T>();
				(*(p.data->ref_count))++;
			}
			data->ptr = p.data->ptr;
			data->ref_count = p.data->ref_count;
		}
//		template <typename U> SharedPtr(SharedPtr<U> &&p){
//			std::cout<<"5. Move the managed object---------------------------\n";
//		}
		SharedPtr &operator=(const SharedPtr &p){
			std::cout<<"6. Operator equal------------copy from same type------------------------\n";
			if(p.data!=nullptr){
				if(data==nullptr){
					data		= new smart_pointer<T>();
					data->ptr	= p.data->ptr;
					data->ref_count = p.data->ref_count;
					(*p.data->ref_count)+=1;
					return * this;
				}
				*(data->ref_count) -=1;
//				std::cout<<*(data->ref_count)<<"+++++++++++++++++++";
//				assert((*data->ref_count)<0 );
				if(data->ref_count == 0){
					delete(data->ptr);
				}
				data->ptr 		= p.data->ptr;
				*(p.data->ref_count)+=1;
				data->ref_count 	= p.data->ref_count;	
			}
			else{
				assert(0&&"undefined in operator = copy form the same");
			}
			return *this;
		}
		template <typename U> SharedPtr& operator=(SharedPtr<U> &&p){
			std::cout<<"7. operator = template u &&& move instruction-------------------------\n";


		}
		SharedPtr &operator=(const SharedPtr &&p){
			std::cout<<"8. double && operator =------------------------------\n";
		}
		template <typename U> SharedPtr<T> & operator = (const SharedPtr<U> &p){
			std::cout<<"9. operator = copying from a different type-------------------\n";
			if(p.data!=nullptr){
				if(data == nullptr){
					data 		= new smart_pointer<T>();
					data->ptr	= p.data->ptr;
					data->ref_count = p.data->ref_count;
					*(p.data->ref_count)+=1;
					return *this;
				}
				*(data->ref_count) -=1;
			//	assert(*(data->ref_count)<0 &&"weirdo error in operator =");
				if((*data->ref_count) == 0){
					delete(data->ptr);
				}
				data->ptr 		= p.data->ptr;
				*(p.data->ref_count)+=1;
				data->ref_count 	= p.data->ref_count;	

			}
			else{
				assert(0&&"Uncertain behaviour in = from a different type.");
			}
			return *this;
		}
	
		void reset(){
			if(data!=nullptr){
				if((*data->ref_count)==1){
					delete(data->ptr);
					delete(data->ref_count);
					delete(data);			
					data=nullptr;
				}
				else{
					(*data->ref_count) -=1;
					delete(data);
					data = nullptr;
					
				}
			}
		}
		template <typename U> void reset(U *p){
			if(data!=nullptr){
				if((*data->ref_count)==1){
					delete(data->ref_count);
					delete(data->ptr);
					delete(data);
				}
				else{
					(*data->ref_count)-=1;
					delete(data);
				}
			}
			data = new smart_pointer<T>();
			data->ptr = p;
			data->ref_count = new int(1);

			
		}
		explicit operator bool() const{
			if(data!=nullptr)
				if(data->ptr!=nullptr)
					return true;
			return false;
		}
		T *get() const{
			if(data!=nullptr)
				return data->ptr;
			return nullptr;
		}
		T &operator*() const{
			return (*data->ptr);
		}
		T *operator->()const{
			return data->ptr;
		}
		~SharedPtr(){
//			std::cout<<"10. Destructor--------------------------------------begin\n";	
			if(data!=nullptr){
				if(*(data->ref_count)==1){
				//	std::cout<<*(data->ref_count)<<"========\n";
					delete(data->ref_count);
					delete(data->ptr);
				}
				else{
					(*data->ref_count-=1);
				}
				delete(data);	
			}
		}	
};
//compare two shared pointers
template<typename T1,typename T2>
bool operator==(const SharedPtr<T1> &p,const SharedPtr<T2> &p2){
	if(p.data==nullptr && p2.data==nullptr){
		return true;}
	else if(p.data==nullptr || p2.data==nullptr){
		return false;}
	else{
		return (p.data->ptr == p2.data->ptr);}
}
template<typename T>
bool operator==(const SharedPtr<T> &p,std::nullptr_t){
	return (p==SharedPtr<T>() );
}
template<typename T>
bool operator==(std::nullptr_t,const SharedPtr<T> &p){
	return p==nullptr;
}
template<typename T1, typename T2>
bool operator!=(const SharedPtr<T1>&p1,const SharedPtr<T2>&p2){
	return !(p1==p2);
}

template<typename T,typename U>
bool operator!=(const SharedPtr<T> &p1,std::nullptr_t){
	return !(p1==nullptr);
}
template<typename T,typename U>
bool operator!=(std::nullptr_t,const SharedPtr<T> &p1){
	return !(p1==nullptr);
}
template<typename T,typename U>
SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp){

/*	SharedPtr<T> ret;
	ret.data   		= new smart_pointer<T>();
	ret.data->ptr 		= static_cast<T *>(sp.data->ptr);
	ret.data->ref_count 	= sp.data->ref_count;
	return ret;
*/	
//	return SharedPtr<T>(smart_pointer<T>(static_cast<T *>(sp.get()),sp.data->ref_count));
}


}//ending namespace
#endif
