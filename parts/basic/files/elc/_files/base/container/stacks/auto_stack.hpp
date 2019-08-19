//defs.hpp
//at namespace elc::container::stack_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
struct auto_stack_t:base_stack_t<T>{//提头优化容器，可用于hashtable
private:
	typedef base_stack_t<T>base_t;
	typedef auto_stack_t<T>this_t;

	using base_t::base_t;
	using base_t::data_t;
	using base_t::_m;
public:
	this_t&operator=(this_t&&a)noexcept{base_t::operator=(a);return*this;}
	template<typename T_>
	maybe_fail_reference<T>get(T_&&a){
		data_t*tmp=_m,**tmp_=&_m;
		while(tmp!=null_ptr){
			if(a==tmp->_data){
				*tmp_=tmp->_next;
				tmp->_next=_m;
				_m=tmp;
				return tmp->_data;
			}
			tmp_=&tmp->_next;
			tmp=*tmp_;
		}
		return note::fail;
	}
	bool in_stack(const T&a){
		return get(a).not_fail();
	}
};
